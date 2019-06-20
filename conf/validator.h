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

#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "../data/schema.h"

namespace conf {

class Validator
{

  /** Config Validation Class
  tasks:
    validates data in config **/

public:
  Validator(data::Schema *conf) : _conf(conf) {};
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
  bool validateTableAttributes(data::Table*, const char*);
  bool validateTableColumnCount(data::Table*, const char*);
  bool validateTableColumnNames(data::Table*, const char*);
  bool validateTableColumns(data::Table*, const char*);
  bool validateTableFuncdeps(data::Table*, const char*);
  bool validateTableForeignKeys(data::Table*, const char*);
  bool revalidateTableFuncdeps(data::Table*, const char*);
  bool validateTableHarden(data::Table*, const char*);
  bool validateTableCondIncDep(data::Table*, const char*);

  void explodeFuncdeps(data::Table*);
  bool validateFuncdeps(data::Table*, const char*);

  /** column level validation **/
  bool validateColumnLength(data::Column*, const char*, const char*);
  bool validateColumnDatatype(data::Column*, const char*, const char*);
  bool validateColumnKey(data::Column*, data::Table*, const char*, const char*);
  bool validateColumnBasevalue(data::Column*, const char*, const char*);
  void validateColumnUniqueCount(data::Column*, data::Table*);

  /** functional dependency level validation **/
  bool validateFuncdepColumns(data::Funcdep*, data::Table*, const char*);

  /** conditional inclusion dependency level validation **/
  bool validateCondIncDepAttributes(data::Table*, const char*);
  bool validateCondIncDepColumns(data::Table*, const char*);
  bool validateCondIncDepConditions(data::Table*, const char*);
  bool validateCondIncDepLogic(data::Table*, const char*);


  data::Schema* _conf;

};

} // namespaces

#endif

