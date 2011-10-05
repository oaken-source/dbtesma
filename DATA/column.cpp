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

/** public ******************************************************************************************************/

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

		return *this;
	}

	Column::~Column() {}

	void Column::setAttribute(Column::ATTRIBUTES type, std::string &in)
	{
		_attributes[type] = in;
	}

	bool Column::getAttribute(Column::ATTRIBUTES type, std::string &out)
	{
		bool good = _attributes.find(type) != _attributes.end();
		if(good)
			out = _attributes[type];
		return good;
	}

	void Column::next()
	{
		(*this.*_generationMethod)();		
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
	
	void Column::resetTemp()
	{
		_wrapper->setValue(_cached);
	}
	
	void Column::print()
	{
		_wrapper->print();
	}
	
	void Column::setDatatype(Column::DATATYPES type)
	{
		_datatype = type;
	}

	Column::DATATYPES Column::getDatatype()
	{
		return _datatype;
	}
		
	void Column::setLength(unsigned long long length)
	{
		_length = length;
	}

	unsigned long long Column::getLength()
	{
		return _length;
	}

	void Column::setKey(Column::KEYTYPES type)
	{
		_key = type;
	}

	Column::KEYTYPES Column::getKey()
	{
		return _key;
	}
	
	void Column::setUniqueValueCount(unsigned long long in)
	{
		if(_unique == 0 || _unique > in)
		{
			_unique = in;
			_wrapper->setRange(in);
		}
	}

	unsigned long long Column::getUniqueValueCount()
	{
		return _unique;
	}
	
	void Column::setGenerationType(Column::GENERATION_TYPES type)
	{
		switch(type)
		{
			case Column::RANGED:
				_generationMethod = &DATA::Column::generateDataRandomInRange;
				break;
			case Column::RANDOM:
				_generationMethod = &DATA::Column::generateDataRandom;
				break;
			case Column::STORED:
				_generationMethod = &DATA::Column::generateDataKeyPrimary;
				break;
		}
	}

	void Column::setupMulticolumKeyConstraints(int n, int m, unsigned long long rows)
	{
		_dex = n;
		_siz = m;
		_rows = rows;
	}
	
	int Column::getDex()
	{
		return _dex;
	}
	
	void Column::setDex(int dex)
	{
		_dex = dex;
	}
	
	int Column::getSiz()
	{
		return _siz;
	}
	
	void Column::setSiz(int siz)
	{
		_siz = siz;
	}

	unsigned long long Column::getRows()
	{
		return _rows;
	}
	
	void Column::getBasevalue(std::string &in)
	{
		_wrapper->getBasevalue(in);
	}

	bool Column::setBasevalue(std::string &in)
	{
		return _wrapper->setBasevalue(in);
	}

	void Column::seedBasevalue()
	{
		_wrapper->seedBasevalue();
	}

	void Column::populateDatatypeWrapper()
	{
		switch(_datatype)
		{
			case Column::INT:
				_wrapper = new DATA::WRAPPER::IntWrapper(_length);
				break;
			case Column::VARCHAR:
				_wrapper = new DATA::WRAPPER::VarcharWrapper(_length);
				break;
			case Column::CHAR:
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
		_parentColumns = (DATA::Column**) malloc(sizeof(DATA::Column*) * _parentCount);
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
		_parentColumns = (DATA::Column**) malloc(sizeof(DATA::Column*) * _parentCount);
		unsigned int i;
		for(i = 0; i < in.size(); i++)
			_parentColumns[i] = in[i];
		
		_generationMethod = &DATA::Column::processReverseFuncdep;		
	}
	
	void Column::registerFKReferences(std::vector<DATA::Column*> &in)
	{
		_parentCount = in.size();
		_parentColumns = (DATA::Column**) malloc(sizeof(DATA::Column*) * _parentCount);
		unsigned int i;
		for(i = 0; i < in.size(); i++)
			_parentColumns[i] = in[i];

		if(_parentCount == 1)
			_generationMethod = &DATA::Column::processFKDepSingle;
		else
			_generationMethod = &DATA::Column::processFKDepDeep;
	}
	
	bool Column::isIndependent()
	{
		return (_generationMethod == &DATA::Column::generateDataRandom);
	}
	
/** private *****************************************************************************************************/

	void Column::generateDataRandomInRange()
	{
		_wrapper->generateRandomInRange();
	}
	
	void Column::generateDataRandom()
	{
		_wrapper->generateRandom();
	}
	
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

		if(_parentColumns[1]->_wrapper->getValue() != value || (value > 0 && rand() % _siz < _dex))
			value--;
		
		_wrapper->setValue(value);	
	}

} // namespaces
