/******************************************************************************
 *                          dbtesma data generator                             *
 *                                                                             *
 *    Copyright (C) 2011 - 2019  Andreas Grapentin                             *
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

#include "ui.h"

#include <cstdarg>
#include <cstdio>

static const char *fg_red = "\033[1;31m";
static const char *fg_green = "\033[1;32m";
static const char *fg_yellow = "\033[1;33m";
static const char *fg_reset = "\033[0m";

namespace helper {

/** public ********************************************************************/

  void Ui::println(const char in[])
  {
    std::cout << in << std::endl;
  }

  void Ui::printlnspaced(const char in[], unsigned int space)
  {
    unsigned int i;
    for(i = 0; i < space; i++)
      std::cout << " ";
    std::cout << in << std::endl;
  }

  void Ui::overrln(const char in[])
  {
    std::cout << "\r" << in << std::endl;
  }

  void Ui::emptyln()
  {
    std::cout << std::endl;
  }

  void Ui::printraw(const char in[])
  {
    std::cout << in << std::flush;
  }

  void Ui::printerr(const char in[], ...)
  {
    std::cout << "      [" << fg_red << " !! " << fg_reset << "]  ";

    va_list vl;
    va_start(vl, in);
    char buf[512];
    vsnprintf(buf, 512, in, vl);
    std::cout << buf << std::endl;
    va_end(vl);
  }

  void Ui::printwrn(const char in[], ...)
  {
    std::cout << "      [" << fg_yellow << " -- " << fg_reset << "]  ";

    va_list vl;
    va_start(vl, in);
    char buf[512];
    vsnprintf(buf, 512, in, vl);
    std::cout << buf << std::endl;
    va_end(vl);
  }

  void Ui::printok()
  {
    std::cout << "      [" << fg_green << " ok " << fg_reset << "]" << std::endl;
  }

  void Ui::overrok()
  {
    std::cout << "\r      [" << fg_green << " ok " << fg_reset << "]" << std::endl;
  }

  void Ui::overrfail()
  {
    std::cout << "\r      [" << fg_red << " !! " << fg_reset << "]" << std::endl;
  }

  void Ui::printTime()
  {
    struct tm *current;
    time_t now;

    time(&now);
    current = localtime(&now);

    std::cout << "  [" << std::setfill('0') << std::setw(2) << current->tm_hour
      << ":" << std::setw(2) << current->tm_min << ":" << std::setw(2)
      << current->tm_sec << "]" << std::endl;
  }

  void Ui::startProgress(const char name[])
  {
    std::cout << "      [    ] " << name << std::flush;
  }

} // namespaces

