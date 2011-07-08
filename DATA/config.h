#ifndef CONFIG_H
#define CONFIG_H

#include "table.h"

#include <iostream>
#include "../HELPER/osabstractionhelper.h"

namespace DATA {

class Config
{

	/**
	Config Data Class
	tasks:
		contains the top-level schema information
	**/

public:
	Config() : _error(""), _tables(std::vector<DATA::Table*>()), _hardened_fds(false) {};
	~Config() { };

	void setErrorString(std::string &in) { _error = in; }
	std::string getErrorString() { return _error; }

	void newTable();
	void setTableAttribute(DATA::Table::ATTRIBUTES type, std::string &in) { _tables.back()->setAttribute(type, in); }
	void newColumn() { _tables.back()->newColumn(); }
	void setColumnAttribute(DATA::Column::ATTRIBUTES type, std::string &in) { _tables.back()->setColumnAttribute(type, in); }
	void newFuncdep() { _tables.back()->newFuncdep(false, false); }
	void setFuncdepLhs(std::string lhs) { _tables.back()->setFuncdepLhs(lhs); }
	void setFuncdepRhs(std::string rhs) { _tables.back()->setFuncdepRhs(rhs); }
	

	/** debug **/
	void dumpToStdout();
	
	/** schema scans **/
	void buildSchema();
	void buildSchemaWithoutDatatypes();
	void buildSchemaAsJSON();

	/** access **/ 
	std::vector<DATA::Table*>::size_type size() { return _tables.size(); }
	std::vector<DATA::Table*>::iterator begin() { return _tables.begin(); }
	std::vector<DATA::Table*>::iterator end() { return _tables.end(); }

	void setHardenFdFlag(bool flag) { _hardened_fds = flag; }
	bool hasHardenedFds() { return _hardened_fds; }
	
	DATA::Table* findTableByName(std::string&);

private:

	std::string _error;
	std::vector<DATA::Table*> _tables;
	
	bool _hardened_fds;

};

} // namespaces

#endif // CONFIG_H
