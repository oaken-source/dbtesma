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
 
#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <string>
#include <fstream>

namespace HELPER {

class FileHelper
{

	/**
	Files helper class
	tasks:
		helper class for file management methods
	todo:
		Yes, it's a lazy class. More file functionality has to be outsourced in the future.
	**/

public:

	static bool isExistingFile(std::string&);
	static bool writeRaw(std::string&, const char[]);

private:

};

} // namespaces

#endif // FILEHELPER_H
