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

namespace DATA {

  CondIncDep& CondIncDep::operator=(const CondIncDep &rhs)
  {
    _rowsPerPacket = rhs._rowsPerPacket;
  
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
    
    return *this;
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
      _conditions.push_back(new Condition{size});
  }
  
} // namespaces
