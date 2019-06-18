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

#ifndef TEST_H
#define TEST_H

#define REGISTER_TESTCASE(FUNC, NAME) _testCases.push_back(std::pair<bool(Test::*)(), std::string>((bool (tests::Test::*)())FUNC, std::string(NAME)))

#include "../helper/uihelper.h"

#include <vector>

namespace tests {

class Test
{

	/**
	  base class for test classes
	  tasks:
		- provides basic testing functionality
	**/

public:
	Test() : _className(std::string()), _testCases(std::vector<std::pair<bool(Test::*)(), std::string> >()) {};
	virtual ~Test() {};

	void run();

protected:
	virtual void registerTests() = 0;

	std::string _className;
	std::vector<std::pair<bool(Test::*)(), std::string> > _testCases;

};

} // namespaces

#endif // TEST_H
