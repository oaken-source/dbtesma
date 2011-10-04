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
 
#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include "../DATA/schema.h"
#include "../HELPER/uihelper.h"

namespace GEN {

class DataGenerator
{

	/**
		main data generation class
		tasks:
			starts the data generation process for each table
	**/

public:

	DataGenerator(DATA::Schema *conf) : _conf(conf) { srand(time(NULL)); };
	DataGenerator(const DataGenerator &obj) : _conf(obj._conf) {};
	DataGenerator& operator=(const DataGenerator&);
	~DataGenerator();

	/** standard table processing **/
	void processTables(bool);
	/** offset-only table processing **/
	void processTablesOffsetsOnly(bool);

private:

	/** config **/ 
	DATA::Schema *_conf;
};

} // namespaces

#endif // DATAGENERATOR_H
