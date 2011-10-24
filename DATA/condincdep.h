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

#include "column.h"

#include <string>
#include <vector>

namespace DATA {

class CondIncDep
{

  struct Packet
  {
    Packet() : _values(std::vector<unsigned int>()), _rows(0) {};
  
    std::vector<unsigned int> _values;
    unsigned int _rows;
  };
  
  struct Condition
  {
    Condition(unsigned int size) : _size(size), 
      _columns(std::vector<DATA::Column*>()), 
      _fixtures(std::vector<Condition*>()) {};
  
    unsigned int _size;
    std::vector<DATA::Column*> _columns;
    std::vector<Condition*> _fixtures;
  };

public:
  CondIncDep() : _rowsPerPacket(0), _completeness(0.0), _rows(0), 
    _conditions(std::vector<Condition*>()), _lhs(std::vector<DATA::Column*>()), 
    _rhs(NULL), _completenessString(""), _rowsString(""), 
    _conditionStrings(std::vector<std::string>()), _lhsString(""), 
    _rhsString(""), _unused(std::vector<Condition*>()),
    _usedonce(std::vector<Condition*>()) { };
  CondIncDep(const CondIncDep &obj) : _rowsPerPacket(obj._rowsPerPacket), 
    _completeness(obj._completeness), _rows(obj._rows), 
    _conditions(obj._conditions), _lhs(obj._lhs), _rhs(obj._rhs), 
    _completenessString(obj._completenessString), _rowsString(obj._rowsString),
    _conditionStrings(obj._conditionStrings), _lhsString(obj._lhsString), 
    _rhsString(obj._rhsString), _unused(obj._unused), _usedonce(obj._usedonce)
    { };
  CondIncDep& operator=(const CondIncDep&);
  ~CondIncDep() { };
  
  //bool conditionSortPredicate(Condition*, Condition*);
  void buildPackets();
  
  void setRowsPerPacket(unsigned int in) { _rowsPerPacket = in; }
  
  std::string getCompletenessString() { return _completenessString; }
  double getCompleteness() {return _completeness; }
  void setCompleteness(std::string in) { _completenessString = in; }
  void setCompleteness(double in) { _completeness = in; }
  
  std::string getRowsString() { return _rowsString; }
  void setRows(std::string in) { _rowsString = in; }
  void setRows(unsigned long long in) { _rows = in; }
  
  std::string popConditionsString();
  void pushConditions(std::string in) { _conditionStrings.push_back(in); }
  void pushConditions(unsigned int, unsigned int);
  unsigned int getMaxCondSize();
  
  std::string getLhsString() { return _lhsString; }
  void setLhs(std::string in) { _lhsString = in; }
  void pushLhs(DATA::Column *in) { _lhs.push_back(in); }
  unsigned int lhsSize() { return _lhs.size(); }
  std::vector<DATA::Column*>::iterator lhsBegin() { return _lhs.begin(); }
  std::vector<DATA::Column*>::iterator lhsEnd() { return _lhs.end(); }
  
  std::string getRhsString() { return _rhsString; } 
  DATA::Column* getRhs() { return _rhs; }
  void setRhs(std::string in) { _rhsString = in; }
  void setRhs(DATA::Column *in) { _rhs = in; }
  
private:
  
  std::vector<Condition*>::iterator getGreatest(
    std::vector<Condition*>::iterator, std::vector<Condition*>::iterator);
  
  unsigned int _rowsPerPacket;

  double _completeness;
  unsigned long long _rows;
  std::vector<Condition*> _conditions;
  std::vector<DATA::Column*> _lhs;
  DATA::Column* _rhs;

  std::string _completenessString;
  std::string _rowsString;
  std::vector<std::string> _conditionStrings;
  std::string _lhsString;
  std::string _rhsString;
  
  std::vector<Condition*> _unused;
  std::vector<Condition*> _usedonce;
  
};

} // namespaces

#endif
