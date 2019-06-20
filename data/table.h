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

#ifndef TABLE_H
#define TABLE_H

#include "funcdepgraph.h"
#include "condincdep.h"

namespace data {

class Table
{

  /** Table Data Class
  tasks:
    holds table related information
    implements table-scoped data generation logic **/

public:

  enum e_Attributes
  {
    A_None,
    A_Name,
    A_Rows
  };

  Table() : _attributes(std::map<e_Attributes, std::string>()),
    _columns(std::vector<data::Column*>()),
    _funcdeps(std::vector<data::Funcdep*>()),
    _funcdepGraph(new data::FuncdepGraph()),
    _columnsSorted(std::vector<data::Column*>()), _cind(0),
    _columnPrimaryKeyGroups(std::map<std::string, std::vector<data::Column*> >()),
    _out(new std::ofstream()), _rows(0), _rowsToHarden(0) {};
  Table(const Table &obj) : _attributes(obj._attributes),
    _columns(obj._columns), _funcdeps(obj._funcdeps),
    _funcdepGraph(obj._funcdepGraph), _columnsSorted(obj._columnsSorted),
    _cind(obj._cind), _columnPrimaryKeyGroups(obj._columnPrimaryKeyGroups),
    _out(obj._out), _rows(obj._rows), _rowsToHarden(obj._rowsToHarden) {};
  Table& operator=(const Table&);
  ~Table() {};

  void setAttribute(e_Attributes t, std::string &in) { _attributes[t] = in; }
  bool getAttribute(e_Attributes, std::string&);

  void newColumn() { _columns.push_back(new data::Column); }
  data::Column* passColumn() { return _columns.back(); }

  void newFuncdep(bool, bool);
  data::Funcdep* passFuncdep() { return _funcdeps.back(); }

  void setFuncdeps(std::vector<data::Funcdep*> vec) { _funcdeps.swap(vec); }

  bool startCondIncDep();
  data::CondIncDep* passCondIncDep() { return _cind; }

  void addColumnToPrimaryKeyGroup(std::string&, data::Column*);

  void setRowCount(unsigned long long rows) { _rows = rows; }
  unsigned long long getRowCount() { return _rows; }

  void setRowsToHarden(unsigned int x) { _rowsToHarden = x; }

  void normalize();

  bool sortColumns();
  bool resortColumns();
  void priorizeColumn(data::Column*);

  void print(bool, bool);

  std::vector<data::Column*>::iterator columns_begin()
    { return _columns.begin(); }
  std::vector<data::Column*>::iterator columns_end()
    { return _columns.end(); }
  std::vector<data::Column*>::size_type columns_size()
    { return _columns.size(); }

  std::vector<data::Funcdep*>::iterator funcdeps_begin()
    { return _funcdeps.begin(); }
  std::vector<data::Funcdep*>::iterator funcdeps_end()
    { return _funcdeps.end(); }
  std::vector<data::Funcdep*>::size_type funcdeps_size()
    { return _funcdeps.size(); }

  data::Column* findColumnByName(std::string&);

private:

  void openOutstream();
  void closeOutstream();

  void printHeader();
  void next();
  void nextNoIncrement();
  void printRow();

  void harden();

  int getKeyGroupPos(data::Column*);
  int getKeyGroupSize(data::Column*);

  void fillColumnsGraph();
  bool buildColumnsSequence();

  void refillColumnsGraph();
  bool rebuildColumnsSequence();

  void registerColumnReferences(data::Column*);
  void registerFKColumnReferences(data::Column*);

  std::map<e_Attributes, std::string> _attributes;
  std::vector<data::Column*> _columns;
  std::vector<data::Funcdep*> _funcdeps;

  data::FuncdepGraph *_funcdepGraph;
  std::vector<data::Column*> _columnsSorted;

  data::CondIncDep *_cind;

  std::map<std::string, std::vector<data::Column*> > _columnPrimaryKeyGroups;

  std::ofstream *_out;

  /** metrics **/
  unsigned long long _rows;
  unsigned int _rowsToHarden;

};

} // namespaces

#endif
