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

#include "fontcolor.h"

namespace helper {

/** public ********************************************************************/

#ifdef _WIN32

  void FontColor::red()
  {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, FOREGROUND_RED);
  }

  void FontColor::green()
  {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, FOREGROUND_GREEN);
  }

  void FontColor::yellow()
  {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, (FOREGROUND_RED | FOREGROUND_GREEN));
  }

  void FontColor::reset()
  {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, (FOREGROUND_RED | FOREGROUND_BLUE
      | FOREGROUND_GREEN));
  }

#else

  void FontColor::red()
  {
    std::cout << "\033[1;" << 31 << "m";
  }

  void FontColor::green()
  {
    std::cout << "\033[1;" << 32 << "m";
  }

  void FontColor::yellow()
  {
    std::cout << "\033[1;" << 33 << "m";
  }

  void FontColor::reset()
  {
    std::cout << "\033[0m";
  }

#endif

} // namespaces

