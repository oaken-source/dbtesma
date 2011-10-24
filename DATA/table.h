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
 
#ifndef TABLE_H
#define TABLE_H

#include "funcdepgraph.h"
#include "condincdep.h"

namespace DATA {

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
    _columns(std::vector<DATA::Column*>()), 
    _funcdeps(std::vector<DATA::Funcdep*>()), 
    _funcdepGraph(new DATA::FuncdepGraph()), 
    _columnsSorted(std::vector<DATA::Column*>()), _cind(0),
    _columnPrimaryKeyGroups(std::map<std::string, std::vector<DATA::Column*> >()), 
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
  
  void newColumn() { _columns.push_back(new DATA::Column); }
  DATA::Column* passColumn() { return _columns.back(); }
  
  void newFuncdep(bool, bool);
  DATA::Funcdep* passFuncdep() { return _funcdeps.back(); }
  
  void setFuncdeps(std::vector<DATA::Funcdep*> vec) { _funcdeps.swap(vec); }
 
  bool startCondIncDep();
  DATA::CondIncDep* passCondIncDep() { return _cind; }

  void addColumnToPrimaryKeyGroup(std::string&, DATA::Column*);

  void setRowCount(unsigned long long rows) { _rows = rows; }
  unsigned long long getRowCount() { return _rows; }
  
  void setRowsToHarden(unsigned int x) { _rowsToHarden = x; }

  void normalize();
  
  bool sortColumns();
  bool resortColumns();

  void print(bool, bool);

  std::vector<DATA::Column*>::iterator columns_begin() 
    { return _columns.begin(); }
  std::vector<DATA::Column*>::iterator columns_end() 
    { return _columns.end(); }
  std::vector<DATA::Column*>::size_type columns_size() 
    { return _columns.size(); }
  
  std::vector<DATA::Funcdep*>::iterator funcdeps_begin() 
    { return _funcdeps.begin(); }
  std::vector<DATA::Funcdep*>::iterator funcdeps_end() 
    { return _funcdeps.end(); }
  std::vector<DATA::Funcdep*>::size_type funcdeps_size() 
    { return _funcdeps.size(); }

  DATA::Column* findColumnByName(std::string&);
  
private:

  void openOutstream();
  void closeOutstream();

  void printHeader();
  void next();
  void nextNoIncrement();
  void printRow();
  
  void harden();
  
  int getKeyGroupPos(DATA::Column*);
  int getKeyGroupSize(DATA::Column*);

  void fillColumnsGraph();
  bool buildColumnsSequence();

  void refillColumnsGraph();
  bool rebuildColumnsSequence();
  
  void registerColumnReferences(DATA::Column*);
  void registerFKColumnReferences(DATA::Column*);
  
  std::map<e_Attributes, std::string> _attributes;
  std::vector<DATA::Column*> _columns;
  std::vector<DATA::Funcdep*> _funcdeps;
  
  DATA::FuncdepGraph *_funcdepGraph;
  std::vector<DATA::Column*> _columnsSorted;
  
  DATA::CondIncDep *_cind;
  
  std::map<std::string, std::vector<DATA::Column*> > _columnPrimaryKeyGroups;

  std::ofstream *_out;

  /** metrics **/
  unsigned long long _rows;
  unsigned int _rowsToHarden;

};

} // namespaces

#endif
