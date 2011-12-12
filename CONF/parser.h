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

#ifndef PARSER_H
#define PARSER_H

#include "validator.h"

namespace CONF {

class Parser
{

  /** Config Parser Class
  tasks:
    reads input from tesmafile **/

  /** enumeration containing the possible parsing contexts **/
  enum e_Context
  {
    C_None,
    C_Table,
    C_Column,
    C_Funcdep,
    C_CondIncDep
  };


public:

  Parser(std::string &in, DATA::Schema *conf) : _conf(conf), _filename(in),
    _in(new std::ifstream(in.c_str())), _context(C_None),
    _processContext(std::map<e_Context, bool (Parser::*)(std::string&)>())
  {
    _processContext[C_None] = &Parser::processLineContextNone;
    _processContext[C_Table] = &Parser::processLineContextTable;
    _processContext[C_Column] = &Parser::processLineContextColumn;
    _processContext[C_Funcdep] = &Parser::processLineContextFuncDep;
    _processContext[C_CondIncDep] = &Parser::processLineContextCondIncDep;
  };
  Parser(const Parser &obj) : _conf(obj._conf), _filename(obj._filename),
    _in(obj._in), _context(obj._context),
    _processContext(obj._processContext) {};
  Parser& operator=(const Parser&);
  ~Parser();

  /** main parsing method - starts parsing, and afterward starts validation **/
  bool parseAndValidate();

private:

  /** top level parsing **/
  bool parse();

  /** line-scoped parsing - dispatches to context line-scoped methods **/
  bool processLine(std::string&);

  /** contexted line-scoped parsing methods **/
  bool processLineContextNone(std::string&);
  bool processLineContextTable(std::string&);
  bool processLineContextColumn(std::string&);
  bool processLineContextFuncDep(std::string&);
  bool processLineContextCondIncDep(std::string&);


  DATA::Schema *_conf;
  std::string _filename;
  std::ifstream *_in;
  e_Context _context;

  std::map<e_Context, bool (Parser::*)(std::string&)> _processContext;
};

} // namespaces

#endif

