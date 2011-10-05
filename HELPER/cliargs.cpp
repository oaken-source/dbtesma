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
 
#include "cliargs.h"

#include <cstdarg>

namespace HELPER {

/** public ********************************************************************/

  bool CliArgs::addFlag(std::string name, int id)
  {
    if(_flag.find(name) != _flag.end() || _pair.find(name) != _pair.end())
      return false;

    addFlagParam(name, id);
    return true;
  }
  
  bool CliArgs::addPair(std::string name, int id, std::string def)
  {
    if(_flag.find(name) != _flag.end() || _pair.find(name) != _pair.end())
      return false;

    addPairParam(name, id);
    if(def.length() > 0)
      _pairDef[id] = def;
    return true;  
  }

  void CliArgs::parse(int argc, char *argv[])
  {
    clear();

    unsigned int values = 0;

    int i;
    for(i = 1; i < argc; i++)
    {
      std::string param(argv[i]);
      std::map<std::string, int>::iterator f = _flag.find(param);
      std::map<std::string, int>::iterator p = _pair.find(param);

      if(f != _flag.end()) // flag
      {
        if(_flags.find((*f).second) != _flags.end())
          setWarning("%s specified more than once", argv[i]);
        _flags[(*f).second] = true;
      }
      else if(p != _pair.end()) // pair
      {
        if(_pairs.find((*p).second) != _pairs.end())
          setWarning("%s specified more than once", argv[i]);
        if(i < argc - 1)
        {
          i++;
          _pairs[(*p).second] = std::string(argv[i]);
        }
        else
          setError("%s must precede a value", argv[i]);
      }
      else // value
      {
        if(values < _maxValues)
        {
          values++;
          _values.push_back(std::string(argv[i]));
        }
        else
          setError("unrecognized parameter '%s'", argv[i]);
      }
    }

    if(values < _minValues)
      setError("insufficient number of parameters given");
  }

  bool CliArgs::flag(int id)
  {
    return (_flags.find(id) != _flags.end());
  }

  std::string CliArgs::pair(int id)
  {
    if(_pairs.find(id) != _pairs.end())
      return _pairs[id];
    else
      return std::string("");
  }
  
  std::string CliArgs::popValue()
  {
    if(_values.size() > 0)
    {
      std::string s = _values.front();
      _values.pop_front();
      return s;
    }
    else
      return std::string("");
  }

/** private *******************************************************************/

  void CliArgs::clear()
  {
    _flags.clear();
    _pairs.clear();
    _values.clear();
    _msg.clear();
    
    _error = false; 
    
    std::map<int, std::string>::iterator i = _pairDef.begin();
    for(; i != _pairDef.end(); i++)
      _pairs[(*i).first] = (*i).second;
  }
  
  void CliArgs::setError(const char str[], ...)
  {
    va_list vl;
    va_start(vl, str);
    char buf[512];
    vsnprintf(buf, 512, str, vl);
    _msg.push_back(std::pair<std::string, bool>(std::string(buf), true));
    _error = true;
    va_end(vl);
  }

  void CliArgs::setWarning(const char str[], ...)
  {
    va_list vl;
    va_start(vl, str);
    char buf[512];
    vsnprintf(buf, 512, str, vl);
    _msg.push_back(std::pair<std::string, bool>(std::string(buf), false));
    va_end(vl);
  }
  
  std::pair<std::string, bool> CliArgs::popMsg()
  {
    std::pair<std::string, bool> p = _msg.front();
    _msg.pop_front();
    return p;
  }

} // namespaces

