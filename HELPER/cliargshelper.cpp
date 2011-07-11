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
 
#include "cliargshelper.h"

namespace HELPER {

/** public ******************************************************************************************************************/

	bool CliArgsHelper::addParam(std::string name, int id, CliArgsHelper::PARAM_TYPES type, CliArgsHelper::CONSTRAINTS constraint)
	{
		bool good = false;
		if(_flagParams.find(name) == _flagParams.end() && _pairParams.find(name) == _pairParams.end())
		{
			good = true;
			switch(type)
			{
			case CliArgsHelper::FLAG_PARAM:
				addFlagParam(name, id, constraint);
				break;
			case CliArgsHelper::PAIR_PARAM:
				addPairParam(name, id, constraint);
				break;
			default:
				good = false;
				break;
			}
		}
		return good;
	}

	void CliArgsHelper::setMaxValueParamCount(unsigned int value)
	{
		_maxValueParams = value;
	}

	void CliArgsHelper::setMinValueParamCount(unsigned int value)
	{
		_minValueParams = value;
	}

	bool CliArgsHelper::parse(std::map<int, bool> &flags, std::map<int, std::string> &pairs, std::vector<std::string> &values, int argc, char *argv[], std::string &error)
	{
		bool good = true;
		unsigned int valueParams = 0;

		flags.clear();
		pairs.clear();
		values.clear();

		int x;
		for(x = 1; x < argc; x++)
		{
			std::string param(argv[x]);
			std::map<std::string, int>::iterator i = _flagParams.find(param);
			std::map<std::string, int>::iterator j = _pairParams.find(param);

			if(i != _flagParams.end())
			{
				// flag params
				flags[(*i).second] = true;

			}
			else if(j != _pairParams.end())
			{
				// pair param
				if(x < argc - 1)
				{
					x++;
					if(matchesConstraints((*j).first, std::string(argv[x])))
						pairs[(*j).second] = std::string(argv[x]);
					else
					{
						error = std::string("invalid value \"")+argv[x]+"\" for \""+argv[x-1]+"\"";
						good = false;
						break;
					}
				}
				else
				{
					error = std::string("\"")+argv[x]+"\" must be followed by a value";
					good = false;
					break;
				}
			}
			else
			{
				// value param
				if(valueParams < _maxValueParams)
				{
					valueParams++;
					values.push_back(std::string(argv[x]));
				}
				else
				{
					error = std::string("unrecognized parameter \"")+argv[x]+"\"";
					good = false;
					break;
				}
			}
		}

		if(good && (valueParams < _minValueParams))
		{
			error = "not enough parameters given";
			good = false;
		}

		return good;
	}

/** private *****************************************************************************************************************/

	void CliArgsHelper::addFlagParam(std::string name, int id, CliArgsHelper::CONSTRAINTS constraint)
	{
		_flagParams[name] = id;
		_paramConstraints[name] = constraint;
	}

	void CliArgsHelper::addPairParam(std::string name, int id, CliArgsHelper::CONSTRAINTS constraint)
	{
		_pairParams[name] = id;
		_paramConstraints[name] = constraint;
	}

	bool CliArgsHelper::matchesConstraints(std::string name, std::string value)
	{
		CliArgsHelper::CONSTRAINTS constraint = _paramConstraints.find(name)->second;
		bool good = true;
		switch(constraint)
		{
		case CliArgsHelper::CONSTRAINT_ISINT:
			good = isInt(value);
			break;
		case CliArgsHelper::CONSTRAINT_NONE:
			good = true;
			break;
		}
		return good;
	}

	bool CliArgsHelper::isInt(std::string value)
	{
		std::istringstream i(value);
		int x;
		return (i >> x);
	}

} // namespaces

