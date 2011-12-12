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

#ifdef _WIN32
  #include <windows.h>
#endif

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

#ifdef _WIN32

  void File::makeAbsolute(std::string &in)
  {
    std::string path = getCwd();
    path += "\\";
    path += in;

    std::string::size_type position = path.find("\\");
    while (position != path.npos)
    {
      path.replace(position, 1, "\\\\");
      position = path.find("\\", position + 2);
    }

    in = path;
  }

#else

  void File::makeAbsolute(std::string &in)
  {
    std::string path = getCwd();
    path += "/";
    path += in;
    in = path;
  }

#endif

/** private *******************************************************************/

#ifdef _WIN32

  std::string File::getCwd()
  {
    char buffer[1024];
    GetCurrentDirectory(1024, buffer);
    return std::string(buffer);
  }

#else

  std::string File::getCwd()
  {
    char buffer[1024];
    return std::string(getcwd(buffer, 1024));
  }

#endif

} // namespaces

