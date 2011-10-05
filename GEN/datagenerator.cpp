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
 
#include "datagenerator.h"

namespace GEN {

/** public ******************************************************************************************************/

	DataGenerator& DataGenerator::operator=(const DataGenerator &rhs)
	{
		_conf = rhs._conf;
		return *this;
	}

	DataGenerator::~DataGenerator()
	{

	}

	void DataGenerator::processTables(bool noHeader)
	{
		std::vector<DATA::Table*>::iterator i;
		for(i = _conf->begin(); i != _conf->end(); i++)
		{
			std::string name;
			(*i)->getAttribute(DATA::Table::ATTR_NAME, name);
			HELPER::Ui::startProgress(name.c_str());
			(*i)->print(noHeader, _conf->hasHardenedFds());
			HELPER::Ui::overrok();			
		}
	}

/** private *****************************************************************************************************/

} // namespaces
