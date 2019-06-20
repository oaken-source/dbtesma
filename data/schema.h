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

#ifndef SCHEMA_H
#define SCHEMA_H

#include "table.h"

#include <iostream>
#include "../helper/file.h"

namespace data {

class Schema
{

  /** Schema Data Class
  tasks:
    contains the top-level schema information **/

public:
  Schema() : _error(""), _tables(std::vector<data::Table*>()),
    _hardened_fds(false) {};
  ~Schema() { };

  void setError(const char[], ...);
  std::string getErrorString() { return _error; }

  void newTable() { _tables.push_back(new data::Table()); }
  data::Table* passTable() { return _tables.back(); }

  /** process **/
  void processTables(bool);
  void processTablesOffsetsOnly(bool);

  /** debug **/
  void dumpToStdout();

  /** schema scans **/
  void buildSchema();
  void buildSchemaWithoutDatatypes();
  void buildSchemaAsJSON();

  /** access **/
  std::vector<data::Table*>::size_type size() { return _tables.size(); }
  std::vector<data::Table*>::iterator begin() { return _tables.begin(); }
  std::vector<data::Table*>::iterator end() { return _tables.end(); }

  void setHardenFdFlag(bool flag) { _hardened_fds = flag; }
  bool hasHardenedFds() { return _hardened_fds; }

  data::Table* findTableByName(std::string&);

private:

  std::string _error;
  std::vector<data::Table*> _tables;

  bool _hardened_fds;

};

} // namespaces

#endif
