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

#include "stringhelpertest.h"

int main()
{
	/** entry point **/
	HELPER::UiHelper::printTime();

	HELPER::UiHelper::println(" DBTesMa data generator testing suite");
	HELPER::UiHelper::println(" - starting up...");

	/** prepare test vector **/
	std::vector<TESTS::Test*> tests = std::vector<TESTS::Test*>();

	/** register testclasses **/
	tests.push_back(new TESTS::StringHelperTest());

	HELPER::UiHelper::printok();

	HELPER::UiHelper::println(" - running tests...");

	/** run tests **/
	std::vector<TESTS::Test*>::iterator i = tests.begin();
	for(;i != tests.end(); ++i)
		(*i)->run();

	/** shut down **/
	HELPER::UiHelper::println(" - shutting down...");
	HELPER::UiHelper::printok();
	HELPER::UiHelper::println(" DBTesMa tests finished");

	/** finished **/
	HELPER::UiHelper::printTime();

	return 0;
}

