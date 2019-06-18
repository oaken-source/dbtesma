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
 
#ifndef VARCHARwrapper_H
#define VARCHARwrapper_H

#include "datatypewrapper.h"

namespace data { namespace wrapper {

class VarcharWrapper : public data::wrapper::DatatypeWrapper
{

  /**
    varchar field wrapper class
  **/

public:
  VarcharWrapper(int width) : _basevalue(), _value(), _intvalue(0), 
    _width(width), _alphabet("abcdefghijklmnopqrstuvwxyz"), 
    _alphabetSize(26) {};
  VarcharWrapper(const VarcharWrapper &obj) : DatatypeWrapper(), 
    _basevalue(obj._basevalue), _value(obj._value), _intvalue(obj._intvalue), 
    _width(obj._width), _alphabet(obj._alphabet), 
    _alphabetSize(obj._alphabetSize) {};
  VarcharWrapper& operator=(const VarcharWrapper&);
  ~VarcharWrapper();

  void generateRandomInRange();
  void generateRandom();
  void print();
  void resetToBasevalue();
  
  void getBasevalue(std::string&);
  bool setBasevalue(std::string&);
  void zeroBasevalue();
  void seedBasevalue();

  void setValue(unsigned long long);
  void setValueInRange(unsigned long long);
  unsigned long long getValue();
  
  VarcharWrapper& operator++();

  std::string translate(unsigned int);

private:

  unsigned char *_basevalue;
  unsigned char *_value;
  
  unsigned long long _intvalue;
  
  unsigned int _width;

  const char *_alphabet;
  unsigned int _alphabetSize;
  
};

}} // namespaces

#endif
