/******************************************************************************
 *                          dbtesma data generator                             *
 *                                                                             *
 *    Copyright (C) 2011  Andreas Grapentin                                    *
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

#include "intwrapper.h"

namespace data { namespace wrapper {

/** public ********************************************************************/

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

  void IntWrapper::resetToBasevalue()
  {
    _value = _basevalue;
  }

  void IntWrapper::getBasevalue(std::string &in)
  {
    helper::Strings::strval(in, _basevalue);
  }

  bool IntWrapper::setBasevalue(std::string &in)
  {
    if(in.length() <= _width)
    {
      _basevalue = helper::Strings::ullval(in);
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
    if(_width < 8)
    {
      unsigned int max;
      unsigned int c;
      for(max = 1, c = 0; c < _width; c++, max *= 10);
      _value %= max;
      if(_value < 1000)
        _value += 1000;
    }
    return *this;
  }

  std::string IntWrapper::translate(unsigned int in)
  {
    std::stringstream x;
    x << in;
    return x.str();
  }

/** private *******************************************************************/

}} // namespaces
