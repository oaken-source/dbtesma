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
 
#ifndef CLIARGSHELPER_H
#define CLIARGSHELPER_H

#include <map>
#include <vector>
#include <sstream>

namespace HELPER {

class CliArgsHelper
{

	/** 
	Command Line Arguments helper class
	tasks:
		provides functionality for parsing the command line arguments
	**/

public:

	/** enumeration containing the different types of parameters
			flags:	parameters representing a boolean value
			pairs:	parameters that require a value to be specified directly afterwards **/
	enum PARAM_TYPES
	{
		FLAG_PARAM,
		PAIR_PARAM
	} typedef PARAM_TYPES;

	/** enumeration containing the different types of constraints available on the arguments **/
	enum CONSTRAINTS
	{
		CONSTRAINT_NONE,
		CONSTRAINT_ISINT
	} typedef CONSTRAINTS;

	CliArgsHelper() : _flagParams(std::map<std::string, int>()), _pairParams(std::map<std::string, int>()), _paramConstraints(std::map<std::string, CliArgsHelper::CONSTRAINTS>()), _minValueParams(), _maxValueParams() { };
	CliArgsHelper(int min, int max) : _flagParams(std::map<std::string, int>()), _pairParams(std::map<std::string, int>()), _paramConstraints(std::map<std::string, CliArgsHelper::CONSTRAINTS>()), _minValueParams(min), _maxValueParams(max) { };
	~CliArgsHelper() { };

	/** adds a specified parameter to the parser **/
	bool addParam(std::string, int, CliArgsHelper::PARAM_TYPES, CliArgsHelper::CONSTRAINTS = CliArgsHelper::CONSTRAINT_NONE);

	/** specifies the minimum/maximum amount of parameter independent values **/
	void setMinValueParamCount(unsigned int);
	void setMaxValueParamCount(unsigned int);

	/** initiate the parsing process **/
	bool parse(std::map<int, bool>&, std::map<int, std::string>&, std::vector<std::string>&, int, char*[], std::string&);

private:

	/** specific adding methods for flag and pair parameters **/
	void addFlagParam(std::string, int, CliArgsHelper::CONSTRAINTS);
	void addPairParam(std::string, int, CliArgsHelper::CONSTRAINTS);

	/** check for a given constraint **/
	bool matchesConstraints(std::string, std::string);

	bool isInt(std::string);

	std::map<std::string, int> _flagParams;
	std::map<std::string, int> _pairParams;
	std::map<std::string, CliArgsHelper::CONSTRAINTS> _paramConstraints;

	unsigned int _minValueParams;
	unsigned int _maxValueParams;

};

} // namespace

#endif // CLIARGSHELPER_H
