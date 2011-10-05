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
 
#ifndef FONTCOLOR_H
#define FONTCOLOR_H

#include <string>

#ifdef _WIN32
  #include <windows.h>
#else
  #include <unistd.h>
  #include <iostream>
#endif

namespace HELPER {

class FontColor
{

  /** Font Colour helper class
  tasks:
    handles coloured output to stdout **/

public:

  /** stdout color handling **/
  static void red();
  static void green();
  static void yellow();
  static void reset();

private:
  
};

} // namespaces

#endif

