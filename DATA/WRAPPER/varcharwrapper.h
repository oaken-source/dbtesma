#ifndef VARCHARWRAPPER_H
#define VARCHARWRAPPER_H

#include "datatypewrapper.h"

namespace DATA { namespace WRAPPER {

class VarcharWrapper : public DATA::WRAPPER::DatatypeWrapper
{

	/**
	  varchar field wrapper class
	**/

public:
	VarcharWrapper(int width) : _basevalue(), _value(), _intvalue(0), _width(width), _alphabet("abcdefghijklmnopqrstuvwxyz"), _alphabetSize(26) {};
	VarcharWrapper(const VarcharWrapper &obj) : DatatypeWrapper(), _basevalue(obj._basevalue), _value(obj._value), _intvalue(obj._intvalue), _width(obj._width), _alphabet(obj._alphabet), _alphabetSize(obj._alphabetSize) {};
	VarcharWrapper& operator=(const VarcharWrapper&);
	~VarcharWrapper();

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
	
	VarcharWrapper& operator++();

private:

	unsigned char *_basevalue;
	unsigned char *_value;
	
	unsigned long long _intvalue;
	
	unsigned int _width;

	const char *_alphabet;
	unsigned int _alphabetSize;
	
};

}} // namespaces

#endif // VARCHARWRAPPER_H
