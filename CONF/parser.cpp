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
    DATA::Table::e_Attributes type = DATA::Table::A_None;
    if(HELPER::Strings::stripleft(in, "name"))
      type = DATA::Table::A_Name;
    else if(HELPER::Strings::stripleft(in, "rows"))
      type = DATA::Table::A_Rows;
  
    std::string value;
  
    if(type != DATA::Table::A_None
      && HELPER::Strings::stripleft(in, "=")
      && HELPER::Strings::popQuotedValue(in, value))
    {
      _conf->passTable()->setAttribute(type, value);
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
      _conf->passTable()->newColumn();
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
      _conf->passTable()->newFuncdep(false, false);
      _context = C_Funcdep;
      if(!HELPER::Strings::empty(in))
        processLineContextFuncDep(in);
      else
        return true;
    }

    /** new Conditional Inclusion Dependency expression **/
    if(HELPER::Strings::stripleft(in, "cond_inc_dep")
      && HELPER::Strings::stripleft(in, "=")
      && HELPER::Strings::stripleft(in, "{"))
    {
      if(!_conf->passTable()->startCIND())
        return false;
      _context = C_CondIncDep;
      if(!HELPER::Strings::empty(in))
        processLineContextCondIncDep(in);
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
    DATA::Column::e_Attributes type = DATA::Column::A_None;
    if(HELPER::Strings::stripleft(in, "name"))
      type = DATA::Column::A_Name;
    else if(HELPER::Strings::stripleft(in, "datatype"))
      type = DATA::Column::A_Datatype;
    else if(HELPER::Strings::stripleft(in, "length"))
      type = DATA::Column::A_Length;
    else if(HELPER::Strings::stripleft(in, "key_group"))
      type = DATA::Column::A_KeyGroup;
    else if(HELPER::Strings::stripleft(in, "key"))
      type = DATA::Column::A_Key;
    else if(HELPER::Strings::stripleft(in, "basevalue"))
      type = DATA::Column::A_Basevalue;
    else if(HELPER::Strings::stripleft(in, "unique"))
      type = DATA::Column::A_Unique;
    else if(HELPER::Strings::stripleft(in, "foreignkey"))
      type = DATA::Column::A_ForeignKey;

    std::string value;

    if(type != DATA::Column::A_None
      && HELPER::Strings::stripleft(in, "=")
      && HELPER::Strings::popQuotedValue(in, value))
    {
      _conf->passTable()->passColumn()->setAttribute(type, value);
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
      _conf->passTable()->passFuncdep()->setLhs(value);
      if(!HELPER::Strings::empty(in))
        processLineContextFuncDep(in);
      else
        return true;
    }
    
    if(HELPER::Strings::stripleft(in, "rhs")
      && HELPER::Strings::stripleft(in, "=")
      && HELPER::Strings::popQuotedValue(in, value))
    {
      _conf->passTable()->passFuncdep()->setRhs(value);
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

  bool Parser::processLineContextCondIncDep(std::string &in)
  {
/*    std::string value;
    
    if(HELPER::Strings::stripleft(in, "lhs")
      && HELPER::Strings::stripleft(in, "=")
      && HELPER::Strings::popQuotedValue(in, value))
    {
      _conf->setCondIncDepLhs(value);
      if(!HELPER::Strings::empty(in))
        processLineContextCondIncDep(in);
      else
        return true;
    }
    
    if(HELPER::Strings::stripleft(in, "rhs")
      && HELPER::Strings::stripleft(in, "=")
      && HELPER::Strings::popQuotedValue(in, value))
    {
      _conf->setCondIncDepRhs(value);
      if(!HELPER::Strings::empty(in))
        processLineContextCondIncDep(in);
      else
        return true;
    }*/

    /** end Conditional Inclusion Dependency expression **/ 
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

