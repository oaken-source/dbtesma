#ifndef DATATYPEWRAPPER_H
#define DATATYPEWRAPPER_H

#include "../../HELPER/stringhelper.h"

#include <string.h>
#include <cstdlib>
#include <fstream>

namespace DATA { namespace WRAPPER {

class DatatypeWrapper
{

	/**
	abstract parent class for datatype wrappers
	tasks:
		implements interface to be used by the datatype wrappers
	**/

public:

	DatatypeWrapper() : _range(0), _out(new std::ofstream()) {};
	DatatypeWrapper(const DatatypeWrapper &obj) : _range(obj._range), _out(obj._out) {};
	DatatypeWrapper& operator=(const DatatypeWrapper&);
	virtual ~DatatypeWrapper();

	void populateOutstream(std::ofstream*);
	void setRange(unsigned long long);

	virtual void generateRandomInRange() = 0;
	virtual void generateRandom() = 0;
	virtual void print() = 0;
	virtual void printOffset() = 0;

	virtual void resetToBasevalue() = 0;
	
	virtual void getBasevalue(std::string&) = 0;
	virtual bool setBasevalue(std::string&) = 0;
	virtual void seedBasevalue() = 0;
	
	virtual void setValue(unsigned long long) = 0;
	virtual void setValueInRange(unsigned long long) = 0;
	virtual unsigned long long getValue() = 0;

	virtual DatatypeWrapper& operator++() = 0;
	
protected:
	
	unsigned long long _range;

	std::ofstream *_out;
	
};

}} // namespaces

#endif // DATATYPEWRAPPER_H
