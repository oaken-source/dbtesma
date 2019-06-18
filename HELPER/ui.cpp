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

#include "ui.h"

#include <cstdarg>
#include <cstdio>

namespace HELPER {

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
    std::cout << "      [";
    HELPER::FontColor::red();
    std::cout << " !! ";
    HELPER::FontColor::reset();
    std::cout << "]  ";
    va_list vl;
    va_start(vl, in);
    char buf[512];
    vsnprintf(buf, 512, in, vl);
    std::cout << buf << std::endl;
    va_end(vl);
  }

  void Ui::printwrn(const char in[], ...)
  {
    std::cout << "      [";
    HELPER::FontColor::yellow();
    std::cout << " -- ";
    HELPER::FontColor::reset();
    std::cout << "]  ";
    va_list vl;
    va_start(vl, in);
    char buf[512];
    vsnprintf(buf, 512, in, vl);
    std::cout << buf << std::endl;
    va_end(vl);
  }

  void Ui::printok()
  {
    std::cout << "      [";
    HELPER::FontColor::green();
    std::cout << " ok ";
    HELPER::FontColor::reset();
    std::cout << "]" << std::endl;
  }

  void Ui::overrok()
  {
    std::cout << "\r" << "      [";
    HELPER::FontColor::green();
    std::cout << " ok ";
    HELPER::FontColor::reset();
    std::cout << "]" << std::endl;
  }

  void Ui::overrfail()
  {
    std::cout << "\r" << "      [";
    HELPER::FontColor::red();
    std::cout << " !! ";
    HELPER::FontColor::reset();
    std::cout << "]" << std::endl;
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
    std::cout << "      [    ] " << name  << std::flush;
  }

} // namespaces

