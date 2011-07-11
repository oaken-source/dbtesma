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
 
#ifndef CONFIGVALIDATOR_H
#define CONFIGVALIDATOR_H

#include "../DATA/config.h"

namespace CONF {

class Configvalidator
{

	/**
	Config Validation Class
	tasks:
		validates data in config
	todo:
		this class is a monster! tame it!
	**/

public:
	Configvalidator() : _conf(NULL) {};
	Configvalidator(const Configvalidator &obj) : _conf(obj._conf) {};
	Configvalidator& operator=(const Configvalidator&);
	~Configvalidator();

	/** top level validation - calls medium level validation methods **/
	bool validate(DATA::Config*);

private:

	/** table related validation **/
	bool validateTableCount();
	bool validateTableNames();
	bool validateTableAttributes();
	bool validateTableColumns();
	bool validateTableFuncdeps();

	/** column related validation **/
	bool validateColumnCount(DATA::Table*);
	bool validateColumnNames(DATA::Table*);
	bool validateColumnDatatypes(DATA::Table*);
	bool validateColumnSize(DATA::Table*);

	/** functional dependencies - first pass **/
	bool validateFuncdepColumns(DATA::Table*);
	void explodeFuncdeps(DATA::Table*);
	bool validateFuncdeps(DATA::Table*);
	
	/** functional dependencies - second pass **/
	bool revalidateFuncdeps();
	bool revalidateTableFuncdeps(DATA::Table*);
	
	/** datatype preparation **/
	void populateDatatypeWrappers(DATA::Table*);

	/** bottom-level column related validation **/
	bool validateColumnKey(DATA::Table*);
	bool validateColumnBasevalue(DATA::Table*);
	bool validateColumnUniqueCount(DATA::Table*);

	/** foreign key validation **/
	bool validateForeignKeyConstraints();
	bool validateForeignKeyConstraints(DATA::Table*);

	/** harden fd validation - more strict than basic fd validation **/
	bool validateHardenedFds();
	bool validateHardenedFds(DATA::Table*);
	
	DATA::Config* _conf;
	
};

} // namespaces

#endif // CONFIGVALIDATOR_H
