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

#include "table.h"

#include <iostream>

namespace data {

/** public ********************************************************************/

  Table& Table::operator=(const Table &rhs)
  {
    _attributes = rhs._attributes;
    _columns = rhs._columns;
    _funcdeps = rhs._funcdeps;
    _funcdepGraph = rhs._funcdepGraph;
    _columnsSorted = rhs._columnsSorted;
    _cind = rhs._cind;
    _columnPrimaryKeyGroups = rhs._columnPrimaryKeyGroups;
    _out = rhs._out;
    _rows = rhs._rows;
    _rowsToHarden = rhs._rowsToHarden;

    return *this;
  }

  bool Table::getAttribute(Table::e_Attributes type, std::string &out)
  {
    if(_attributes.find(type) == _attributes.end())
      return false;
    out = _attributes[type];
    return true;
  }

  void Table::newFuncdep(bool fkdepFlag, bool reverseFDFlag)
  {
    data::Funcdep *newFuncdep = new data::Funcdep(fkdepFlag, reverseFDFlag);
    _funcdeps.push_back(newFuncdep);
  }

  bool Table::startCondIncDep()
  {
    if(_cind)
      return false;
    _cind = new data::CondIncDep();
    return true;
  }

  void Table::addColumnToPrimaryKeyGroup(std::string& group, data::Column *col)
  {
    if(_columnPrimaryKeyGroups.find(group) == _columnPrimaryKeyGroups.end())
      _columnPrimaryKeyGroups[group] = std::vector<data::Column*>();

    _columnPrimaryKeyGroups[group].push_back(col);
  }

  void Table::normalize()
  {
    std::vector<data::Column*>::iterator i = _columns.begin();
    for(;i != _columns.end(); i++)
    {
      int n = getKeyGroupPos(*i);
      int m = getKeyGroupSize(*i);
      (*i)->setupMulticolumKeyConstraints(n, m, _rows);
    }
  }

  bool Table::sortColumns()
  {
    fillColumnsGraph();
    return buildColumnsSequence();
  }

  bool Table::resortColumns()
  {
    refillColumnsGraph();
    return rebuildColumnsSequence();
  }

  void Table::priorizeColumn(data::Column *c)
  {
    std::vector<data::Column*>::iterator i = _columnsSorted.begin();
    for(; (*i) != c && i != _columnsSorted.end(); i++);
    if(i != _columnsSorted.end())
    {
      std::vector<data::Column*>::iterator j = i;
      while(j != _columnsSorted.begin())
      {
        j--;
        std::cout << "before: " << (j - _columnsSorted.begin()) << ": " << (*j) << ", " << ((j + 1) - _columnsSorted.begin()) << ": " << (*(j+1)) << std::endl;
        data::Column *tmp;
        tmp = (*(j+1));
        (*(j+1)) = (*j);
        (*j) = tmp;
      }
    }
  }

  void Table::print(bool noHeader, bool hardenFds)
  {
    openOutstream();

    if(!noHeader)
      printHeader();

    nextNoIncrement();
    printRow();
    unsigned long long i;
    for(i = 1; i < _rows; i++)
    {
      next();
      printRow();
    }

    if(hardenFds)
      harden();

    closeOutstream();
  }

  data::Column* Table::findColumnByName(std::string &in)
  {
    std::vector<data::Column*>::iterator i;
    std::string columnName;
    for(i = _columns.begin(); i != _columns.end(); i++)
    {
      (*i)->getAttribute(data::Column::A_Name, columnName);
      if(!columnName.compare(in))
        return (*i);
    }

    return NULL;
  }

/** private *******************************************************************/

  void Table::openOutstream()
  {
    std::string name = _attributes[Table::A_Name] + ".csv";
    _out->open(name.c_str());
    std::vector<data::Column*>::iterator i = _columns.begin();
    for(;i != _columns.end(); i++)
      (*i)->populateOutstream(_out);
  }

  void Table::closeOutstream()
  {
    _out->close();
  }

  void Table::printHeader()
  {
    std::vector<data::Column*>::iterator i = _columns.begin();
    std::string st;
    (*i)->getAttribute(data::Column::A_Name, st);
    (*_out) << st;
    i++;
    for(;i != _columns.end(); i++)
    {
      (*i)->getAttribute(data::Column::A_Name, st);
      (*_out) << "," << st;
    }
    (*_out) << std::endl;
  }

  void Table::next()
  {
    std::vector<data::Column*>::iterator i = _columnsSorted.begin();
    for(; i != _columnsSorted.end(); i++)
      (*i)->next();
  }

  void Table::nextNoIncrement()
  {
    std::vector<data::Column*>::iterator i = _columnsSorted.begin();
    for(; i != _columnsSorted.end(); i++)
      (*i)->nextNoIncrement();
  }

  void Table::printRow()
  {
    std::vector<data::Column*>::iterator i = _columns.begin();

    (*i)->print();
    i++;
    for(; i != _columns.end(); i++)
    {
      (*_out) << ",";
      (*i)->print();
    }
    (*_out) << std::endl;
  }

  void Table::harden()
  {
    /** build lookup index **/
    std::map<data::Column*, unsigned int> indexByColumn = std::map<data::Column*, unsigned int>();
    std::vector<data::Column*> rhsCols = std::vector<data::Column*>();

    std::vector<data::Funcdep*>::iterator fd = _funcdeps.begin();
    int lhs_index = 1;
    for(; fd != _funcdeps.end(); fd++)
    {
      indexByColumn[*((*fd)->rhs_begin())] = 0;
      rhsCols.push_back(*((*fd)->rhs_begin()));
      std::vector<data::Column*>::iterator lhs = (*fd)->lhs_begin();
      for(; lhs != (*fd)->lhs_end(); lhs++)
      {
        indexByColumn[*lhs] = lhs_index;
        lhs_index++;
      }
    }

    unsigned int index;
    std::vector<data::Column*>::iterator c;
    /** first pass **/
    for(index = 1; index <= _rowsToHarden; index++)
    {
      for(c = _columns.begin(); c != _columns.end(); c++)
      {
        if(indexByColumn.find(*c) == indexByColumn.end())
          (*c)->nextHarden();
        else if(indexByColumn[*c] == index)
          (*c)->incrementTemp();
      }
      for(c = rhsCols.begin(); c != rhsCols.end(); c++)
        (*c)->next();

      c = _columns.begin();

      (*c)->print();
      if(indexByColumn[*c] == index)
        (*c)->resetTemp();
      c++;
      for(; c != _columns.end(); c++)
      {
        (*_out) << ",";
        (*c)->print();
        if(indexByColumn[*c] == index)
          (*c)->resetTemp();
      }
      (*_out) << std::endl;
    }
    /** second pass **/
    for(index = 1; index <= _rowsToHarden; index++)
    {
      for(c = _columns.begin(); c != _columns.end(); c++)
      {
        if(indexByColumn.find(*c) == indexByColumn.end())
          (*c)->nextHarden();
        else if(indexByColumn[*c] == index)
          (*c)->incrementTempSmall();
      }
      for(c = rhsCols.begin(); c != rhsCols.end(); c++)
        (*c)->next();

      c = _columns.begin();

      (*c)->print();
      c++;
      for(; c != _columns.end(); c++)
      {
        (*_out) << ",";
        (*c)->print();
        if(indexByColumn[*c] == index)
          (*c)->resetTemp();
      }
      (*_out) << std::endl;
    }
  }

  int Table::getKeyGroupPos(data::Column *col)
  {
    std::map<std::string, std::vector<data::Column*> >::iterator i;
    for(i = _columnPrimaryKeyGroups.begin(); i != _columnPrimaryKeyGroups.end(); i++)
    {
      std::vector<data::Column*>::iterator j;
      int k = 0;
      for(j = (*i).second.begin(); j != (*i).second.end(); j++)
      {
        if((*j) == col)
          return k;
        k++;
      }
    }
    return -1;
  }

  int Table::getKeyGroupSize(data::Column *col)
  {
    std::map<std::string, std::vector<data::Column*> >::iterator i;
    for(i = _columnPrimaryKeyGroups.begin(); i != _columnPrimaryKeyGroups.end(); i++)
    {
      std::vector<data::Column*>::iterator j;
      int k = 0;
      for(j = (*i).second.begin(); j != (*i).second.end(); j++)
      {
        if((*j) == col)
          return (*i).second.size();
        k++;
      }
    }
    return -1;
  }

  void Table::fillColumnsGraph()
  {
    std::vector<data::Column*>::iterator i = _columns.begin();
    for(; i != _columns.end(); i++)
      _funcdepGraph->push_back(*i);

    std::vector<data::Funcdep*>::iterator j = _funcdeps.begin();
    for(; j != _funcdeps.end(); j++)
      _funcdepGraph->push_back(*j);
  }

  bool Table::buildColumnsSequence()
  {
    data::Column *c = _funcdepGraph->pop_back();
    while(c)
    {
      _columnsSorted.push_back(c);
      registerColumnReferences(c);
      c = _funcdepGraph->pop_back();
    }

    return _columns.size() == _columnsSorted.size();
  }

  void Table::refillColumnsGraph()
  {
    _funcdepGraph->clear();
    fillColumnsGraph();
  }

  bool Table::rebuildColumnsSequence()
  {
    _columnsSorted.clear();

    data::Column *c = _funcdepGraph->pop_back();
    while(c)
    {
      _columnsSorted.push_back(c);
      registerFKColumnReferences(c);
      c = _funcdepGraph->pop_back();
    }

    return _columns.size() == _columnsSorted.size();
  }

  void Table::registerColumnReferences(data::Column *c)
  {
    std::vector<data::Funcdep*>::iterator i = _funcdeps.begin();
    for(; i != _funcdeps.end(); i++)
    {
      if(*((*i)->rhs_begin()) == c)
      {
        if((*i)->isReverseFD())
          c->registerReverseReferences((*i)->lhs());
        else
          c->registerReferences((*i)->lhs());
      }
    }
  }

  void Table::registerFKColumnReferences(data::Column *c)
  {
    std::vector<data::Funcdep*>::iterator i = _funcdeps.begin();
    for(; i != _funcdeps.end(); i++)
    {
      if(*((*i)->rhs_begin()) == c && (*i)->isFKDep())
        c->registerFKReferences((*i)->lhs());
    }
  }

} // namespaces



