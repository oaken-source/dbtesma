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

#ifndef FUNCDEP_H
#define FUNCDEP_H

#include "../data/column.h"

namespace data {

class Funcdep
{

  /** Funcdep Data Class
  tasks:
    contains functional dependency related information, as well as
    pipelined accessors to right hand side and left hand side columns **/

public:

  Funcdep(bool fkdepFlag, bool reverseFDFlag) : _lhsString(std::string()),
    _rhsString(std::string()), _lhsColumns(std::vector<data::Column*>()),
    _rhsColumns(std::vector<data::Column*>()), _fkdepFlag(fkdepFlag),
    _reverseFDFlag(reverseFDFlag) {};
  Funcdep(const Funcdep &obj) : _lhsString(std::string()),
    _rhsString(std::string()), _lhsColumns(obj._lhsColumns),
    _rhsColumns(obj._rhsColumns), _fkdepFlag(obj._fkdepFlag),
    _reverseFDFlag(obj._reverseFDFlag) {};
  ~Funcdep() {};

  void setLhs(std::string in) { _lhsString = in; }
  void setRhs(std::string in) { _rhsString = in; }

  void getLhsString(std::string &value) { value = _lhsString; }
  void getRhsString(std::string &value) { value = _rhsString; }

  void addLhsColumn(data::Column *c) { _lhsColumns.push_back(c); }
  void addRhsColumn(data::Column *c) { _rhsColumns.push_back(c); }

  std::vector<data::Column*>::iterator rhs_begin() { return _rhsColumns.begin(); }
  std::vector<data::Column*>::iterator rhs_end() { return _rhsColumns.end(); }
  std::vector<data::Column*>::size_type rhs_size() { return _rhsColumns.size(); }

  void replaceRhs(data::Column*);

  std::vector<data::Column*>& lhs() { return _lhsColumns; }
  std::vector<data::Column*>::iterator lhs_begin() { return _lhsColumns.begin(); }
  std::vector<data::Column*>::iterator lhs_end() { return _lhsColumns.end(); }
  std::vector<data::Column*>::iterator lhs_find(data::Column *c);
  std::vector<data::Column*>::iterator lhs_erase(std::vector<data::Column*>::iterator i) { return _lhsColumns.erase(i); }
  std::vector<data::Column*>::size_type lhs_size() { return _lhsColumns.size(); }

  bool isFKDep() { return _fkdepFlag; }
  bool isReverseFD() {return _reverseFDFlag; }

private:

  std::string _lhsString;
  std::string _rhsString;

  std::vector<data::Column*> _lhsColumns;
  std::vector<data::Column*> _rhsColumns;

  bool _fkdepFlag;
  bool _reverseFDFlag;

};

} // namespaces

#endif
