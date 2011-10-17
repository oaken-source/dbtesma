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
 
#ifndef INTWRAPPER_H
#define INTWRAPPER_H

#include "datatypewrapper.h"

namespace DATA { namespace WRAPPER {

class IntWrapper : public DATA::WRAPPER::DatatypeWrapper
{

  /** integer field wrapper class **/

public:
  IntWrapper(int width) : _basevalue(0), _value(0), _width(width) {}
  ~IntWrapper();

  void generateRandomInRange();
  void generateRandom();
  void print();
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

#endif
