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

#ifndef CONDINCDEP_H
#define CONDINCDEP_H

#include <string>
#include <vector>

namespace DATA {

class CondIncDep
{

public:
  CondIncDep() : _completenessString(""), _rowsString(""), _positivesString(""),
    _conditionStrings(std::vector<std::string>()), _lhsString(""), 
    _rhsString("") { };   
  ~CondIncDep() { };
  
  void setCompleteness(std::string in) { _completenessString = in; }
  void setRows(std::string in) { _rowsString = in; }
  void setPositives(std::string in) { _positivesString = in; }
  void pushConditions(std::string in) { _conditionStrings.push_back(in); }
  void setLhs(std::string in) { _lhsString = in; }
  void setRhs(std::string in) { _rhsString = in; }
  
private:
  std::string _completenessString;
  std::string _rowsString;
  std::string _positivesString;
  std::vector<std::string> _conditionStrings;
  std::string _lhsString;
  std::string _rhsString;
  
};

} // namespaces

#endif
