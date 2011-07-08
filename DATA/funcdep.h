#ifndef FUNCDEP_H
#define FUNCDEP_H

#include "../DATA/column.h"

namespace DATA {

class Funcdep
{

	/**
	Funcdep Data Class
	tasks:
		contains functional dependency related information, as well as pipelined accessord to right hand side and left hand side columns
	**/

public:

	Funcdep(bool fkdepFlag, bool reverseFDFlag) : _lhsString(std::string()), _rhsString(std::string()), _lhsColumns(std::vector<DATA::Column*>()), _rhsColumns(std::vector<DATA::Column*>()), _fkdepFlag(fkdepFlag), _reverseFDFlag(reverseFDFlag) {};
	Funcdep(const Funcdep &obj) : _lhsString(std::string()), _rhsString(std::string()), _lhsColumns(obj._lhsColumns), _rhsColumns(obj._rhsColumns), _fkdepFlag(obj._fkdepFlag), _reverseFDFlag(obj._reverseFDFlag) {};
	~Funcdep() {};

	void setLhs(std::string in) { _lhsString = in; }
	void setRhs(std::string in) { _rhsString = in; }
	
	void getLhsString(std::string &value) { value = _lhsString; }
	void getRhsString(std::string &value) { value = _rhsString; }
	
	void addLhsColumn(DATA::Column *c) { _lhsColumns.push_back(c); }
	void addRhsColumn(DATA::Column *c) { _rhsColumns.push_back(c); }
	
	std::vector<DATA::Column*>::iterator rhs_begin() { return _rhsColumns.begin(); }
	std::vector<DATA::Column*>::iterator rhs_end() { return _rhsColumns.end(); }
	std::vector<DATA::Column*>::size_type rhs_size() { return _rhsColumns.size(); }
	
	void replaceRhs(DATA::Column *c) { _rhsColumns.clear(); _rhsColumns.push_back(c); }

	std::vector<DATA::Column*>& lhs() { return _lhsColumns; }
	std::vector<DATA::Column*>::iterator lhs_begin() { return _lhsColumns.begin(); }
	std::vector<DATA::Column*>::iterator lhs_end() { return _lhsColumns.end(); }
	std::vector<DATA::Column*>::iterator lhs_find(DATA::Column *c);
	std::vector<DATA::Column*>::iterator lhs_erase(std::vector<DATA::Column*>::iterator i) { return _lhsColumns.erase(i); }
	std::vector<DATA::Column*>::size_type lhs_size() { return _lhsColumns.size(); }
	
	bool isFKDep() { return _fkdepFlag; }
	bool isReverseFD() {return _reverseFDFlag; }
	
private:

	std::string _lhsString;
	std::string _rhsString;
	
	std::vector<DATA::Column*> _lhsColumns;
	std::vector<DATA::Column*> _rhsColumns;
	
	bool _fkdepFlag;
	bool _reverseFDFlag;

};

} // namespaces

#endif // FUNCDEP_H
