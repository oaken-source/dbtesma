/******************************************************************************
 *                          dbtesma data generator                             *
 *                                                                             *
 *    Copyright (C) 2011  Andreas Grapentin                                    *
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

#ifndef CLIARGS_H
#define CLIARGS_H

#include <map>
#include <vector>
#include <list>
#include <sstream>

namespace helper {

class CliArgs
{

  /** Command Line Arguments helper class
    tasks:
      provides functionality for parsing command line arguments
    parameter types:
      flags:  boolean values, e.g. '--version'
      pairs:  string/int values indicated by preceding flag e.g. '-o a.out'
      values: string/int values not preceded by a specified flag **/

public:

  CliArgs() : _flag(std::map<std::string, int>()),
    _pair(std::map<std::string, int>()), _minValues(), _maxValues(),
    _pairDef(std::map<int, std::string>()), _flags(std::map<int, bool>()),
    _pairs(std::map<int, std::string>()), _values(std::list<std::string>()),
    _error(false), _msg(std::list<std::pair<std::string, bool> >()) { };
  CliArgs(int min, int max) : _flag(std::map<std::string, int>()),
    _pair(std::map<std::string, int>()), _minValues(min), _maxValues(max),
    _pairDef(std::map<int, std::string>()), _flags(std::map<int, bool>()),
    _pairs(std::map<int, std::string>()), _values(std::list<std::string>()),
    _error(false), _msg(std::list<std::pair<std::string, bool> >()) { };
  ~CliArgs() { };

  /** add a parameter **/
  bool addFlag(std::string, int);
  bool addPair(std::string, int, std::string = std::string(""));

  /** start parsing **/
  void parse(int, char*[]);

  /** fetch information **/
  bool flag(int);
  std::string pair(int);
  std::string popValue();

  /** fetch error/warning states **/
  bool hasError() { return _error; }
  bool hasMsg() { return _msg.size(); }
  std::pair<std::string, bool> popMsg();

private:

  /** specific adding methods for flag and pair parameters **/
  void addFlagParam(std::string name, int id) { _flag[name] = id; }
  void addPairParam(std::string name, int id) { _pair[name] = id; }

  /** clear parsed information on self **/
  void clear();

  /** errors and warnings handling **/
  void setError(const char*, ...);
  void setWarning(const char*, ...);

  /** store parameter information **/
  std::map<std::string, int> _flag;
  std::map<std::string, int> _pair;

  /** limitations of value count **/
  unsigned int _minValues;
  unsigned int _maxValues;

  /** store default values for pairs **/
  std::map<int, std::string> _pairDef;

  /** store parsed information **/
  std::map<int, bool> _flags;
  std::map<int, std::string> _pairs;
  std::list<std::string> _values;

  /** store error state **/
  bool _error;
  std::list<std::pair<std::string, bool> > _msg;

};

} // namespace

#endif

