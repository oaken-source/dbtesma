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

#ifndef STRINGS_H
#define STRINGS_H

/** include testing functionality, if building the test suite **/
#ifdef MAKE_TESTS
  #include "../tests/stringhelpertest.h"
#endif

#include <string>
#include <sstream>

namespace helper {

class Strings
{

  /** Strings helper class
  tasks:
    provides string functionality used mainly for schema parsing **/

  #ifdef MAKE_TESTS
    friend class StringsTest;
  #endif

public:
  /** remove a comment from a string - use _commentDelimiter as delimiter **/
  static void stripComments(std::string&);

  /** trim methods **/
  static void trim(std::string&);
  static void trimleft(std::string&);
  static void trimright(std::string&);

  /** check if a string is empty (has length of 0) **/
  static bool empty(std::string&);

  /** check if a string starts with a given value, and remove the occurrence
    param1:  string to be checked
    param2:  substring to be searched
    returns  true, if substring has been found and erased, false otherwise **/
  static bool stripleft(std::string&, const char[]);

  /** if param1 starts with an enquoted value, pop the value to param2
    param1: string to be checked
    param2: string that gets the enquoted value written to
    returns true, if a value has been written, false otherwise **/
  static bool popQuotedValue(std::string&, std::string&);

  /** check if param1 starts with a value preceding ':', pop the value to param2
    param1: string to be checked
    param2: string that gets the found value written to
    returns true, if a value has been written, false otherwise **/
  static bool popColonSeparatedValue(std::string&, std::string&);

  /** similar to popTableName, but uses ',' as a delimiter instead of ':' **/
  static bool popCSV(std::string&, std::string&);

  /** conversion methods **/
  static int intval(std::string&);
  static int uintval(std::string&);
  static unsigned long long ullval(std::string&);
  static void strval(std::string&, unsigned long long);
  static double doubval(std::string&);

  /** searches for param2 in param1 of max. length of param3
    returns the index of the occurrence, if one was found, else -1 **/
  static int getIndexFromAlphabet(const char*, char, unsigned int);

private:

  /** array containing whitespace characters **/
  static const char* _whitespace;
  /** array containing comment delimiter characters **/
  static const char* _commentDelimiter;

};

} // namespaces

#endif

