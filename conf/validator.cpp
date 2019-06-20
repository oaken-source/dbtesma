/******************************************************************************
 *                          dbtesma data generator                             *
 *                                                                             *
 *    Copyright (C) 2011  Andreas Grapentin                                    *
 *                                                                             *
 *    This program is free software: you can redistribute it and/or modify     *
 *    it under the terms of the GNU General Public License as published by     *
 *    the Free Software Foundation, either version 3 of the License, or        *
 *    (at your option) any later version.                                      *
 *                                                                             *
 *    This program is distributed in the hope that it will be useful,          *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *    GNU General Public License for more details.                             *
 *                                                                             *
 *    You should have received a copy of the GNU General Public License        *
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
 ******************************************************************************/

#include "validator.h"

#include <cmath>

namespace conf {

/** public ********************************************************************/

  Validator& Validator::operator=(const Validator &rhs)
  {
    _conf = rhs._conf;
    return *this;
  }

  bool Validator::validate()
  {
    return (validateTableCount()
      && validateTableNames()
      && validateTables());
  }

/** private *******************************************************************/

  bool Validator::validateTableCount()
  {
    if(_conf->size() == 0)
    {
      _conf->setError("missing table definition");
      return false;
    }
    return true;
  }

  bool Validator::validateTableNames()
  {
    std::map<std::string, bool> tableNames;

    std::vector<data::Table*>::iterator i = _conf->begin();
    for(; i != _conf->end(); i++)
    {
      std::string name;
      if((*i)->getAttribute(data::Table::A_Name, name))
      {
        if(tableNames.find(name) == tableNames.end())
          tableNames[name] = true;
        else
        {
          _conf->setError("invalid tablename '%s': not unique", name.c_str());
          return false;
        }
      }
      else
      {
        _conf->setError("missing name definition in table");
        return false;
      }
    }

    return true;
  }

  bool Validator::validateTables()
  {
    std::vector<data::Table*>::iterator i = _conf->begin();
    for(; i != _conf->end(); i++)
    {
      std::string name;
      (*i)->getAttribute(data::Table::A_Name, name);

      if(!validateTableAttributes((*i), name.c_str())
        || !validateTableColumnCount((*i), name.c_str())
        || !validateTableColumnNames((*i), name.c_str())
        || !validateTableColumns((*i), name.c_str())
        || !validateTableFuncdeps((*i), name.c_str())
        || !validateTableForeignKeys((*i), name.c_str())
        || !revalidateTableFuncdeps((*i), name.c_str())
        || (_conf->hasHardenedFds()
          && !validateTableHarden((*i), name.c_str()))
        || !validateTableCondIncDep((*i), name.c_str()))
        return false;
    }
    return true;
  }

  bool Validator::validateTableAttributes(data::Table *t, const char tn[])
  {
    std::string rows_str;
    if(!t->getAttribute(data::Table::A_Rows, rows_str))
    {
      _conf->setError("%s: missing required attribute 'rows'", tn);
      return false;
    }
    unsigned long long rows = helper::Strings::intval(rows_str);
    t->setRowCount(rows);
    return true;
  }

  bool Validator::validateTableColumnCount(data::Table *t, const char tn[])
  {
    if(!t->columns_size())
    {
      _conf->setError("%s: missing column definition", tn);
      return false;
    }
    return true;
  }

  bool Validator::validateTableColumnNames(data::Table *t, const char tn[])
  {
    std::map<std::string, bool> columnNames;

    std::vector<data::Column*>::iterator i = t->columns_begin();
    for(; i != t->columns_end(); i++)
    {
      std::string name;
      if(!(*i)->getAttribute(data::Column::A_Name, name))
      {
        _conf->setError("%s: missing name definition in column", tn);
        return false;
      }
      if(name.find(",") != name.npos)
      {
        _conf->setError("%s: %s: invalid charcter ','", tn, name.c_str());
        return false;
      }
      if(columnNames.find(name) != columnNames.end())
      {
        _conf->setError("%s: invalid columnname '%s': not unique", tn,
          name.c_str());
        return false;
      }
      columnNames[name] = true;
    }
    return true;
  }

  bool Validator::validateTableColumns(data::Table *t, const char tn[])
  {
    std::vector<data::Column*>::iterator i = t->columns_begin();
    for(; i != t->columns_end(); i++)
    {
      std::string name;
      (*i)->getAttribute(data::Column::A_Name, name);

      if(!validateColumnLength((*i), tn, name.c_str())
        || !validateColumnDatatype((*i), tn, name.c_str())
        || !validateColumnKey((*i), t, tn, name.c_str())
        || !validateColumnBasevalue((*i), tn, name.c_str()))
        return false;

      validateColumnUniqueCount((*i), t);
    }
    t->normalize();
    return true;
  }

  bool Validator::validateColumnLength(data::Column *c, const char tn[],
    const char cn[])
  {
    std::string length_str;
    if(!c->getAttribute(data::Column::A_Length, length_str))
    {
      _conf->setError("%s: %s: missing required attribute 'length'", tn, cn);
      return false;
    }
    unsigned long long length = helper::Strings::intval(length_str);
    c->setLength(length);
    return true;
  }

  bool Validator::validateColumnDatatype(data::Column *c, const char tn[],
    const char cn[])
  {
    std::string datatype;
    if(c->getAttribute(data::Column::A_Datatype, datatype))
    {
      if(datatype.compare("int") == 0)
        c->setDatatype(data::Column::D_Int);
      else if(datatype.compare("varchar") == 0)
        c->setDatatype(data::Column::D_Varchar);
      else if(datatype.compare("char") == 0)
        c->setDatatype(data::Column::D_Char);
      else
      {
        _conf->setError("%s: %s: invalid value for 'datatype': '%s'", tn, cn,
          datatype.c_str());
        return false;
      }
    }
    else
      c->setDatatype(data::Column::D_Int);
    return true;
  }

  bool Validator::validateColumnKey(data::Column *c, data::Table *t,
    const char tn[], const char cn[])
  {
    std::string key;
    if(c->getAttribute(data::Column::A_Key, key))
    {
      if(!key.compare("primary"))
      {
        c->setKey(data::Column::KT_Primary);
        c->setGenerationType(data::Column::GT_Stored);
        std::string key_group;
        if(c->getAttribute(data::Column::A_KeyGroup, key_group))
          t->addColumnToPrimaryKeyGroup(key_group, c);
      }
      else
      {
        _conf->setError("%s: %s: invalid value for 'key': '%s'", tn, cn,
          key.c_str());
        return false;
      }
    }
    else
      c->setKey(data::Column::KT_None);
    return true;
  }

  bool Validator::validateColumnBasevalue(data::Column *c, const char tn[],
    const char cn[])
  {
    std::string basevalue;
    if(c->getAttribute(data::Column::A_Basevalue, basevalue))
    {
      if(!c->setBasevalue(basevalue))
      {
        _conf->setError("%s: %s: invalid value for 'basevalue': '%s'", tn, cn,
          basevalue.c_str());
        return false;
      }
    }
    else
      c->seedBasevalue();
    return true;
  }

  void Validator::validateColumnUniqueCount(data::Column *c, data::Table *t)
  {
    std::string unique_str;
    if(c->getAttribute(data::Column::A_Unique, unique_str))
    {
      unsigned long long uniques = helper::Strings::ullval(unique_str);
      c->setUniqueValueCount(uniques);
      c->setGenerationType(data::Column::GT_Ranged);
    }
    else
      c->setUniqueValueCount(t->getRowCount());
  }

  bool Validator::validateTableFuncdeps(data::Table *t, const char tn[])
  {
    std::vector<data::Funcdep*>::iterator i = t->funcdeps_begin();
    for(; i != t->funcdeps_end(); i++)
    {
      if(!validateFuncdepColumns((*i), t, tn))
        return false;
    }

    explodeFuncdeps(t);

    if(!validateFuncdeps(t, tn))
      return false;

    return true;
  }

  bool Validator::validateFuncdepColumns(data::Funcdep *f, data::Table *t,
    const char tn[])
  {
    std::string columnsString;
    f->getLhsString(columnsString);

    while(!helper::Strings::empty(columnsString))
    {
      std::string column;
      if(helper::Strings::popCSV(columnsString, column))
      {
        data::Column *c = t->findColumnByName(column);
        if(!c)
        {
          _conf->setError("%s: fd: invalid column: '%s'", tn, column.c_str());
          return false;
        }
        else
          f->addLhsColumn(c);
      }
      else
      {
        _conf->setError("%s: fd: missing lhs column definition", tn);
        return false;
      }
    }

    f->getRhsString(columnsString);

    while(!helper::Strings::empty(columnsString))
    {
      std::string column;
      if(helper::Strings::popCSV(columnsString, column))
      {

        data::Column *c = t->findColumnByName(column);
        if(!c)
        {
          _conf->setError("%s: fd: invalid column: '%s'", tn, column.c_str());
          return false;
        }
        else
          f->addRhsColumn(c);
      }
      else
      {
        _conf->setError("%s: fd: missing rhs column definition", tn);
        return false;
      }
    }
    return true;
  }

  void Validator::explodeFuncdeps(data::Table *t)
  {
    std::vector<data::Funcdep*> newFuncdeps;
    std::vector<data::Funcdep*>::iterator i = t->funcdeps_begin();
    for(; i != t->funcdeps_end(); i++)
    {
      std::vector<data::Column*>::iterator j = (*i)->rhs_begin();
      for(; j != (*i)->rhs_end(); j++)
      {
        data::Funcdep *fd = new data::Funcdep(**i);
        fd->replaceRhs(*j);
        newFuncdeps.push_back(fd);
      }
    }
    t->setFuncdeps(newFuncdeps);
  }

  bool Validator::validateFuncdeps(data::Table *t, const char tn[])
  {
    /** remove self-references **/
    std::vector<data::Funcdep*>::iterator i = t->funcdeps_begin();
    for(; i != t->funcdeps_end(); i++)
    {
      data::Column *rhs = *((*i)->rhs_begin()); // dereference iterator!
      if((*i)->lhs_find(rhs) != (*i)->lhs_end())
      {
        (*i)->lhs_erase((*i)->lhs_find(rhs));
        if((*i)->lhs_size() == 0)
        {
          std::string cn;
          (rhs)->getAttribute(data::Column::A_Name, cn);
          _conf->setError("%s: fd: self reference on column '%s'", tn,
            cn.c_str());
          return false;
        }
      }
    }

    /** validate funcdep generation direction **/
    std::map<data::Column*, std::vector<data::Funcdep*> > rhsvalues;

    i = t->funcdeps_begin();
    for(; i != t->funcdeps_end(); i++)
    {
      data::Column *rhs = *((*i)->rhs_begin());
      if(rhsvalues.find(rhs) == rhsvalues.end())
      {
        rhsvalues[rhs] = std::vector<data::Funcdep*>();
        rhsvalues[rhs].push_back(*i);
      }
      else
        rhsvalues[rhs].push_back(*i);
    }

    std::vector<data::Funcdep*> newFuncdeps = std::vector<data::Funcdep*>();
    std::map<data::Column*, bool> occupiedColumns = std::map<data::Column*, bool>();

    std::map<data::Column*, std::vector<data::Funcdep*> >::iterator r;
    for(r = rhsvalues.begin(); r != rhsvalues.end(); r++)
    {
      data::Column *rhs = (*r).first;
      if((*r).second.size() == 1)
      {
        newFuncdeps.push_back((*r).second[0]);
        occupiedColumns[rhs] = true;
      }
      else
      {
        data::Funcdep *head_fd = 0;
        data::Funcdep *last_fd = (*r).second.back();

        std::vector<data::Funcdep*>::iterator f = (*r).second.begin();
        for(; f != (*r).second.end(); f++)
        {
          bool good = true;
          std::vector<data::Column*>::iterator c = (*f)->lhs_begin();
          for(; c != (*f)->lhs_end(); c++)
            good &= ((occupiedColumns.find(*c) == occupiedColumns.end())
              && (rhsvalues.find(*c) == rhsvalues.end())
              && ((*c)->isIndependent()));

          if(!good)
          {
            if(!head_fd)
            {
              head_fd = (*f);
              newFuncdeps.push_back(*f);
              occupiedColumns[rhs] = true;
            }
            else
            {
              std::string cn;
              rhs->getAttribute(data::Column::A_Name, cn);
              _conf->setError("%s: fd: unresolvable dependencies for '%s'", tn,
                cn.c_str());
              return false;
            }
          }
          else if((!head_fd) && ((*f) == last_fd))
          {
            head_fd = (*f);
            newFuncdeps.push_back(*f);
            occupiedColumns[rhs] = true;
          }
          else
          {
            c = (*f)->lhs_begin();
            for(; c != (*f)->lhs_end(); c++)
            {
              occupiedColumns[(*c)] = true;
              data::Funcdep *fd = new data::Funcdep(false, true);
              fd->addRhsColumn(*c);
              fd->addLhsColumn(rhs);
              newFuncdeps.push_back(fd);
            }
          }
        }
      }
    }

    t->setFuncdeps(newFuncdeps);

    if(!t->sortColumns())
    {
      _conf->setError("%s: circular functional dependencies", tn);
      return false;
    }

    return true;
  }

  bool Validator::validateTableForeignKeys(data::Table *t, const char tn[])
  {
    std::map<data::Table*, std::map<std::string, std::map<unsigned int, std::vector<data::Column*> > > > FKDepGraph = std::map<data::Table*, std::map<std::string, std::map<unsigned int, std::vector<data::Column*> > > >();

    std::vector<data::Column*>::iterator i = t->columns_begin();
    for(; i != t->columns_end(); i++)
    {
      std::string foreignkey_str;
      if((*i)->getAttribute(data::Column::A_ForeignKey, foreignkey_str))
      {
        std::string table_name;
        if(helper::Strings::popColonSeparatedValue(foreignkey_str, table_name))
        {
          data::Table *tx = _conf->findTableByName(table_name);
          if(!tx)
          {
            std::string cn;
            (*i)->getAttribute(data::Column::A_Name, cn);
            _conf->setError("%s: %s: fk: table not found: '%s'", tn, cn.c_str(),
              table_name.c_str());
            return false;
          }
          data::Column *c = tx->findColumnByName(foreignkey_str);
          if(!c)
          {
            std::string cn;
            (*i)->getAttribute(data::Column::A_Name, cn);
            _conf->setError("%s: %s: fk: column not found: '%s'", tn,
              cn.c_str(), foreignkey_str.c_str());
            return false;
          }
          std::string key_group;
          if(c->getAttribute(data::Column::A_KeyGroup, key_group))
          {
            //-> foreign key group element found
            int c_dex = c->getDex();

            if(c_dex && !(*i)->isIndependent())
            {
              std::string cn;
              (*i)->getAttribute(data::Column::A_Name, cn);
              _conf->setError("%s: %s: unresolvable fk-fd dependencies", tn,
                cn.c_str());
              return false;
            }

            // push foreign key group element into lookup vector
            if(FKDepGraph.find(tx) == FKDepGraph.end())
              FKDepGraph[tx] = std::map<std::string, std::map<unsigned int, std::vector<data::Column*> > >();
            if(FKDepGraph[tx].find(key_group) == FKDepGraph[tx].end())
              FKDepGraph[tx][key_group] = std::map<unsigned int, std::vector<data::Column*> >();
            if(FKDepGraph[tx][key_group].find(c_dex) == FKDepGraph[tx][key_group].end())
              FKDepGraph[tx][key_group][c_dex] = std::vector<data::Column*>();
            FKDepGraph[tx][key_group][c_dex].push_back(*i);

            std::string base;
            c->getBasevalue(base);
            int c_siz = c->getSiz();
            unsigned long long uniques = c->getUniqueValueCount();
            (*i)->setBasevalue(base);
            (*i)->setUniqueValueCount(uniques / c_siz);
            (*i)->setSiz(c_siz);
            (*i)->setDex(c_dex);

            if(!c_dex)
              (*i)->setGenerationType(data::Column::GT_Ranged);
          }
          else
          {
            std::string base;
            c->getBasevalue(base);
            unsigned long long uniques = c->getUniqueValueCount();
            (*i)->setBasevalue(base);
            (*i)->setUniqueValueCount(uniques);
            (*i)->setGenerationType(data::Column::GT_Ranged);
          }
        }
        else
        {
          std::string cn;
          (*i)->getAttribute(data::Column::A_Name, cn);
          _conf->setError("%s: %s: invalid foreign key: '%s'", tn, cn.c_str(),
            foreignkey_str.c_str());
          return false;
        }
      }
    }

    std::map<data::Table*, std::map<std::string, std::map<unsigned int, std::vector<data::Column*> > > >::iterator f = FKDepGraph.begin();
    for(; f != FKDepGraph.end(); f++)
    {
      std::map<std::string, std::map<unsigned int, std::vector<data::Column*> > >::iterator g = (*f).second.begin();
      for(; g != (*f).second.end(); g++)
      {
        if((*g).second.find(0) == (*g).second.end())
        {
          _conf->setError("%s: fk: unreferenced primary key head column", tn);
          return false;
        }

        std::vector<data::Column*> lhs_head_cols = (*g).second[0];
        std::string lhs_base;
        data::Column *lhs_base_pt = lhs_head_cols[0];
        lhs_base_pt->getAttribute(data::Column::A_Name, lhs_base);

        std::string lhs_add = "";
        data::Column *lhs_add_pt = 0;
        std::map<unsigned int, std::vector<data::Column*> >::iterator d = (*g).second.begin();
        d++;
        for(; d != (*g).second.end(); d++)
        {
          std::string rhs;
          std::string lhs = lhs_base + lhs_add;
          data::Column *rhs_pt = 0;
          std::vector<data::Column*>::iterator c = (*d).second.begin();
          for(; c != (*d).second.end(); c++)
          {
            (*c)->getAttribute(data::Column::A_Name, rhs);
            rhs_pt = (*c);
            t->newFuncdep(true, false);
            t->passFuncdep()->setRhs(rhs);
            t->passFuncdep()->setLhs(lhs);
            t->passFuncdep()->addRhsColumn(rhs_pt);
            t->passFuncdep()->addLhsColumn(lhs_base_pt);
            if(lhs_add_pt)
              t->passFuncdep()->addLhsColumn(lhs_add_pt);
          }

          lhs_add = ",";
          lhs_add += rhs;
          lhs_add_pt = rhs_pt;
        }
      }
    }
    return true;
  }

  bool Validator::revalidateTableFuncdeps(data::Table *t, const char tn[])
  {
    /** ensure unique rhs **/
    std::map<data::Column*, bool> rhsvalues;

    std::vector<data::Funcdep*>::iterator i = t->funcdeps_begin();
    for(; i != t->funcdeps_end(); i++)
    {
      data::Column *rhs = *((*i)->rhs_begin());
      if(rhsvalues.find(rhs) == rhsvalues.end())
        rhsvalues[rhs] = true;
      else
      {
        std::string cn;
        rhs->getAttribute(data::Column::A_Name, cn);
        _conf->setError("%s: fd: rhs_column '%s' in of foreign key", tn,
          cn.c_str());
        return false;
      }
    }

    if(!t->resortColumns())
    {
      _conf->setError("%s: conflicting fk and fd", tn);
      return false;
    }

    return true;
  }

  bool Validator::validateTableHarden(data::Table *t, const char tn[])
  {
    if(!t->funcdeps_size())
      return true;

    unsigned int rowsToHarden = 0;

    std::map<data::Column*, bool> usedCols = std::map<data::Column*, bool>();

    /** search for interferences **/
    std::vector<data::Funcdep*>::iterator i = t->funcdeps_begin();
    for(; i != t->funcdeps_end(); i++)
    {
      rowsToHarden += (*i)->lhs_size();
      std::vector<data::Column*>::iterator c = (*i)->lhs_begin();
      for(; c != (*i)->lhs_end(); c++)
      {
        if(usedCols.find(*c) != usedCols.end())
        {
          _conf->setError("%s: can't harden interferencing fd's", tn);
          return false;
        }
        usedCols[*c] = true;
      }
      if(usedCols.find(*((*i)->rhs_begin())) != usedCols.end())
      {
        _conf->setError("%s: can't harden interferencing fd's", tn);
        return false;
      }
      usedCols[*((*i)->rhs_begin())] = true;
    }

    /** check for sufficient table rows **/
    if(t->getRowCount() <= rowsToHarden * 2)
    {
      _conf->setError("%s: insufficient row count for hardening", tn);
      return false;
    }
    t->setRowCount(t->getRowCount() - rowsToHarden * 2);
    t->setRowsToHarden(rowsToHarden);

    return true;
  }

  bool Validator::validateTableCondIncDep(data::Table *t, const char tn[])
  {
    if(!t->passCondIncDep())
      return true;

    if(!validateCondIncDepAttributes(t, tn)
      || !validateCondIncDepColumns(t, tn)
      || !validateCondIncDepConditions(t, tn)
      || !validateCondIncDepLogic(t, tn))
      return false;

    return true;
  }

  bool Validator::validateCondIncDepAttributes(data::Table *t, const char tn[])
  {
    std::string attr;

    attr = t->passCondIncDep()->getCompletenessString();
    double c = helper::Strings::doubval(attr);
    if(c > 0 && c <= 1)
      t->passCondIncDep()->setCompleteness(c);
    else
    {
      _conf->setError("%s: cind: invalid value for 'completeness': '%s'", tn,
        attr.c_str());
      return false;
    }

    attr = t->passCondIncDep()->getRowsString();
    unsigned long long r = helper::Strings::ullval(attr);
    if(r > 0 && r <= t->getRowCount())
      t->passCondIncDep()->setRows(r);
    else
    {
      _conf->setError("%s: cind: invalid value for 'rows': '%s'", tn,
        attr.c_str());
      return false;
    }

    return true;
  }

  bool Validator::validateCondIncDepColumns(data::Table *t, const char tn[])
  {
    std::string attr;
    attr = t->passCondIncDep()->getLhsString();

    while(!helper::Strings::empty(attr))
    {
      std::string col;
      if(helper::Strings::popCSV(attr, col))
      {
        data::Column *c = t->findColumnByName(col);
        if(!c)
        {
          _conf->setError("%s: cind: invalid column: '%s'", tn, col.c_str());
          return false;
        }
        else
          t->passCondIncDep()->pushLhs(c);
      }
      else
      {
        _conf->setError("%s: cind: invalid lhs column definition", tn);
        return false;
      }
    }

    attr = t->passCondIncDep()->getRhsString();

    data::Column *c = t->findColumnByName(attr);
    if(!c)
    {
      _conf->setError("%s: cind: invalid column: '%s'", tn, attr.c_str());
      return false;
    }
    else
      t->passCondIncDep()->setRhs(c);


    return true;
  }

  bool Validator::validateCondIncDepConditions(data::Table *t, const char tn[])
  {
    std::string attr = t->passCondIncDep()->popConditionsString();
    while(!attr.empty())
    {
      std::string sizeString;
      helper::Strings::popColonSeparatedValue(attr, sizeString);

      unsigned int size = helper::Strings::uintval(sizeString);
      unsigned int count = helper::Strings::uintval(attr);

      if(size < 1)
      {
        _conf->setError("%s: cind: invalid size '%u' in condition", tn, size);
        return false;
      }
      if(count < 1)
      {
        _conf->setError("%s: cind: invalid count '%u' in condition", tn, count);
        return false;
      }

      t->passCondIncDep()->pushConditions(size, count);

      attr = t->passCondIncDep()->popConditionsString();
    }

    return true;
  }

  bool Validator::validateCondIncDepLogic(data::Table *t, const char tn[])
  {
    // check if max size <= no. of lhs columns
    if(t->passCondIncDep()->lhsSize() < t->passCondIncDep()->getMaxCondSize())
    {
      _conf->setError("%s: cind: size of conditions exceeds number of columns",
        tn);
      return false;
    }

    // calculate rows per packet
    t->passCondIncDep()->setRowsPerPacket(
      ceil(t->passCondIncDep()->getCompleteness() * t->getRowCount() / 2));

    // check if lhs & rhs columns valid
    std::vector<data::Column*>::iterator i = t->passCondIncDep()->lhsBegin();
    for(; i != t->passCondIncDep()->lhsEnd(); i++)
      if(!(*i)->isIndependent())
      {
        std::string cn;
        (*i)->getAttribute(data::Column::A_Name, cn);
        _conf->setError("%s: cind: non-independent column in cind lhs: '%s'",
          tn, cn.c_str());
        return false;
      }
    if(!t->passCondIncDep()->getRhs()->isIndependent())
    {
      std::string cn;
      t->passCondIncDep()->getRhs()->getAttribute(data::Column::A_Name, cn);
      _conf->setError("%s: cind: non-independent column in cind rhs: '%s'",
        tn, cn.c_str());
      return false;
    }

    // build packets
    t->passCondIncDep()->buildPackets();

    // check if completeness <= max completeness  (requires no. of packets)
    double maxCompleteness = 2.0/(t->passCondIncDep()->packetsSize());
    if(t->passCondIncDep()->getCompleteness() > maxCompleteness)
    {
      std::string cn;
      t->passCondIncDep()->getRhs()->getAttribute(data::Column::A_Name, cn);
      _conf->setError("%s: cind: completeness %s exceeds maximum of 2/%i (%f)",
        tn, t->passCondIncDep()->getCompletenessString().c_str(),
        t->passCondIncDep()->packetsSize(), maxCompleteness);
      return false;
    }

    // assign values
    data::Column *c = t->passCondIncDep()->finalizePackets();

    t->priorizeColumn(c);

    return true;
  }

} // namespaces

