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

namespace DATA {

class Table
{

	/**
	Table Data Class
	tasks:
		holds table related information
		implements table-scoped data generation logic
	**/

public:

	enum ATTRIBUTES
	{
		ATTR_NONE,
		ATTR_NAME,
		ATTR_ROWS
	} typedef ATTRIBUTES;
	
	Table() : _attributes(std::map<ATTRIBUTES, std::string>()), _columns(std::vector<DATA::Column*>()), _funcdeps(std::vector<DATA::Funcdep*>()), _funcdepGraph(new DATA::FuncdepGraph()), _columnsSorted(std::vector<DATA::Column*>()), _columnPrimaryKeyGroups(std::map<std::string, std::vector<DATA::Column*> >()), _out(new std::ofstream()), _rows(0), _rowsToHarden(0) {};
	Table(const Table &obj) : _attributes(obj._attributes), _columns(obj._columns), _funcdeps(obj._funcdeps), _funcdepGraph(obj._funcdepGraph), _columnsSorted(obj._columnsSorted), _columnPrimaryKeyGroups(obj._columnPrimaryKeyGroups), _out(obj._out), _rows(obj._rows), _rowsToHarden(obj._rowsToHarden) {};
	Table& operator=(const Table&);
	~Table();

	void setAttribute(ATTRIBUTES, std::string&);
	bool getAttribute(ATTRIBUTES, std::string&);
	void setColumnAttribute(DATA::Column::ATTRIBUTES, std::string&);
	void newColumn();
	
	void newFuncdep(bool, bool);
	void setFuncdepLhs(std::string);
	void setFuncdepRhs(std::string);
	
	void addFuncdepLhsColumn(DATA::Column *c) { _funcdeps.back()->addLhsColumn(c); }
	void addFuncdepRhsColumn(DATA::Column *c) { _funcdeps.back()->addRhsColumn(c); }
	
	void setFuncdeps(std::vector<DATA::Funcdep*> vec) { _funcdeps.swap(vec); }
	
	void addColumnToPrimaryKeyGroup(std::string&, DATA::Column*);

	void setRowCount(unsigned long long);
	unsigned long long getRowCount();
	
	void setRowsToHarden(unsigned int x) { _rowsToHarden = x; }

	void normalize();
	
	bool sortColumns();
	bool resortColumns();

	void print(bool, bool);

	std::vector<DATA::Column*>::iterator columns_begin() { return _columns.begin(); }
	std::vector<DATA::Column*>::iterator columns_end() { return _columns.end(); }
	std::vector<DATA::Column*>::size_type columns_size() { return _columns.size(); }
	
	std::vector<DATA::Funcdep*>::iterator funcdeps_begin() { return _funcdeps.begin(); }
	std::vector<DATA::Funcdep*>::iterator funcdeps_end() { return _funcdeps.end(); }
	std::vector<DATA::Funcdep*>::size_type funcdeps_size() { return _funcdeps.size(); }

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
	
	std::map<ATTRIBUTES, std::string> _attributes;
	std::vector<DATA::Column*> _columns;
	std::vector<DATA::Funcdep*> _funcdeps;
	
	DATA::FuncdepGraph* _funcdepGraph;
	std::vector<DATA::Column*> _columnsSorted;
	
	std::map<std::string, std::vector<DATA::Column*> > _columnPrimaryKeyGroups;

	std::ofstream *_out;

	/** metrics **/
	unsigned long long _rows;
	unsigned int _rowsToHarden;

};

} // namespaces

#endif // TABLE_H
