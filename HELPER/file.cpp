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
 
#include "file.h"

namespace HELPER {

/** public ********************************************************************/

  bool File::exists(std::string filename)
  {
    std::ifstream file(filename.c_str());
    return(file);
  }

  bool File::writeRaw(std::string filename, const char str[])
  {
    std::ofstream of;
    of.open(filename.c_str());
    if(!of.good())
      return false;
    of << str;
    of.close();
    return true;
  }
    
} // namespaces












