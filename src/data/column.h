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

#ifndef COLUMN_H
#define COLUMN_H

#include "wrapper/intwrapper.h"
#include "wrapper/varcharwrapper.h"
#include "wrapper/charwrapper.h"

#include <map>
#include <vector>

namespace data {

class Column
{

  /** Column Data Class
  tasks:
    holds column information
    contains column generation logic **/

public:

  enum e_GenTypes
  {
    GT_Ranged,
    GT_Random,
    GT_Stored
  };

  enum e_Datatypes
  {
    D_Int,
    D_Varchar,
    D_Char
  };

  enum e_KeyTypes
  {
    KT_None,
    KT_Primary
  };

  enum e_Attributes
  {
    A_None,
    A_Name,
    A_Datatype,
    A_Length,
    A_Key,
    A_Basevalue,
    A_KeyGroup,
    A_Unique,
    A_ForeignKey
  };

  Column() : _generationMethod(&data::Column::generateDataRandom),
    _attributes(std::map<e_Attributes, std::string>()), _wrapper(NULL),
    _datatype(D_Int), _length(0), _key(KT_None),
    _unique(0), _rows(0), _dex(0), _siz(0), _parentCount(0), _parentColumns(0),
    _siblingCount(0), _out(new std::ofstream()), _cached(0),
    _cindValues(std::vector<std::vector<unsigned int> >()) {};
  Column(const Column &obj) : _generationMethod(obj._generationMethod),
    _attributes(obj._attributes), _wrapper(obj._wrapper),
    _datatype(obj._datatype), _length(obj._length), _key(obj._key),
    _unique(obj._unique), _rows(obj._rows), _dex(obj._dex), _siz(obj._siz),
    _parentCount(obj._parentCount), _parentColumns(obj._parentColumns),
    _siblingCount(obj._siblingCount), _out(obj._out), _cached(obj._cached),
    _cindValues(obj._cindValues) {};
  Column& operator=(const Column&);
  ~Column() {};

  /** raw attribute accessing **/
  void setAttribute(e_Attributes t, std::string &in) { _attributes[t] = in; }
  bool getAttribute(e_Attributes, std::string&);

  /** different top-level generation methods **/
  void next() { (*this.*_generationMethod)(); }
  void nextNoIncrement();
  void nextHarden();
  void incrementTemp();
  void incrementTempSmall();
  void resetTemp() { _wrapper->setValue(_cached); }
  void print() { _wrapper->print(); }

  /** getter / setter for actual attributes **/
  void setDatatype(e_Datatypes);
  e_Datatypes getDatatype() { return _datatype; }
  void setLength(unsigned long long l) { _length = l; }
  unsigned long long getLength() { return _length; }
  void setKey(e_KeyTypes t) { _key = t; }
  e_KeyTypes getKey() { return _key; }
  void setUniqueValueCount(unsigned long long);
  unsigned long long getUniqueValueCount() { return _unique; }

  void setGenerationType(e_GenTypes);

  void setupMulticolumKeyConstraints(int, int, unsigned long long);

  /** primary key related attributes **/
  int getDex() { return _dex; }
  void setDex(int dex) { _dex = dex; }
  int getSiz() { return _siz; }
  void setSiz(int siz) { _siz = siz; }
  unsigned long long getRows() { return _rows; }

  void getBasevalue(std::string &in) { _wrapper->getBasevalue(in); }
  bool setBasevalue(std::string &in) { return _wrapper->setBasevalue(in); }
  void seedBasevalue() { _wrapper->seedBasevalue(); }

  void populateDatatypeWrapper();
  void populateOutstream(std::ofstream*);

  /** parent column registration for fds, reverse fds and fks **/
  void registerReferences(std::vector<data::Column*>&);
  void registerReverseReferences(std::vector<data::Column*>&);
  void registerFKReferences(std::vector<data::Column*>&);

  void setHeadCondIncDep(std::vector<std::vector<unsigned int> >);
  void setChildCondIncDep(data::Column*);
  void setRhsCondIncDep(data::Column*);

  bool isIndependent();

  void registerSiblingCount(unsigned int in) { _siblingCount = in; }

  std::string translate(unsigned int in) { return _wrapper->translate(in); }

private:

  /** bottom-level generation methods **/
  void generateDataRandomInRange() { _wrapper->generateRandomInRange(); }
  void generateDataRandom() { _wrapper->generateRandom(); }
  void generateDataKeyPrimary();
  void generateDataNext();

  void processFuncdep();
  void processFuncdepInRange();
  void processReverseFuncdep();
  void processFKDepSingle();
  void processFKDepDeep();
  void processHeadCondIncDep();
  void processChildCondIncDep();
  void processRhsCondIncDep();

  void (data::Column::*_generationMethod)();

  std::map<e_Attributes, std::string> _attributes;

  data::wrapper::DatatypeWrapper *_wrapper;

  e_Datatypes _datatype;
  unsigned long long _length;
  e_KeyTypes _key;
  unsigned long long _unique;

  unsigned long long _rows;
  int _dex;
  int _siz;

  unsigned int _parentCount;
  data::Column** _parentColumns;

  unsigned int _siblingCount;

  std::ofstream *_out;

  unsigned long long _cached;

  std::vector<std::vector<unsigned int> > _cindValues;

};

} // namespaces

#endif
