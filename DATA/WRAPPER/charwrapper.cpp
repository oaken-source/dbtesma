#include "charwrapper.h"

namespace DATA { namespace WRAPPER {

/** public ******************************************************************************************************/

	CharWrapper& CharWrapper::operator=(const CharWrapper &rhs)
	{
		_basevalue = rhs._basevalue;
		_value = rhs._value;
		_intvalue = rhs._intvalue;
		_width = rhs._width;
		_alphabet = rhs._alphabet;
		_alphabetSize = rhs._alphabetSize;		
	
		return *this;
	}

	CharWrapper::~CharWrapper() {}

	void CharWrapper::generateRandomInRange()
	{
		resetToBasevalue();
		unsigned long long x = rand() % _range;

		_intvalue = x;
		
		int diff;
		int i = 0;
		while(x > 0)
		{
			diff = x % _alphabetSize;
			x /= _alphabetSize;

			if(_value[i] == 255)
			{
				_value[i] = 0;
				_value[i+1] = -1;
			}

			_value[i] += diff;
			x += _value[i] / _alphabetSize;
			_value[i] %= _alphabetSize;
			i++;
		}
	}
	
	void CharWrapper::generateRandom()
	{
		resetToBasevalue();
		unsigned long long x = rand();

		_intvalue = x;
		
		int diff;
		int i = 0;
		while(x > 0)
		{
			diff = x % _alphabetSize;
			x /= _alphabetSize;

			if(_value[i] == 255)
			{
				_value[i] = 0;
				_value[i+1] = -1;
			}

			_value[i] += diff;
			x += _value[i] / _alphabetSize;
			_value[i] %= _alphabetSize;
			i++;
		}
	}

	void CharWrapper::print()
	{
		unsigned int i;
		for(i = 0; i < _width; i++)
		{
			(*_out) << _alphabet[_value[i]];
		}
	}

	void CharWrapper::printOffset()
	{
		(*_out) << _intvalue;
	}

	void CharWrapper::resetToBasevalue()
	{
		free(_value);
		_value = (unsigned char*)malloc(sizeof(unsigned char) * (_width+1));
		memcpy(_value, _basevalue, _width+1);
	}

	void CharWrapper::getBasevalue(std::string &in)
	{
		std::stringstream sst;
		unsigned int i;
		for(i = 0; i < _width; i++)
			sst << _alphabet[_value[i]];
		in = sst.str();
	}

	bool CharWrapper::setBasevalue(std::string &in)
	{	
		unsigned int length = in.length();
		if(length == _width)
		{
			_basevalue = (unsigned char*)malloc(sizeof(unsigned char) * (_width+1));
			unsigned int i;
			for(i = 0; i < length; i++)
			{
				int index = HELPER::StringHelper::getIndexFromAlphabet(_alphabet, in[i], _alphabetSize);
				if(index >= 0)
					_basevalue[i] = index;
				else
					return false;
			}
			_basevalue[length] = -1;
			resetToBasevalue();
			return true;
		}
		else
			return false;
	}

	void CharWrapper::seedBasevalue()
	{
		_basevalue = (unsigned char*)malloc(sizeof(unsigned char) * (_width+1));
		unsigned int i;
		for(i = 0; i < _width; i++)
			_basevalue[i] = rand() % _alphabetSize;
		_basevalue[_width] = 0;
		resetToBasevalue();
	}

	void CharWrapper::setValue(unsigned long long x)
	{
		resetToBasevalue();

		_intvalue = x;
		
		int diff;
		int i = 0;
		while(x > 0)
		{
			diff = x % _alphabetSize;
			x /= _alphabetSize;

			if(_value[i] == 255)
			{
				_value[i] = 0;
				_value[i+1] = -1;
			}

			_value[i] += diff;
			x += _value[i] / _alphabetSize;
			_value[i] %= _alphabetSize;
			i++;
		}
	}
	
	void CharWrapper::setValueInRange(unsigned long long in)
	{
		resetToBasevalue();
		unsigned long long x = in % _range;

		_intvalue = x;
		
		int diff;
		int i = 0;
		while(x > 0)
		{
			diff = x % _alphabetSize;
			x /= _alphabetSize;

			if(_value[i] == 255)
			{
				_value[i] = 0;
				_value[i+1] = -1;
			}

			_value[i] += diff;
			x += _value[i] / _alphabetSize;
			_value[i] %= _alphabetSize;
			i++;
		}
	}
	
	unsigned long long CharWrapper::getValue()
	{
		return _intvalue;
	}

	CharWrapper& CharWrapper::operator++()
	{
		unsigned int index = 0;
		while(index < _width)
		{
			_value[index]++;
			_value[index] %= _alphabetSize;
			if(_value[index] > 0) break;
			index++;
		}
		return *this;
	}

/** private *****************************************************************************************************/

}} // namespaces
