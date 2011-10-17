/******************************************************************************
 *                          dbtesma data generator                             *
 *                                                                             *
 *    Copyright (C) 2011  Andreas Henning                                      *
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

namespace CONF {

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
    
    std::vector<DATA::Table*>::iterator i = _conf->begin();
    for(; i != _conf->end(); i++)
    {
      std::string name;
      if((*i)->getAttribute(DATA::Table::A_Name, name))
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
    std::vector<DATA::Table*>::iterator i = _conf->begin();
    for(; i != _conf->end(); i++)
    {
      std::string name;
      (*i)->getAttribute(DATA::Table::A_Name, name);    
    
      if(!validateTableAttributes((*i), name.c_str())
        || !validateTableColumnCount((*i), name.c_str())
        || !validateTableColumnNames((*i), name.c_str())
        || !validateTableColumns((*i), name.c_str())
        || !validateTableFuncdeps((*i), name.c_str())
        || !validateTableForeignKeys((*i), name.c_str())
        || !revalidateTableFuncdeps((*i), name.c_str())
        || (_conf->hasHardenedFds() 
          && !validateTableHarden((*i), name.c_str())))
        return false;
    }
    return true;
  }

  bool Validator::validateTableAttributes(DATA::Table *t, const char tn[])
  {
    std::string rows_str;
    if(!t->getAttribute(DATA::Table::A_Rows, rows_str))
    {
      _conf->setError("%s: missing required attribute 'rows'", tn);
      return false;
    }
    unsigned long long rows = HELPER::Strings::intval(rows_str);
    t->setRowCount(rows);
    return true;
  }

  bool Validator::validateTableColumnCount(DATA::Table *t, const char tn[])
  {
    if(!t->columns_size())
    {
      _conf->setError("%s: missing column definition", tn);
      return false;
    }
    return true;
  }

  bool Validator::validateTableColumnNames(DATA::Table *t, const char tn[])
  {
    std::map<std::string, bool> columnNames;

    std::vector<DATA::Column*>::iterator i = t->columns_begin();
    for(; i != t->columns_end(); i++)
    {
      std::string name;
      if(!(*i)->getAttribute(DATA::Column::ATTR_NAME, name))
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

  bool Validator::validateTableColumns(DATA::Table *t, const char tn[])
  {
    std::vector<DATA::Column*>::iterator i = t->columns_begin();
    for(; i != t->columns_end(); i++)
    {
      std::string name;
      (*i)->getAttribute(DATA::Column::ATTR_NAME, name);
      
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
  
  bool Validator::validateColumnLength(DATA::Column *c, const char tn[],
    const char cn[])
  {
    std::string length_str;
    if(!c->getAttribute(DATA::Column::ATTR_LENGTH, length_str))
    {
      _conf->setError("%s: %s: missing required attribute 'length'", tn, cn);
      return false;
    }
    unsigned long long length = HELPER::Strings::intval(length_str);
    c->setLength(length);
    return true;
  }

  bool Validator::validateColumnDatatype(DATA::Column *c, const char tn[], 
    const char cn[])
  {
    std::string datatype;
    if(c->getAttribute(DATA::Column::ATTR_DATATYPE, datatype))
    {
      if(datatype.compare("int") == 0)
        c->setDatatype(DATA::Column::INT);
      else if(datatype.compare("varchar") == 0)
        c->setDatatype(DATA::Column::VARCHAR);
      else if(datatype.compare("char") == 0)
        c->setDatatype(DATA::Column::CHAR);
      else
      {
        _conf->setError("%s: %s: invalid value for 'datatype': '%s'", tn, cn,
          datatype.c_str());
        return false;
      }
    }
    else
      c->setDatatype(DATA::Column::INT);
    return true;
  }

  bool Validator::validateColumnKey(DATA::Column *c, DATA::Table *t,
    const char tn[], const char cn[])
  {
    std::string key;
    if(c->getAttribute(DATA::Column::ATTR_KEY, key))
    {
      if(!key.compare("primary"))
      {
        c->setKey(DATA::Column::KEY_PRIMARY);
        c->setGenerationType(DATA::Column::STORED);
        std::string key_group;
        if(c->getAttribute(DATA::Column::ATTR_KEY_GROUP, key_group))
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
      c->setKey(DATA::Column::KEY_NONE);
    return true;
  }
  
  bool Validator::validateColumnBasevalue(DATA::Column *c, const char tn[],
    const char cn[])
  {
    std::string basevalue;
    if(c->getAttribute(DATA::Column::ATTR_BASEVALUE, basevalue))
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
  
  void Validator::validateColumnUniqueCount(DATA::Column *c, DATA::Table *t)
  {
    std::string unique_str;
    if(c->getAttribute(DATA::Column::ATTR_UNIQUE, unique_str))
    {
      unsigned long long uniques = HELPER::Strings::ullval(unique_str);
      c->setUniqueValueCount(uniques);        
      c->setGenerationType(DATA::Column::RANGED);
    }
    else
      c->setUniqueValueCount(t->getRowCount());
  }
  
  bool Validator::validateTableFuncdeps(DATA::Table *t, const char tn[])
  {
    std::vector<DATA::Funcdep*>::iterator i = t->funcdeps_begin();
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
  
  bool Validator::validateFuncdepColumns(DATA::Funcdep *f, DATA::Table *t, 
    const char tn[])
  {
    std::string columnsString;
    f->getLhsString(columnsString);
    
    while(!HELPER::Strings::empty(columnsString))
    {
      std::string column;
      if(HELPER::Strings::popCSV(columnsString, column))
      {
        DATA::Column *c = t->findColumnByName(column);
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
    
    while(!HELPER::Strings::empty(columnsString))
    {
      std::string column;
      if(HELPER::Strings::popCSV(columnsString, column))
      {
        
        DATA::Column *c = t->findColumnByName(column);
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
  
  void Validator::explodeFuncdeps(DATA::Table *t)
  {
    std::vector<DATA::Funcdep*> newFuncdeps;
    std::vector<DATA::Funcdep*>::iterator i = t->funcdeps_begin();
    for(; i != t->funcdeps_end(); i++)
    {
      std::vector<DATA::Column*>::iterator j = (*i)->rhs_begin();
      for(; j != (*i)->rhs_end(); j++)
      {
        DATA::Funcdep *fd = new DATA::Funcdep(**i);
        fd->replaceRhs(*j);
        newFuncdeps.push_back(fd);
      }
    }
    t->setFuncdeps(newFuncdeps);
  }  
  
  bool Validator::validateFuncdeps(DATA::Table *t, const char tn[])
  {
    /** remove self-references **/
    std::vector<DATA::Funcdep*>::iterator i = t->funcdeps_begin();
    for(; i != t->funcdeps_end(); i++)
    {
      DATA::Column *rhs = *((*i)->rhs_begin()); // dereference iterator!
      if((*i)->lhs_find(rhs) != (*i)->lhs_end())
      {
        (*i)->lhs_erase((*i)->lhs_find(rhs));
        if((*i)->lhs_size() == 0)
        {
          std::string cn;
          (rhs)->getAttribute(DATA::Column::ATTR_NAME, cn);
          _conf->setError("%s: fd: self reference on column '%s'", tn, 
            cn.c_str());
          return false;
        }
      }
    }
    
    /** validate funcdep generation direction **/
    std::map<DATA::Column*, std::vector<DATA::Funcdep*> > rhsvalues;
    
    i = t->funcdeps_begin();
    for(; i != t->funcdeps_end(); i++)
    {
      DATA::Column *rhs = *((*i)->rhs_begin());
      if(rhsvalues.find(rhs) == rhsvalues.end())
      {
        rhsvalues[rhs] = std::vector<DATA::Funcdep*>();
        rhsvalues[rhs].push_back(*i);
      }
      else
        rhsvalues[rhs].push_back(*i);
    }

    std::vector<DATA::Funcdep*> newFuncdeps = std::vector<DATA::Funcdep*>();
    std::map<DATA::Column*, bool> occupiedColumns = std::map<DATA::Column*, bool>();
    
    std::map<DATA::Column*, std::vector<DATA::Funcdep*> >::iterator r;
    for(r = rhsvalues.begin(); r != rhsvalues.end(); r++)
    {
      DATA::Column *rhs = (*r).first;
      if((*r).second.size() == 1)
      {
        newFuncdeps.push_back((*r).second[0]);
        occupiedColumns[rhs] = true;
      }
      else
      {
        DATA::Funcdep *head_fd = 0;
        DATA::Funcdep *last_fd = (*r).second.back();
      
        std::vector<DATA::Funcdep*>::iterator f = (*r).second.begin();
        for(; f != (*r).second.end(); f++)
        {
          bool good = true;
          std::vector<DATA::Column*>::iterator c = (*f)->lhs_begin();
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
              rhs->getAttribute(DATA::Column::ATTR_NAME, cn);              
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
              DATA::Funcdep *fd = new DATA::Funcdep(false, true);
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
  
  bool Validator::validateTableForeignKeys(DATA::Table *t, const char tn[])
  {
    std::map<DATA::Table*, std::map<std::string, std::map<unsigned int, std::vector<DATA::Column*> > > > FKDepGraph = std::map<DATA::Table*, std::map<std::string, std::map<unsigned int, std::vector<DATA::Column*> > > >();
    
    std::vector<DATA::Column*>::iterator i = t->columns_begin();
    for(; i != t->columns_end(); i++)
    {
      std::string foreignkey_str;
      if((*i)->getAttribute(DATA::Column::ATTR_FOREIGNKEY, foreignkey_str))
      {
        std::string table_name;
        if(HELPER::Strings::popTableName(foreignkey_str, table_name))
        {
          DATA::Table *tx = _conf->findTableByName(table_name);
          if(!tx)
          {
            std::string cn;
            (*i)->getAttribute(DATA::Column::ATTR_NAME, cn);
            _conf->setError("%s: %s: fk: table not found: '%s'", tn, cn.c_str(),
              table_name.c_str());
            return false;
          }
          DATA::Column *c = tx->findColumnByName(foreignkey_str);
          if(!c)
          {
            std::string cn;
            (*i)->getAttribute(DATA::Column::ATTR_NAME, cn);
            _conf->setError("%s: %s: fk: column not found: '%s'", tn, 
              cn.c_str(), foreignkey_str.c_str());
            return false;
          }
          std::string key_group;
          if(c->getAttribute(DATA::Column::ATTR_KEY_GROUP, key_group))
          {
            //-> foreign key group element found            
            int c_dex = c->getDex();
            
            if(c_dex && !(*i)->isIndependent())
            {
              std::string cn;
              (*i)->getAttribute(DATA::Column::ATTR_NAME, cn);
              _conf->setError("%s: %s: unresolvable fk-fd dependencies", tn, 
                cn.c_str());
              return false;
            }
            
            // push foreign key group element into lookup vector
            if(FKDepGraph.find(tx) == FKDepGraph.end())
              FKDepGraph[tx] = std::map<std::string, std::map<unsigned int, std::vector<DATA::Column*> > >();
            if(FKDepGraph[tx].find(key_group) == FKDepGraph[tx].end())
              FKDepGraph[tx][key_group] = std::map<unsigned int, std::vector<DATA::Column*> >();
            if(FKDepGraph[tx][key_group].find(c_dex) == FKDepGraph[tx][key_group].end())
              FKDepGraph[tx][key_group][c_dex] = std::vector<DATA::Column*>();
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
              (*i)->setGenerationType(DATA::Column::RANGED);
          }
          else
          {
            std::string base;
            c->getBasevalue(base);
            unsigned long long uniques = c->getUniqueValueCount();
            (*i)->setBasevalue(base);
            (*i)->setUniqueValueCount(uniques);
            (*i)->setGenerationType(DATA::Column::RANGED);
          }
        }
        else
        {
          std::string cn;
          (*i)->getAttribute(DATA::Column::ATTR_NAME, cn);
          _conf->setError("%s: %s: invalid foreign key: '%s'", tn, cn.c_str(), 
            foreignkey_str.c_str());
          return false;
        }
      }
    }
    
    std::map<DATA::Table*, std::map<std::string, std::map<unsigned int, std::vector<DATA::Column*> > > >::iterator f = FKDepGraph.begin();
    for(; f != FKDepGraph.end(); f++)
    {
      std::map<std::string, std::map<unsigned int, std::vector<DATA::Column*> > >::iterator g = (*f).second.begin();
      for(; g != (*f).second.end(); g++)
      {
        if((*g).second.find(0) == (*g).second.end())
        {
          _conf->setError("%s: fk: unreferenced primary key head column", tn);
          return false;
        }
        
        std::vector<DATA::Column*> lhs_head_cols = (*g).second[0];
        std::string lhs_base;
        DATA::Column *lhs_base_pt = lhs_head_cols[0];
        lhs_base_pt->getAttribute(DATA::Column::ATTR_NAME, lhs_base);
        
        std::string lhs_add = "";
        DATA::Column *lhs_add_pt = 0;
        std::map<unsigned int, std::vector<DATA::Column*> >::iterator d = (*g).second.begin();
        d++;
        for(; d != (*g).second.end(); d++)
        {
          std::string rhs;
          std::string lhs = lhs_base + lhs_add;
          DATA::Column *rhs_pt = 0;
          std::vector<DATA::Column*>::iterator c = (*d).second.begin();
          for(; c != (*d).second.end(); c++)
          {      
            (*c)->getAttribute(DATA::Column::ATTR_NAME, rhs);
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
  
  bool Validator::revalidateTableFuncdeps(DATA::Table *t, const char tn[])
  {
    /** ensure unique rhs **/
    std::map<DATA::Column*, bool> rhsvalues;
    
    std::vector<DATA::Funcdep*>::iterator i = t->funcdeps_begin();
    for(; i != t->funcdeps_end(); i++)
    {
      DATA::Column *rhs = *((*i)->rhs_begin());
      if(rhsvalues.find(rhs) == rhsvalues.end())
        rhsvalues[rhs] = true;  
      else
      {
        std::string cn;
        rhs->getAttribute(DATA::Column::ATTR_NAME, cn);
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
  
  bool Validator::validateTableHarden(DATA::Table *t, const char tn[])
  {
    if(!t->funcdeps_size())
      return true;
      
    unsigned int rowsToHarden = 0;
    
    std::map<DATA::Column*, bool> usedCols = std::map<DATA::Column*, bool>();
  
    /** search for interferences **/
    std::vector<DATA::Funcdep*>::iterator i = t->funcdeps_begin();
    for(; i != t->funcdeps_end(); i++)
    {
      rowsToHarden += (*i)->lhs_size();
      std::vector<DATA::Column*>::iterator c = (*i)->lhs_begin();
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

} // namespaces

