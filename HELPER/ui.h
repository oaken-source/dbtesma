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
 
#ifndef UI_H
#define UI_H

#include "../macros.h"
#include "fontcolor.h"

#include <iostream>
#include <iomanip>
#include <time.h>

namespace HELPER {

class Ui
{

  /** User Interaction class
  tasks:
    handles command line output for convenience and continuity **/

public:

  static void println(const char[]);
  static void printlnspaced(const char[], unsigned int);
  static void overrln(const char[]);
  static void emptyln();
  static void printraw(const char[]);

  static void printerr(const char[], ...);
  static void printwrn(const char[], ...);
  static void printok();

  static void overrok();
  static void overrfail();

  static void printTime();
  
  static void startProgress(const char[]);

private:

};

} // namespaces

#endif

