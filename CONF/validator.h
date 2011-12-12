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

#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "../DATA/schema.h"

namespace CONF {

class Validator
{

  /** Config Validation Class
  tasks:
    validates data in config **/

public:
  Validator(DATA::Schema *conf) : _conf(conf) {};
  Validator(const Validator &obj) : _conf(obj._conf) {};
  Validator& operator=(const Validator&);
  ~Validator() {};

  /** top level validation - calls medium level validation methods **/
  bool validate();

private:

  /** top level validation **/
  bool validateTableCount();
  bool validateTableNames();
  bool validateTables();

  /** table level validation **/
  bool validateTableAttributes(DATA::Table*, const char*);
  bool validateTableColumnCount(DATA::Table*, const char*);
  bool validateTableColumnNames(DATA::Table*, const char*);
  bool validateTableColumns(DATA::Table*, const char*);
  bool validateTableFuncdeps(DATA::Table*, const char*);
  bool validateTableForeignKeys(DATA::Table*, const char*);
  bool revalidateTableFuncdeps(DATA::Table*, const char*);
  bool validateTableHarden(DATA::Table*, const char*);
  bool validateTableCondIncDep(DATA::Table*, const char*);

  void explodeFuncdeps(DATA::Table*);
  bool validateFuncdeps(DATA::Table*, const char*);

  /** column level validation **/
  bool validateColumnLength(DATA::Column*, const char*, const char*);
  bool validateColumnDatatype(DATA::Column*, const char*, const char*);
  bool validateColumnKey(DATA::Column*, DATA::Table*, const char*, const char*);
  bool validateColumnBasevalue(DATA::Column*, const char*, const char*);
  void validateColumnUniqueCount(DATA::Column*, DATA::Table*);

  /** functional dependency level validation **/
  bool validateFuncdepColumns(DATA::Funcdep*, DATA::Table*, const char*);

  /** conditional inclusion dependency level validation **/
  bool validateCondIncDepAttributes(DATA::Table*, const char*);
  bool validateCondIncDepColumns(DATA::Table*, const char*);
  bool validateCondIncDepConditions(DATA::Table*, const char*);
  bool validateCondIncDepLogic(DATA::Table*, const char*);


  DATA::Schema* _conf;

};

} // namespaces

#endif

