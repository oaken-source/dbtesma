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

#include "parser.h"

namespace conf {

/** public ********************************************************************/

  Parser& Parser::operator=(const Parser &rhs)
  {
    _conf = rhs._conf;
    _filename = rhs._filename;
    _in = rhs._in;
    _context = rhs._context;

    return *this;
  }

  Parser::~Parser()
  {
    _in->close();
  }

  bool Parser::parseAndValidate()
  {
    conf::Validator *cv = new conf::Validator(_conf);
    return (parse() && cv->validate());
  }

/** private *******************************************************************/

  bool Parser::parse()
  {
    _context = C_None;
    std::string line;
    unsigned int lineNo = 0;

    while (_in->good())
    {
      lineNo++;
      getline((*_in),line);
      if(!processLine(line))
      {
        _conf->setError("%u: syntax error near '%s'", lineNo, line.c_str());
        return false;
      }
    }

    return true;
  }

  bool Parser::processLine(std::string &in)
  {
    helper::Strings::stripComments(in);
    helper::Strings::trim(in);

    if(!helper::Strings::empty(in))
      return (this->*(_processContext[_context]))(in);
    else
      return true;
  }

  bool Parser::processLineContextNone(std::string &in)
  {
    /** new Table expression **/
    if(helper::Strings::stripleft(in, "table")
      && helper::Strings::stripleft(in, "=")
      && helper::Strings::stripleft(in, "{"))
    {
      _conf->newTable();
      _context = C_Table;
      if(!helper::Strings::empty(in))
        return processLineContextTable(in);
      else
        return true;
    }
    return false;
  }

  bool Parser::processLineContextTable(std::string &in)
  {
    /** attribute expression **/
    data::Table::e_Attributes type = data::Table::A_None;
    if(helper::Strings::stripleft(in, "name"))
      type = data::Table::A_Name;
    else if(helper::Strings::stripleft(in, "rows"))
      type = data::Table::A_Rows;

    std::string value;

    if(type != data::Table::A_None
      && helper::Strings::stripleft(in, "=")
      && helper::Strings::popQuotedValue(in, value))
    {
      _conf->passTable()->setAttribute(type, value);
      if(!helper::Strings::empty(in))
        return processLineContextTable(in);
      else
        return true;
    }

    /** new Column expression **/
    if(helper::Strings::stripleft(in, "column")
      && helper::Strings::stripleft(in, "=")
      && helper::Strings::stripleft(in, "{"))
    {
      _conf->passTable()->newColumn();
      _context = C_Column;
      if(!helper::Strings::empty(in))
        processLineContextColumn(in);
      else
        return true;
    }

    /** new Functional Dependency expression **/
    if(helper::Strings::stripleft(in, "functional_dep")
      && helper::Strings::stripleft(in, "=")
      && helper::Strings::stripleft(in, "{"))
    {
      _conf->passTable()->newFuncdep(false, false);
      _context = C_Funcdep;
      if(!helper::Strings::empty(in))
        processLineContextFuncDep(in);
      else
        return true;
    }

    /** new Conditional Inclusion Dependency expression **/
    if(helper::Strings::stripleft(in, "cond_inc_dep")
      && helper::Strings::stripleft(in, "=")
      && helper::Strings::stripleft(in, "{"))
    {
      if(!_conf->passTable()->startCondIncDep())
        return false;
      _context = C_CondIncDep;
      if(!helper::Strings::empty(in))
        processLineContextCondIncDep(in);
      else
        return true;
    }

    /** end Table expression **/
    if(helper::Strings::stripleft(in, "}"))
    {
      _context = C_None;
      if(!helper::Strings::empty(in))
        processLineContextNone(in);
      else
        return true;
    }

    return false;
  }

  bool Parser::processLineContextColumn(std::string &in)
  {
    /** attribute expression **/
    data::Column::e_Attributes type = data::Column::A_None;
    if(helper::Strings::stripleft(in, "name"))
      type = data::Column::A_Name;
    else if(helper::Strings::stripleft(in, "datatype"))
      type = data::Column::A_Datatype;
    else if(helper::Strings::stripleft(in, "length"))
      type = data::Column::A_Length;
    else if(helper::Strings::stripleft(in, "key_group"))
      type = data::Column::A_KeyGroup;
    else if(helper::Strings::stripleft(in, "key"))
      type = data::Column::A_Key;
    else if(helper::Strings::stripleft(in, "basevalue"))
      type = data::Column::A_Basevalue;
    else if(helper::Strings::stripleft(in, "unique"))
      type = data::Column::A_Unique;
    else if(helper::Strings::stripleft(in, "foreignkey"))
      type = data::Column::A_ForeignKey;

    std::string value;

    if(type != data::Column::A_None
      && helper::Strings::stripleft(in, "=")
      && helper::Strings::popQuotedValue(in, value))
    {
      _conf->passTable()->passColumn()->setAttribute(type, value);
      if(!helper::Strings::empty(in))
        processLineContextColumn(in);
      else
        return true;
    }

    /** end Column expression **/
    if(helper::Strings::stripleft(in, "}"))
    {
      _context = C_Table;
      if(!helper::Strings::empty(in))
        processLineContextTable(in);
      else
        return true;
    }

    return false;
  }

  bool Parser::processLineContextFuncDep(std::string &in)
  {
    std::string value;

    if(helper::Strings::stripleft(in, "lhs")
      && helper::Strings::stripleft(in, "=")
      && helper::Strings::popQuotedValue(in, value))
    {
      _conf->passTable()->passFuncdep()->setLhs(value);
      if(!helper::Strings::empty(in))
        processLineContextFuncDep(in);
      else
        return true;
    }

    if(helper::Strings::stripleft(in, "rhs")
      && helper::Strings::stripleft(in, "=")
      && helper::Strings::popQuotedValue(in, value))
    {
      _conf->passTable()->passFuncdep()->setRhs(value);
      if(!helper::Strings::empty(in))
        processLineContextFuncDep(in);
      else
        return true;
    }

    /** end Conditional Inclusion Dependency expression **/
    if(helper::Strings::stripleft(in, "}"))
    {
      _context = C_Table;
      if(!helper::Strings::empty(in))
        processLineContextTable(in);
      else
        return true;
    }

    return false;
  }

  bool Parser::processLineContextCondIncDep(std::string &in)
  {
    std::string value;

    if(helper::Strings::stripleft(in, "completeness")
      && helper::Strings::stripleft(in, "=")
      && helper::Strings::popQuotedValue(in, value))
    {
      _conf->passTable()->passCondIncDep()->setCompleteness(value);
      if(!helper::Strings::empty(in))
        processLineContextFuncDep(in);
      else
        return true;
    }

    if(helper::Strings::stripleft(in, "rows")
      && helper::Strings::stripleft(in, "=")
      && helper::Strings::popQuotedValue(in, value))
    {
      _conf->passTable()->passCondIncDep()->setRows(value);
      if(!helper::Strings::empty(in))
        processLineContextFuncDep(in);
      else
        return true;
    }

    if(helper::Strings::stripleft(in, "conditions")
      && helper::Strings::stripleft(in, "=")
      && helper::Strings::popQuotedValue(in, value))
    {
      if(!value.empty())
        _conf->passTable()->passCondIncDep()->pushConditions(value);
      if(!helper::Strings::empty(in))
        processLineContextFuncDep(in);
      else
        return true;
    }

    if(helper::Strings::stripleft(in, "lhs")
      && helper::Strings::stripleft(in, "=")
      && helper::Strings::popQuotedValue(in, value))
    {
      _conf->passTable()->passCondIncDep()->setLhs(value);
      if(!helper::Strings::empty(in))
        processLineContextFuncDep(in);
      else
        return true;
    }

    if(helper::Strings::stripleft(in, "rhs")
      && helper::Strings::stripleft(in, "=")
      && helper::Strings::popQuotedValue(in, value))
    {
      _conf->passTable()->passCondIncDep()->setRhs(value);
      if(!helper::Strings::empty(in))
        processLineContextFuncDep(in);
      else
        return true;
    }

    /** end Functioal Dependency expression **/
    if(helper::Strings::stripleft(in, "}"))
    {
      _context = C_Table;
      if(!helper::Strings::empty(in))
        processLineContextTable(in);
      else
        return true;
    }

    return false;
  }

} // namespaces

