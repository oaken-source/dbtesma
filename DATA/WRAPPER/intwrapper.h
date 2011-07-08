#ifndef INTWRAPPER_H
#define INTWRAPPER_H

#include "datatypewrapper.h"

namespace DATA { namespace WRAPPER {

class IntWrapper : public DATA::WRAPPER::DatatypeWrapper
{

	/**
	  integer field wrapper class
	**/

public:
	IntWrapper(int width) : _basevalue(0), _value(0), _width(width) {}
	~IntWrapper();

	void generateRandomInRange();
	void generateRandom();
	void print();
	void printOffset();
	void resetToBasevalue();
	
	void getBasevalue(std::string&);
	bool setBasevalue(std::string&);
	void seedBasevalue();
	
	void setValue(unsigned long long);
	void setValueInRange(unsigned long long);
	unsigned long long getValue();

	IntWrapper& operator++();

private:

	unsigned long long _basevalue;
	unsigned long long _value;

	unsigned int _width;

};

}} // namespaces

#endif // INTWRAPPER_H
