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

#ifndef STRINGHELPERTEST_H
#define STRINGHELPERTEST_H

#include "test.h"

namespace TESTS {

class StringHelperTest : public Test
{

	/**
	  base class for test classes
	  tasks:
		- provides basic testing functionality
	**/

public:
	StringHelperTest() { _className = "HELPER::StringHelper"; };
	~StringHelperTest() {};

private:
	void registerTests();

	bool test_trim();

};

} // namespaces

#endif // STRINGHELPERTEST_H
