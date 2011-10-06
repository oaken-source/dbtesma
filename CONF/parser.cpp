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

namespace CONF {

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
    CONF::Validator *cv = new CONF::Validator(_conf);

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
    HELPER::Strings::stripComments(in);
    HELPER::Strings::trim(in);

    if(!HELPER::Strings::empty(in))
      return (this->*(_processContext[_context]))(in);
    else
      return true;
  }

  bool Parser::processLineContextNone(std::string &in)
  {    
    /** new Table expression **/
    if(HELPER::Strings::stripleft(in, "table")
      && HELPER::Strings::stripleft(in, "=")
      && HELPER::Strings::stripleft(in, "{"))
    {
      _conf->newTable();
      _context = C_Table;
      if(!HELPER::Strings::empty(in))
        return processLineContextTable(in);
      else
        return true;
    }
    return false;
  }

  bool Parser::processLineContextTable(std::string &in)
  {
    /** attribute expression **/
    DATA::Table::ATTRIBUTES type = DATA::Table::ATTR_NONE;
    if(HELPER::Strings::stripleft(in, "name"))
      type = DATA::Table::ATTR_NAME;
    else if(HELPER::Strings::stripleft(in, "rows"))
      type = DATA::Table::ATTR_ROWS;
  
    std::string value;
  
    if(type != DATA::Table::ATTR_NONE
      && HELPER::Strings::stripleft(in, "=")
      && HELPER::Strings::popQuotedValue(in, value))
    {
      _conf->setTableAttribute(type, value);
      if(!HELPER::Strings::empty(in))
        return processLineContextTable(in);
      else
        return true;
    }

    /** new Column expression **/
    if(HELPER::Strings::stripleft(in, "column")
      && HELPER::Strings::stripleft(in, "=")
      && HELPER::Strings::stripleft(in, "{"))
    {
      _conf->newColumn();
      _context = C_Column;
      if(!HELPER::Strings::empty(in))
        processLineContextColumn(in);      
      else
        return true;
    }
    
    /** new Functional Dependency expression **/
    if(HELPER::Strings::stripleft(in, "functional_dep")
      && HELPER::Strings::stripleft(in, "=")
      && HELPER::Strings::stripleft(in, "{"))
    {
      _conf->newFuncdep();
      _context = C_Funcdep;
      if(!HELPER::Strings::empty(in))
        processLineContextFuncDep(in);
      else
        return true;
    }

    /** end Table expression **/
    if(HELPER::Strings::stripleft(in, "}"))
    {
      _context = C_None;    
      if(!HELPER::Strings::empty(in))
        processLineContextNone(in);
      else
        return true;
    }

    return false;
  }

  bool Parser::processLineContextColumn(std::string &in)
  {
    /** attribute expression **/
    DATA::Column::ATTRIBUTES type = DATA::Column::ATTR_NONE;
    if(HELPER::Strings::stripleft(in, "name"))
      type = DATA::Column::ATTR_NAME;
    else if(HELPER::Strings::stripleft(in, "datatype"))
      type = DATA::Column::ATTR_DATATYPE;
    else if(HELPER::Strings::stripleft(in, "length"))
      type = DATA::Column::ATTR_LENGTH;
    else if(HELPER::Strings::stripleft(in, "key_group"))
      type = DATA::Column::ATTR_KEY_GROUP;
    else if(HELPER::Strings::stripleft(in, "key"))
      type = DATA::Column::ATTR_KEY;
    else if(HELPER::Strings::stripleft(in, "basevalue"))
      type = DATA::Column::ATTR_BASEVALUE;
    else if(HELPER::Strings::stripleft(in, "unique"))
      type = DATA::Column::ATTR_UNIQUE;
    else if(HELPER::Strings::stripleft(in, "foreignkey"))
      type = DATA::Column::ATTR_FOREIGNKEY;

    std::string value;

    if(type != DATA::Column::ATTR_NONE
      && HELPER::Strings::stripleft(in, "=")
      && HELPER::Strings::popQuotedValue(in, value))
    {
      _conf->setColumnAttribute(type, value);
      if(!HELPER::Strings::empty(in))
        processLineContextColumn(in);
      else
        return true;
    }

    /** end Column expression **/
    if(HELPER::Strings::stripleft(in, "}"))
    {
      _context = C_Table;
      if(!HELPER::Strings::empty(in))
        processLineContextTable(in);
      else
        return true;
    }

    return false;
  }
  
  bool Parser::processLineContextFuncDep(std::string &in)
  {
    /** attribute expression **/
    std::string value;
    
    if(HELPER::Strings::stripleft(in, "lhs")
      && HELPER::Strings::stripleft(in, "=")
      && HELPER::Strings::popQuotedValue(in, value))
    {
      _conf->setFuncdepLhs(value);
      if(!HELPER::Strings::empty(in))
        processLineContextFuncDep(in);
      else
        return true;
    }
    
    if(HELPER::Strings::stripleft(in, "rhs")
      && HELPER::Strings::stripleft(in, "=")
      && HELPER::Strings::popQuotedValue(in, value))
    {
      _conf->setFuncdepRhs(value);
      if(!HELPER::Strings::empty(in))
        processLineContextFuncDep(in);
      else
        return true;
    }

    /** end Functioal Dependency expression **/ 
    if(HELPER::Strings::stripleft(in, "}"))
    {
      _context = C_Table;      
      if(!HELPER::Strings::empty(in))
        processLineContextTable(in);
      else
        return true;
    }

    return false;
  }

} // namespaces

