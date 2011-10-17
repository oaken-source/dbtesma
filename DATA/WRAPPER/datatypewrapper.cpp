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
 
#include "datatypewrapper.h"

namespace DATA { namespace WRAPPER {

/** public ********************************************************************/

  DatatypeWrapper& DatatypeWrapper::operator=(const DatatypeWrapper &rhs)
  {
    _range = rhs._range;
    _out = rhs._out;

    return *this;
  }

  DatatypeWrapper::~DatatypeWrapper() {}

  void DatatypeWrapper::populateOutstream(std::ofstream *out)
  {
    _out = out;
  }

  void DatatypeWrapper::setRange(unsigned long long range)
  {
    _range = range;
  }
  
}} // namespaces
