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

#include "funcdep.h"

namespace data {

/** public ********************************************************************/

  void Funcdep::replaceRhs(data::Column *c)
  {
    _rhsColumns.clear();
    _rhsColumns.push_back(c);
  }

  std::vector<data::Column*>::iterator Funcdep::lhs_find(data::Column *c)
  {
    std::vector<data::Column*>::iterator i = _lhsColumns.begin();
    for(; i != _lhsColumns.end(); i++)
    {
      if((*i) == c)
        return i;
    }
    return _lhsColumns.end();

  }

} // namespaces
