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
 
#include "charwrapper.h"

namespace DATA { namespace WRAPPER {

/** public ********************************************************************/

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
        int index = HELPER::Strings::getIndexFromAlphabet(_alphabet, in[i], _alphabetSize);
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

  void CharWrapper::zeroBasevalue()
  {  
      _basevalue = (unsigned char*)malloc(sizeof(unsigned char) * (_width+1));
      for(unsigned int i = 0; i < _width; i++)
        _basevalue[i] = 0;
        
      _basevalue[_width] = -1;
      resetToBasevalue();
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
  
  std::string CharWrapper::translate(unsigned int in)
  {
    setValue(in);
    std::stringstream x;
    unsigned int i;
    for(i = 0; i < _width; i++)
    {
      x << _alphabet[_value[i]];
    }
    return x.str();
  }

}} // namespaces
