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
	helper::Ui::printTime();

	helper::Ui::println(" DBTesMa data generator testing suite");
	helper::Ui::println(" - starting up...");

	/** prepare test vector **/
	std::vector<tests::Test*> tests = std::vector<tests::Test*>();

	/** register testclasses **/
	tests.push_back(new tests::StringHelperTest());

	helper::Ui::printok();

	helper::Ui::println(" - running tests...");

	/** run tests **/
	std::vector<tests::Test*>::iterator i = tests.begin();
	for(;i != tests.end(); ++i)
		(*i)->run();

	/** shut down **/
	helper::Ui::println(" - shutting down...");
	helper::Ui::printok();
	helper::Ui::println(" DBTesMa tests finished");

	/** finished **/
	helper::Ui::printTime();

	return 0;
}

