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

#include "condincdep.h"

#include <cmath>

#include <iostream>

namespace DATA {

  CondIncDep& CondIncDep::operator=(const CondIncDep &rhs)
  {
    _rowsPerPacket = rhs._rowsPerPacket;
    _packets = rhs._packets;

    _completeness = rhs._completeness;
    _rows = rhs._rows;
    _conditions = rhs._conditions;
    _lhs = rhs._lhs;
    _rhs = rhs._rhs;
    _completenessString = rhs._completenessString;
    _rowsString = rhs._rowsString;
    _conditionStrings = rhs._conditionStrings;
    _lhsString = rhs._lhsString;
    _rhsString = rhs._rhsString;

    _unused = rhs._unused;
    _usedonce = rhs._usedonce;

    return *this;
  }

  void CondIncDep::buildPackets()
  {
    std::vector<Condition*> unused = _conditions;
    std::vector<Condition*> usedonce;

    while(!unused.empty() || !usedonce.empty())
    {
      Packet *p = new Packet();
      for(unsigned int i = 0; i < _lhs.size(); i++)
        p->_values.push_back(0); // fill packet with wildcards

      p->_rows = floor(_completeness * _rows / 2);

      std::vector<DATA::Column*> freeLhs = _lhs;
      std::vector<Condition*> included;

      Condition *c;
      if(!usedonce.empty())
      {
        std::vector<Condition*>::iterator i;
        i = getGreatest(usedonce.begin(), usedonce.end());
        c = (*i);
        usedonce.erase(i);
      }
      else
      {
        std::vector<Condition*>::iterator i;
        i = getGreatest(unused.begin(), unused.end());
        c = (*i);
        usedonce.push_back(c);
        unused.erase(i);
      }

      while(c)
      {
        if(!c->_columns.size()) // picked condition is not registered to columns yet
        {
          for(unsigned int x = 0; x < c->_size; x++) // note: we assured that x->_size <= freeLhs.size()
          {
            int random = rand() % freeLhs.size();
            c->_columns.push_back(freeLhs[random]); // register column

            for(unsigned int y = 0; y < _lhs.size(); y++) // register column index
              if(_lhs[y] == freeLhs[random])
              {
                c->_columnIndices.push_back(y);
                break;
              }

            freeLhs.erase(freeLhs.begin() + random); // remove column from freeLhs
          }
        }
        else // remove columns from freeLhs anyway
        {
          for(unsigned int x = 0; x < c->_size; x++) // note: we assured that all columns of c are in freeLhs
            for(unsigned int y = 0; y < freeLhs.size(); y++)
              if(freeLhs[y] == c->_columns[x])
              {
                freeLhs.erase(freeLhs.begin() + y);
                break;
              }
        }

        for(unsigned int x = 0; x < included.size(); x++) // register fixtures
        {
          c->_fixtures.push_back(included[x]);
          included[x]->_fixtures.push_back(c);
        }

        included.push_back(c); // store away
        p->_conditions.push_back(c);

        c = getNext(freeLhs, included, &usedonce, &unused);  // get next Cond.
      }
      _packets.push_back(p);
    }
  }

  DATA::Column* CondIncDep::finalizePackets()
  {
    // assign values to conditions
    std::map<unsigned int, std::vector<unsigned int> > av;
    std::vector<Condition*>::iterator i = _conditions.begin();
    for(; i != _conditions.end(); i++)
    {
      for(unsigned int x = 0; x < (*i)->_columnIndices.size(); x++)
      {
        unsigned int v = rand() % 1000;
        if(av.find((*i)->_columnIndices[x]) == av.end())
        {
          av[(*i)->_columnIndices[x]] = std::vector<unsigned int>();
          av[(*i)->_columnIndices[x]].push_back(v);
          (*i)->_columnValues.push_back(v);
          continue;
        }
        while(1)
        {
          for(unsigned int y = 0; y < av[(*i)->_columnIndices[x]].size(); y++)
            if(av[(*i)->_columnIndices[x]][y] == v)
            {
              v++;
              continue;
            }
          av[(*i)->_columnIndices[x]].push_back(v);
          (*i)->_columnValues.push_back(v);
          break;
        }
      }
    }
    if(DEBUG)
    {
      std::cout << "condition values:" << std::endl;
      i = _conditions.begin();
      unsigned int count = 0;
      for(; i != _conditions.end(); i++)
      {
        unsigned int g;
        std::string column = "";
        _lhs[(*i)->_columnIndices[0]]->getAttribute(DATA::Column::A_Name, column);
        std::cout << " " << count << " (" << column << ":" << _lhs[(*i)->_columnIndices[0]]->translate((*i)->_columnValues[0]);
        std::cerr << "db2 \"select count(*) from dbpedia02 where " << column << "='" << _lhs[(*i)->_columnIndices[0]]->translate((*i)->_columnValues[0]) << "'";
        for(g = 1; g < (*i)->_columnIndices.size(); g++)
        {
          column = "";
          _lhs[(*i)->_columnIndices[g]]->getAttribute(DATA::Column::A_Name, column);
          std::cout << ", " << column << ":" << _lhs[(*i)->_columnIndices[g]]->translate((*i)->_columnValues[g]);
          std::cerr << " and " << column << "='" << _lhs[(*i)->_columnIndices[g]]->translate((*i)->_columnValues[g]) << "'";
        }
        std::cout << ")" << std::endl;
        std::cerr << "\"" << std::endl;
        count++;
      }
    }
    // assign values to packets
    std::vector<Packet*>::iterator p = _packets.begin();
    std::vector<std::vector<unsigned int> > values;
    unsigned int rowcount = 0;
    for(; p != _packets.end(); p++)
    {
      values.push_back(std::vector<unsigned int>());
      values.back().push_back((*p)->_rows);
      rowcount += (*p)->_rows;
      for(unsigned int x = 0; x < _lhs.size(); x++)
        values.back().push_back(0); // fill packet with wildcards

      i = (*p)->_conditions.begin();
      for(; i != (*p)->_conditions.end(); i++)
        for(unsigned int x = 0; x < (*i)->_size; x++)
        {
          values.back()[(*i)->_columnIndices[x]+1] = (*i)->_columnValues[x];
          (*p)->_values[(*i)->_columnIndices[x]] = (*i)->_columnValues[x];
        }
    }

    if(DEBUG)
    {
      std::cout << "packet pairings:" << std::endl;
      p = _packets.begin();
      for(; p != _packets.end(); p++)
      {
        std::cout << " (";
        if((*p)->_conditions.size())
        {
          i = (*p)->_conditions.begin();
          unsigned int k;
          for(k = 0; k != _conditions.size(); k++)
            if((*p)->_conditions[0] == _conditions[k])
              break;
          std::cout << k;
          i++;
          for(; i != (*p)->_conditions.end(); i++)
          {
            for(k = 0; k != _conditions.size(); k++)
              if((*i) == _conditions[k])
                break;
            std::cout << ", " << k;
          }
          std::cout << ")" << std::endl;
        }
      }
    }

    // add wildcard packet to fill row count
    if(_rows - rowcount > 0)
    {
      values.push_back(std::vector<unsigned int>());
      values.back().push_back(_rows - rowcount);
      for(unsigned int x = 0; x < _lhs.size(); x++)
        values.back().push_back(0); // fill packet with wildcards
    }

    // prepare generation

    if(DEBUG)
    {
      std::cout << "value vector:" << std::endl;
      for(unsigned int x = 0; x < values.size(); x++)
      {
        std::cout << " (" << values[x][0];
        for(unsigned int y = 1; y < values[x].size(); y++)
          std::cout << "," << values[x][y];
        std::cout << ")" << std::endl;
      }
    }

    _lhs[0]->setHeadCondIncDep(values);
    for(unsigned int x = 1; x < _lhs.size(); x++)
      _lhs[x]->setChildCondIncDep(_lhs[0]);
    _rhs->setRhsCondIncDep(_lhs[0]);

    return _lhs[0];
  }

  std::string CondIncDep::popConditionsString()
  {
    if(!_conditionStrings.size())
      return "";
    std::string res = _conditionStrings.back();
    _conditionStrings.pop_back();
    return res;
  }

  unsigned int CondIncDep::getMaxCondSize()
  {
    unsigned int res = 0;
    std::vector<CondIncDep::Condition*>::iterator i = _conditions.begin();
    for(; i != _conditions.end(); i++)
      if((*i)->_size > res)
        res = (*i)->_size;

    return res;
  }

  void CondIncDep::pushConditions(unsigned int size, unsigned int count)
  {
    unsigned int i;
    for(i = 0; i < count; i++)
    {
      Condition *c = new Condition(size);
      _conditions.push_back(c);
    }
  }

  std::vector<CondIncDep::Condition*>::iterator CondIncDep::getGreatest(
    std::vector<CondIncDep::Condition*>::iterator begin,
    std::vector<CondIncDep::Condition*>::iterator end)
  {
    std::vector<CondIncDep::Condition*>::iterator c = begin;
    std::vector<CondIncDep::Condition*>::iterator i = begin;
    for(; i != end; i++)
      if((*i)->_size > (*c)->_size)
        c = i;
    return c;
  }

  CondIncDep::Condition* CondIncDep::getNext(std::vector<DATA::Column*> freeLhs,
    std::vector<CondIncDep::Condition*> included,
    std::vector<CondIncDep::Condition*> *usedonce,
    std::vector<CondIncDep::Condition*> *unused)
  {
    Condition *c = 0;
    unsigned int maxSize = 0;
    std::vector<Condition*>::iterator i;
    for(i = usedonce->begin(); i != usedonce->end(); i++)
      if((*i)->_size > maxSize && isValidInContext(freeLhs, included, (*i)))
      {
        maxSize = (*i)->_size;
        c = (*i);
      }

    if(c) // priorize usedonce
    {
      for(i = usedonce->begin(); i != usedonce->end(); i++)
        if((*i) == c)
        {
          usedonce->erase(i);
          break;
        }
      return c;
    }

    for(i = unused->begin(); i != unused->end(); i++)
      if((*i)->_size > maxSize && isValidInContext(freeLhs, included, (*i)))
      {
        maxSize = (*i)->_size;
        c = (*i);
      }

    if(c)
    {
      for(i = unused->begin(); i != unused->end(); i++)
        if((*i) == c)
        {
          unused->erase(i);
          break;
        }
      usedonce->push_back(c);
    }
    return c; // return anyway
  }

  bool CondIncDep::isValidInContext(std::vector<DATA::Column*> freeLhs,
    std::vector<CondIncDep::Condition*> included, CondIncDep::Condition *c)
  {
    if(freeLhs.size() < c->_size)
      return false;

    for(unsigned int i = 0; i < c->_columns.size(); i++) // check for non-free
    {
      bool found = false;
      for(unsigned int j = 0; j < freeLhs.size(); j++)
        if(freeLhs[j] == c->_columns[i])
        {
          found = true;
          break;
        }

      if(!found)
        return false;
    }

    for(unsigned int i = 0; i < c->_fixtures.size(); i++) // check for fixture
      for(unsigned int j = 0; j < included.size(); j++)
        if(c->_fixtures[i] == included[j])
          return false;

    return true;
  }

} // namespaces
