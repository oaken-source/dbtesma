#include "intwrapper.h"

namespace DATA { namespace WRAPPER {

/** public ******************************************************************************************************/

	IntWrapper::~IntWrapper() {}

	void IntWrapper::generateRandomInRange()
	{
		unsigned long long x = rand() % _range;
		_value = _basevalue + x;
	}

	void IntWrapper::generateRandom()
	{
		unsigned long long x = rand() % _range;
		_value = _basevalue + x;
	}
	
	void IntWrapper::print()
	{	
		(*_out) << _value;
	}
	
	void IntWrapper::printOffset()
	{
		(*_out) << _value - _basevalue;
	}

	void IntWrapper::resetToBasevalue()
	{
		_value = _basevalue;
	}

	void IntWrapper::getBasevalue(std::string &in)
	{
		HELPER::StringHelper::strval(in, _basevalue);
	}

	bool IntWrapper::setBasevalue(std::string &in)
	{
		if(in.length() <= _width)
		{
			_basevalue = HELPER::StringHelper::ullval(in);
			resetToBasevalue();
			return true;
		}
		else
			return false;
	}

	void IntWrapper::seedBasevalue()
	{
		unsigned long long x = 0;
		unsigned int i;
		for(i = 1; i < _width; i++)
		{
			x *= 10;
			x += rand()%10;
		}
		_basevalue = x;
		resetToBasevalue();
	}

	void IntWrapper::setValue(unsigned long long in)
	{
		_value = _basevalue + in;
	}
	
	void IntWrapper::setValueInRange(unsigned long long in)
	{
		_value = _basevalue + (in % _range);
	}
	
	unsigned long long IntWrapper::getValue()
	{
		return _value - _basevalue;
	}
	
	IntWrapper& IntWrapper::operator++()
	{
		_value++;
		return *this;
	}

/** private *****************************************************************************************************/

}} // namespaces
