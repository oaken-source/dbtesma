#ifndef CHARWRAPPER_H
#define CHARWRAPPER_H

#include "datatypewrapper.h"

namespace DATA { namespace WRAPPER {

class CharWrapper : public DATA::WRAPPER::DatatypeWrapper
{

	/**
	  Char field wrapper class
	**/

public:
	CharWrapper(int width) : _basevalue(), _value(), _intvalue(0), _width(width), _alphabet("abcdefghijklmnopqrstuvwxyz"), _alphabetSize(26) {};
	CharWrapper(const CharWrapper &obj) : DatatypeWrapper(), _basevalue(obj._basevalue), _value(obj._value), _intvalue(obj._intvalue), _width(obj._width), _alphabet(obj._alphabet), _alphabetSize(obj._alphabetSize) {};
	CharWrapper& operator=(const CharWrapper&);
	~CharWrapper();

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
	
	CharWrapper& operator++();

private:

	unsigned char *_basevalue;
	unsigned char *_value;
	
	unsigned long long _intvalue;
	
	unsigned int _width;

	const char *_alphabet;
	unsigned int _alphabetSize;
	
};

}} // namespaces

#endif // CHARWRAPPER_H
