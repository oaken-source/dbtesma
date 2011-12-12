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

#include "strings.h"

namespace HELPER {

/** public ********************************************************************/

  const char *Strings::_whitespace = " \t\r\n";
  const char *Strings::_commentDelimiter = "#";


  void Strings::stripComments(std::string &in)
  {
    std::string::size_type pos;

    pos = in.find(_commentDelimiter);
    if(pos != in.npos)
      in.erase(pos);
  }

  void Strings::trim(std::string &in)
  {
    trimleft(in);
    trimright(in);
  }

  void Strings::trimleft(std::string &in)
  {
    std::string::size_type pos;

    pos = in.find_first_not_of(_whitespace);
    if(pos != in.npos)
      in.erase(0, pos);
    else
      in.erase(0);
  }

  void Strings::trimright(std::string &in)
  {
    std::string::size_type pos;

    pos = in.find_last_not_of(_whitespace);
    if(pos != in.npos)
      in.erase(pos + 1);
    else
      in.erase(0);
  }

  bool Strings::empty(std::string &in)
  {
    return in.length() == 0;
  }

  bool Strings::stripleft(std::string &in, const char sub[])
  {
    std::string s = sub;
    std::string::size_type pos;
    pos = in.find(s);
    if(pos == 0)
    {
      in.erase(0,s.length());
      trimleft(in);
      return true;
    }
    else
      return false;
  }

  bool Strings::popQuotedValue(std::string &in, std::string &value)
  {
    if(stripleft(in, "\""))
    {
      std::string::size_type pos;
      pos = in.find("\"");

      if(pos != in.npos)
      {
        value = in.substr(0, pos);
        in.erase(0, pos+1);
        trimleft(in);
        return true;
      }
    }

    return false;
  }

  bool Strings::popColonSeparatedValue(std::string &in, std::string &value)
  {
    std::string key;
    key = ":";

    std::string::size_type pos;
    pos = in.find(key);

    if(pos != in.npos && pos > 0)
    {
      value = in.substr(0, pos);
      in.erase(0, pos+1);
      trimleft(in);
      return true;
    }
    else
      return false;
  }

  bool Strings::popCSV(std::string &in, std::string &value)
  {
    if(empty(in))
      return false;

    std::string key;
    key = ",";

    std::string::size_type pos;
    pos = in.find(key);

    value = in.substr(0, pos);

    if(pos == in.npos)
      in.clear();
    else
      in.erase(0, pos+1);
    trimleft(in);

    return true;
  }

  int Strings::uintval(std::string &in)
  {
    std::stringstream sst(in);
    unsigned int x;
    sst >> x;
    return x;
  }

  int Strings::intval(std::string &in)
  {
    std::stringstream sst(in);
    int x;
    sst >> x;
    return x;
  }

  unsigned long long Strings::ullval(std::string &in)
  {
    std::stringstream sst(in);
    unsigned long long x;
    sst >> x;
    return x;
  }

  void Strings::strval(std::string &in, unsigned long long value)
  {
    std::stringstream sst;
    sst << value;
    in = sst.str();
  }

  double Strings::doubval(std::string &in)
  {
    if(in.empty())
      return 0;
    std::stringstream sst(in);
    double x;
    sst >> x;
    return x;
  }

  int Strings::getIndexFromAlphabet(const char *a, char in, unsigned int length)
  {
    unsigned int i;
    for(i = 0; i < length; i++)
      if(a[i] == in) return i;
    return -1;
  }

} // namespaces

