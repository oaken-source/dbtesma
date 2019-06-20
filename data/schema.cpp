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

#include "schema.h"

#include <cstdarg>

namespace data {

/** public ********************************************************************/

  void Schema::setError(const char f[], ...)
  {
    va_list vl;
    va_start(vl, f);
    char buf[512];
    vsnprintf(buf, 512, f, vl);
    _error = std::string(buf);
    va_end(vl);
  }

  void Schema::dumpToStdout()
  {
    std::cout << "schema dump:" << std::endl;
    std::vector<data::Table*>::iterator i;
    for(i = _tables.begin(); i != _tables.end(); i++)
    {
      std::string tableName;
      (*i)->getAttribute(data::Table::A_Name, tableName);
      std::cout << " table: " << tableName << " - " << (*i)->getRowCount() << " rows" << std::endl;
      std::vector<data::Column*>::iterator k;
      for(k = (*i)->columns_begin(); k != (*i)->columns_end(); k++)
      {
        std::string columnName, columnGroup, basevalue;
        (*k)->getAttribute(data::Column::A_KeyGroup, columnGroup);
        (*k)->getAttribute(data::Column::A_Name, columnName);
        (*k)->getBasevalue(basevalue);
        std::cout << "  column: " << columnName << " - " << (((*k)->getDatatype() == data::Column::D_Int)?"D_Int":"D_Varchar") << "(" << (*k)->getLength() << ") - [" << (*k)->getDex() << "][" << (*k)->getSiz() << "] - [" << (*k)->getUniqueValueCount() << "]  (" << basevalue << ")" << std::endl;
      }
      std::vector<data::Funcdep*>::iterator v;
      for(v = (*i)->funcdeps_begin(); v != (*i)->funcdeps_end(); v++)
      {
        std::vector<data::Column*>::iterator z = (*v)->lhs_begin();
        std::string columnName;
        (*z)->getAttribute(data::Column::A_Name, columnName);
        std::cout << "  fd: " << columnName;
        z++;
        for(; z != (*v)->lhs_end(); z++)
        {
          (*z)->getAttribute(data::Column::A_Name, columnName);
          std::cout << "," << columnName;
        }
        std::cout << " --> ";
        z = (*v)->rhs_begin();
        (*z)->getAttribute(data::Column::A_Name, columnName);
        std::cout << columnName;
        z++;
        for(; z != (*v)->rhs_end(); z++)
        {
          (*z)->getAttribute(data::Column::A_Name, columnName);
          std::cout << "," << columnName;
        }
        std::cout << (((*v)->isReverseFD())?" (reversed)":"");
        std::cout << std::endl;
      }
    }
  }

  void Schema::buildSchema()
  {
    std::vector<data::Table*>::iterator i;
    for(i = _tables.begin(); i != _tables.end(); i++)
    {
      std::string tableName;
      (*i)->getAttribute(data::Table::A_Name, tableName);

      std::cout << "drop table if exists " << tableName << ";\ncreate table " << tableName << " (";

      std::vector<data::Column*>::iterator k, kLast;
      for(k = (*i)->columns_begin(), kLast = (*i)->columns_end(), kLast--; k != (*i)->columns_end(); k++)
      {
        std::string columnName;
        (*k)->getAttribute(data::Column::A_Name, columnName);

        std::cout << columnName << " ";

        data::Column::e_Datatypes type = (*k)->getDatatype();
        unsigned int length = (*k)->getLength();

        switch(type)
        {
        case data::Column::D_Int:
          std::cout << ((length > 9)?"big":"") << "int(" << length << ")";
          break;
        case data::Column::D_Char:
          std::cout << "char(" << length << ")";
          break;
        case data::Column::D_Varchar:
          std::cout << "varchar(" << length << ")";
          break;
        }

        if(k != kLast)
          std::cout << ", ";
      }

      std::cout << ");" << std::endl;

      std::string pathName = tableName;
      pathName += ".csv";
      helper::File::makeAbsolute(pathName);

      std::cout << "load data infile \"" << pathName << "\" into table " << tableName << " fields terminated by ',';" << std::endl;
    }
  }

  void Schema::buildSchemaWithoutDatatypes()
  {
    std::vector<data::Table*>::iterator i;
    for(i = _tables.begin(); i != _tables.end(); i++)
    {
      std::string tableName;
      (*i)->getAttribute(data::Table::A_Name, tableName);

      std::cout << "drop table if exists " << tableName << ";\ncreate table " << tableName << " (";

      std::vector<data::Column*>::iterator k, kLast;
      for(k = (*i)->columns_begin(), kLast = (*i)->columns_end(), kLast--; k != (*i)->columns_end(); k++)
      {
        std::string columnName;
        (*k)->getAttribute(data::Column::A_Name, columnName);

        std::cout << columnName << " " << "varchar(" << (((*k)->getLength() < 255)?255:(*k)->getLength()) << ")";

        if(k != kLast)
          std::cout << ", ";
      }

      std::cout << ");" << std::endl;

      std::string pathName = tableName;
      pathName += ".csv";
      helper::File::makeAbsolute(pathName);

      std::cout << "load data infile \"" << pathName << "\" into table " << tableName << " fields terminated by ',';" << std::endl;
    }
  }

  void Schema::buildSchemaAsJSON()
  {
    std::cout << "{\"tables\":[";

    std::vector<data::Table*>::iterator i, iLast;
    for(i = _tables.begin(), iLast = _tables.end(), iLast--; i != _tables.end(); i++)
    {
      std::string tableName;
      (*i)->getAttribute(data::Table::A_Name, tableName);

      std::cout << "{\"name\":\"" << tableName << "\", \"rowNumbers\":" << (*i)->getRowCount() << ", \"columnModels\":[";

      std::vector<data::Column*>::iterator k, kLast;
      for(k = (*i)->columns_begin(), kLast = (*i)->columns_end(), kLast--; k != (*i)->columns_end(); k++)
      {
        std::string columnName;
        (*k)->getAttribute(data::Column::A_Name, columnName);

        unsigned int length = (*k)->getLength();

        std::cout << "{\"name\":\"" << columnName << "\", \"distributionMode\":\"\", \"cellSize\":" << length << ", \"dataType\":\"";

        data::Column::e_Datatypes type = (*k)->getDatatype();

        switch(type)
        {
        case data::Column::D_Int:
          std::cout << ((length > 9)?"BIG":"") << "D_Int";
          break;
        case data::Column::D_Char:
          std::cout << "D_Char";
          break;
        case data::Column::D_Varchar:
          std::cout << "D_Varchar";
          break;
        }

        std::cout << "\"}";

        if(k != kLast)
          std::cout << ", ";
      }

      std::cout << "]}";
      if(i != iLast)
        std::cout << ", ";
    }
    std::cout << "], \"InclusionDependencies\":[]}" << std::endl;
  }

  data::Table* Schema::findTableByName(std::string &in)
  {
    std::vector<data::Table*>::iterator i;
    std::string tableName;
    for(i = _tables.begin(); i != _tables.end(); i++)
    {
      (*i)->getAttribute(data::Table::A_Name, tableName);
      if(!tableName.compare(in))
        return (*i);
    }

    return NULL;
  }

} // namespaces
