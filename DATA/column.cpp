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
 
#include "column.h"

#include <iostream>

namespace DATA {

/** public ********************************************************************/

  Column& Column::operator=(const Column &rhs)
  {
    _generationMethod = rhs._generationMethod;
    _attributes = rhs._attributes;
    _wrapper = rhs._wrapper;
    _datatype = rhs._datatype;
    _length = rhs._length;
    _key = rhs._key;
    _unique = rhs._unique;
    _rows = rhs._rows;
    _dex = rhs._dex;
    _siz = rhs._siz;
    _parentCount = rhs._parentCount;
    _parentColumns = rhs._parentColumns;
    _siblingCount = rhs._siblingCount;
    _out = rhs._out;
    _cached = rhs._cached;
    _cindValues = rhs._cindValues;

    return *this;
  }

  bool Column::getAttribute(Column::e_Attributes type, std::string &out)
  {
    bool good = _attributes.find(type) != _attributes.end();
    if(good)
      out = _attributes[type];
    return good;
  }
  
  void Column::nextNoIncrement()
  {
    if(_generationMethod != &DATA::Column::generateDataKeyPrimary)
      (*this.*_generationMethod)();
  }
  
  void Column::nextHarden()
  {
    if(_generationMethod == &DATA::Column::generateDataKeyPrimary)
      (*this.*_generationMethod)();
  }
  
  void Column::incrementTemp()
  {
    _cached = _wrapper->getValue();
    _wrapper ->setValue(_cached + _siblingCount + 1);
  }
  
  void Column::incrementTempSmall()
  {
    _cached = _wrapper->getValue();
    _wrapper ->setValue(_cached + 1);
  }

  void Column::setDatatype(Column::e_Datatypes type)
  {
    _datatype = type;
    populateDatatypeWrapper();
  }
  
  void Column::setUniqueValueCount(unsigned long long in)
  {
    if(_unique == 0 || _unique > in)
    {
      _unique = in;
      _wrapper->setRange(in);
    }
  }
  
  void Column::setGenerationType(Column::e_GenTypes type)
  {
    switch(type)
    {
      case Column::GT_Ranged:
        _generationMethod = &DATA::Column::generateDataRandomInRange;
        break;
      case Column::GT_Random:
        _generationMethod = &DATA::Column::generateDataRandom;
        break;
      case Column::GT_Stored:
        _generationMethod = &DATA::Column::generateDataKeyPrimary;
        break;
    }
  }

  void Column::setupMulticolumKeyConstraints(int n, int m, unsigned long long r)
  {
    _dex = n;
    _siz = m;
    _rows = r;
  }

  void Column::populateDatatypeWrapper()
  {
    switch(_datatype)
    {
      case Column::D_Int:
        _wrapper = new DATA::WRAPPER::IntWrapper(_length);
        break;
      case Column::D_Varchar:
        _wrapper = new DATA::WRAPPER::VarcharWrapper(_length);
        break;
      case Column::D_Char:
        _wrapper = new DATA::WRAPPER::CharWrapper(_length);
        break;
    }
  }

  void Column::populateOutstream(std::ofstream *out)
  {
    _out = out;
    _wrapper->populateOutstream(out);
  }
  
  void Column::registerReferences(std::vector<DATA::Column*> &in)
  {
    _parentCount = in.size();
    _parentColumns = (DATA::Column**)malloc(sizeof(DATA::Column*)*_parentCount);
    unsigned int i;
    for(i = 0; i < in.size(); i++)
    {
      in[i]->registerSiblingCount(_parentCount);
      _parentColumns[i] = in[i];
    }
    
    if(_generationMethod == &DATA::Column::generateDataRandomInRange)
      _generationMethod = &DATA::Column::processFuncdepInRange;
    else
      _generationMethod = &DATA::Column::processFuncdep;    
  }
  
  void Column::registerReverseReferences(std::vector<DATA::Column*> &in)
  {
    _parentCount = in.size();
    _parentColumns = (DATA::Column**)malloc(sizeof(DATA::Column*)*_parentCount);
    unsigned int i;
    for(i = 0; i < in.size(); i++)
      _parentColumns[i] = in[i];
    
    _generationMethod = &DATA::Column::processReverseFuncdep;    
  }
  
  void Column::registerFKReferences(std::vector<DATA::Column*> &in)
  {
    _parentCount = in.size();
    _parentColumns = (DATA::Column**)malloc(sizeof(DATA::Column*)*_parentCount);
    unsigned int i;
    for(i = 0; i < in.size(); i++)
      _parentColumns[i] = in[i];

    if(_parentCount == 1)
      _generationMethod = &DATA::Column::processFKDepSingle;
    else
      _generationMethod = &DATA::Column::processFKDepDeep;
  }
  
  void Column::setHeadCondIncDep(std::vector<std::vector<unsigned int> > values)
  {
    _parentCount = 0;
    _cindValues = std::vector<std::vector<unsigned int> >(values);
    _generationMethod = &DATA::Column::processHeadCondIncDep;
    if(_datatype == D_Int)
      _wrapper->zeroBasevalue();
    _cached = (rand() % 1000) + 1000;
  }
  
  void Column::setChildCondIncDep(DATA::Column *c)
  {
    _parentCount = 1;
    _parentColumns = (DATA::Column**)malloc(sizeof(DATA::Column*));
    _parentColumns[0] = c;
    _generationMethod = &DATA::Column::processChildCondIncDep;
    if(_datatype == D_Int)
      _wrapper->zeroBasevalue();
    _cached = (rand() % 1000) + 1000;
  }
  
  void Column::setRhsCondIncDep(DATA::Column *c)
  {
    _parentCount = 1;
    _parentColumns = (DATA::Column**)malloc(sizeof(DATA::Column*));
    _parentColumns[0] = c;
    _generationMethod = &DATA::Column::processRhsCondIncDep;
    if(_datatype == D_Int)
      _wrapper->zeroBasevalue();
  }
  
  bool Column::isIndependent()
  {
    return (_generationMethod == &DATA::Column::generateDataRandom);
  }
  
/** private *******************************************************************/

  void Column::generateDataKeyPrimary()
  {
    switch(_dex)
    {
      case -1:
        ++(*_wrapper);
        break;
      case 0:
        ++(*_wrapper);
        _dex = _siz-1;
        break;
      default:
        _dex--;
        break;
    }
  }
  
  void Column::generateDataNext()
  {
    _wrapper->setValue(_cached);
    ++(*_wrapper);
    _cached++;
  }

  void Column::processFuncdep()
  {
    unsigned int i;
    unsigned long long value = 0;
    for(i = 0; i < _parentCount; i++)
      value += _parentColumns[i]->_wrapper->getValue();
    
    value /= (_parentCount + 1);
    
    _wrapper->setValue(value);  
  }
  
  void Column::processFuncdepInRange()
  {
    unsigned int i;
    unsigned long long value = 0;
    for(i = 0; i < _parentCount; i++)
      value += _parentColumns[i]->_wrapper->getValue();
    
    value /= (_parentCount + 1);

    _wrapper->setValueInRange(value);  
  }
  
  void Column::processReverseFuncdep()
  {
    unsigned int i;
    unsigned long long value = 0;
    for(i = 0; i < _parentCount; i++)
      value += _parentColumns[i]->_wrapper->getValue();

    value *= _parentCount + 1;
      
    if(rand() % 2)
      value++;
    
    _wrapper->setValue(value);  
  }
  
  void Column::processFKDepSingle()
  {
    unsigned long long value = _parentColumns[0]->_wrapper->getValue();
    
    if(value > 0 && rand() % _siz < _dex)
      value--;
    
    _wrapper->setValue(value);  
  }
  
  void Column::processFKDepDeep()
  {
    unsigned long long value = _parentColumns[0]->_wrapper->getValue();

    if(_parentColumns[1]->_wrapper->getValue() != value 
      || (value > 0 && rand() % _siz < _dex))
      value--;
    
    _wrapper->setValue(value);  
  }

  void Column::processHeadCondIncDep()
  {
    if(_cindValues.size() == 0)
    {
      _siz = 0;
      _dex = -1;
      _wrapper->setValue(_cached);
      generateDataKeyPrimary();
      _generationMethod = &DATA::Column::generateDataKeyPrimary;
      return;
    }
    _dex = rand() % _cindValues.size();
    //_dex = 0;
    while(!_cindValues[_dex][0] && _cindValues.size() > 0)
    {
      _cindValues.erase(_cindValues.begin() + _dex);
      if(_cindValues.size() > 0)
        _dex = rand() % _cindValues.size();
      //_dex = 0;
    }
    if(_cindValues.size() == 0)
    {
      _siz = 0;
      _dex = -1;
      _wrapper->setValue(_cached);
      generateDataKeyPrimary();
      _generationMethod = &DATA::Column::generateDataKeyPrimary;
      return;
    }
    //std::cerr << _cindValues[_dex][0] << std::endl;
    _cindValues[_dex][0]--;
    _siz = 2;

      
    if(_cindValues[_dex][1])
      _wrapper->setValue(_cindValues[_dex][1]);  
    else
      generateDataNext();
  }

  void Column::processChildCondIncDep()
  {
    if(!_parentColumns[0]->_siz)
    {
      _dex = -1;
      _wrapper->setValue(_cached);
      generateDataKeyPrimary();
      _generationMethod = &DATA::Column::generateDataKeyPrimary;
      return;
    }
   
    if(_parentColumns[0]->_cindValues[_parentColumns[0]->_dex][_parentColumns[0]->_siz])
      _wrapper->setValue(_parentColumns[0]->_cindValues[_parentColumns[0]->_dex][_parentColumns[0]->_siz]);  
    else
      generateDataNext();

    _parentColumns[0]->_siz++;
  }

  void Column::processRhsCondIncDep()
  {   
    if(!_parentColumns[0]->_siz)
      _wrapper->setValue(0);
    else
      _wrapper->setValue(1);
  }

} // namespaces
