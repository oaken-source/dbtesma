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

/** public ******************************************************************************************************/

  Parser& Parser::operator=(const Parser &rhs)
  {
    _filename = rhs._filename;
    _in = rhs._in;
    _context = rhs._context;
  
    return *this;
  }

  Parser::~Parser() 
  {
    _in->close();
  }

  bool Parser::parseAndValidate(DATA::Schema *config)
  {
    CONF::Validator *cv = new CONF::Validator();

    bool good = false;
    while(1)
    {
      if(!parse(config))
        break;
      if(!cv->validate(config))
        break;

      good = true;
  
      break;
    }

    return good;  
  }
  
/** private *****************************************************************************************************/

  bool Parser::parse(DATA::Schema *conf)
  {
    _context = C_None;
    std::string line;
    unsigned int lineNo = 0;    

    bool good = true;

    while (_in->good() && good)
    {
      lineNo++;
      getline((*_in),line);
      good = processLine(line, conf);
    }

    if(!good)
    {
      std::stringstream sst;
      sst << "invalid syntax at line " << lineNo << " near '" << line << "'";
      std::string st = sst.str();
      conf->setErrorString(st);
    }

    return good;
  }

  bool Parser::processLine(std::string &in, DATA::Schema *conf)
  {
    HELPER::Strings::stripComments(in);
  
    HELPER::Strings::trim(in);

    if(!HELPER::Strings::empty(in))
    {
      switch(_context)
      {
        case C_None:
          return processLineContextNone(in, conf);
        case C_Table:
          return processLineContextTable(in, conf);
        case C_Column:
          return processLineContextColumn(in, conf);
        case C_Funcdep:
          return processLineContextFuncDep(in, conf);
      }
    }
    return true;
  }

  bool Parser::processLineContextNone(std::string &in, DATA::Schema *conf)
  {    
    // things that can happen here:
    //   - new table expression
    std::string key;

    key = "table";
    if(HELPER::Strings::stripleft(in, key))
    {
      key = "=";
      if(HELPER::Strings::stripleft(in, key))
      {
        key = "{";
        if(HELPER::Strings::stripleft(in, key))
        {
          if(HELPER::Strings::empty(in))
          {
            conf->newTable();
            _context = C_Table;
            return true;
          }
        }
      }
    }

    return false;
  }

  bool Parser::processLineContextTable(std::string &in, DATA::Schema *conf)
  {
    // things that can happen here:
    //   - name attribute
    //   - rows attribute
    //   - new column expression
    //   - new founctional dependency expression
    //   - end table expression 
    std::string key, key_name, key_rows;
    DATA::Table::ATTRIBUTES type = DATA::Table::ATTR_NONE;

    key_name = "name";
    key_rows = "rows";

    if(HELPER::Strings::stripleft(in, key_name))
      type = DATA::Table::ATTR_NAME;
    else if(HELPER::Strings::stripleft(in, key_rows))
      type = DATA::Table::ATTR_ROWS;
  
    if(type != DATA::Table::ATTR_NONE)
    {
      key = "=";
      if(HELPER::Strings::stripleft(in, key))
      {
        std::string value;
        if(HELPER::Strings::popQuotedValue(in, value))
        {
          if(HELPER::Strings::empty(in))
          {
            conf->setTableAttribute(type, value);
            return true;
          }
        }
      }
    }

    key = "column";
    if(HELPER::Strings::stripleft(in, key))
    {
      key = "=";
      if(HELPER::Strings::stripleft(in, key))
      {
        key = "{";
        if(HELPER::Strings::stripleft(in, key))
        {
          if(HELPER::Strings::empty(in))
          {
            conf->newColumn();
            _context = C_Column;
            return true;
          }
        }
      }
    }
    
    key = "functional_dep";
    if(HELPER::Strings::stripleft(in, key))
    {
      key = "=";
      if(HELPER::Strings::stripleft(in, key))
      {
        key = "{";
        if(HELPER::Strings::stripleft(in, key))
        {
          if(HELPER::Strings::empty(in))
          {
            conf->newFuncdep();
            _context = C_Funcdep;
            return true;
          }
        }
      }
    }

    key = "}";
    if(HELPER::Strings::stripleft(in, key))
    {
      if(HELPER::Strings::empty(in))
      {
        _context = C_None;
        return true;
      }
    }

    return false;
  }

  bool Parser::processLineContextColumn(std::string &in, DATA::Schema *conf)
  {
    // things that can happen here:
    //   - name attribute
    //   - datatype attribute
    //   - length attribute
    //   - key attribute
    //   - basevalue attribute
    //   - unique attribute
    //   - foreignkey attribute
    //   - end column expression
    std::string key, key_name, key_datatype, key_length, key_key, key_basevalue, key_key_group, key_unique, key_foreignkey;
    DATA::Column::ATTRIBUTES type = DATA::Column::ATTR_NONE;

    key_name = "name";
    key_datatype = "datatype";
    key_length = "length";
    key_key_group="key_group";
    key_key = "key";
    key_basevalue = "basevalue";
    key_unique = "unique";
    key_foreignkey = "foreignkey";

    if(HELPER::Strings::stripleft(in, key_name))
      type = DATA::Column::ATTR_NAME;
    else if(HELPER::Strings::stripleft(in, key_datatype))
      type = DATA::Column::ATTR_DATATYPE;
    else if(HELPER::Strings::stripleft(in, key_length))
      type = DATA::Column::ATTR_LENGTH;
    else if(HELPER::Strings::stripleft(in, key_key_group))
      type = DATA::Column::ATTR_KEY_GROUP;
    else if(HELPER::Strings::stripleft(in, key_key))
      type = DATA::Column::ATTR_KEY;
    else if(HELPER::Strings::stripleft(in, key_basevalue))
      type = DATA::Column::ATTR_BASEVALUE;
    else if(HELPER::Strings::stripleft(in, key_unique))
      type = DATA::Column::ATTR_UNIQUE;
    else if(HELPER::Strings::stripleft(in, key_foreignkey))
      type = DATA::Column::ATTR_FOREIGNKEY;

    if(type != DATA::Column::ATTR_NONE)
    {
      key = "=";
      if(HELPER::Strings::stripleft(in, key))
      {
        std::string value;
        if(HELPER::Strings::popQuotedValue(in, value))
        {
          if(HELPER::Strings::empty(in))
          {
            conf->setColumnAttribute(type, value);
            return true;
          }
        }
      }
    }

    key = "}";
    if(HELPER::Strings::stripleft(in, key))
    {
      if(HELPER::Strings::empty(in))
      {
        _context = C_Table;
        return true;
      }
    }

    return false;
  }
  
  bool Parser::processLineContextFuncDep(std::string &in, DATA::Schema *conf)
  {
    // things that can happen here:
    //   - lhs attribute
    //   - rhs attribute

    std::string key;

    key = "lhs";
    
    if(HELPER::Strings::stripleft(in, key))
    {
      key = "=";
      if(HELPER::Strings::stripleft(in, key))
      {
        std::string value;
        if(HELPER::Strings::popQuotedValue(in, value))
        {
          if(HELPER::Strings::empty(in))
          {
            conf->setFuncdepLhs(value);
            return true;
          }
        }
      }
    }
    
    key = "rhs";
    
    if(HELPER::Strings::stripleft(in, key))
    {
      key = "=";
      if(HELPER::Strings::stripleft(in, key))
      {
        std::string value;
        if(HELPER::Strings::popQuotedValue(in, value))
        {
          if(HELPER::Strings::empty(in))
          {
            conf->setFuncdepRhs(value);
            return true;
          }
        }
      }
    }

    key = "}";
    if(HELPER::Strings::stripleft(in, key))
    {
      if(HELPER::Strings::empty(in))
      {
        _context = C_Table;
        return true;
      }
    }

    return false;
  }

} // namespaces

