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
 
#include "osabstractionhelper.h"

namespace HELPER {

/** public ******************************************************************************************************/

#ifdef _WIN32

	void OsAbstractionHelper::setColorRed()
	{
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(h, FOREGROUND_RED);
	}
	
	void OsAbstractionHelper::setColorGreen()
	{
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); 
		SetConsoleTextAttribute(h, FOREGROUND_GREEN);
	}
	
	void OsAbstractionHelper::setColorYellow()
	{
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); 
		SetConsoleTextAttribute(h, (FOREGROUND_RED | FOREGROUND_GREEN));
	}
	
	void OsAbstractionHelper::unsetColor()
	{
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);		
		SetConsoleTextAttribute(h, (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN));
	}
	
	void OsAbstractionHelper::makeAbsolute(std::string &in)
	{
		std::string path = getCurrentWorkingDirectory();
		path += "\\";
		path += in;
		
		std::string::size_type position = path.find("\\");
		while (position != path.npos) 
		{
			path.replace(position, 1, "\\\\");
			position = path.find("\\", position + 2);
		} 
		
		in = path;
	}

#else

	void OsAbstractionHelper::setColorRed()
	{
		std::cout << "\033[1;" << 31 << "m";
	}
	
	void OsAbstractionHelper::setColorGreen()
	{
		std::cout << "\033[1;" << 32 << "m";
	}
	
	void OsAbstractionHelper::setColorYellow()
	{
		std::cout << "\033[1;" << 33 << "m";
	}
	
	void OsAbstractionHelper::unsetColor()
	{
		std::cout << "\033[0m";
	}

	void OsAbstractionHelper::makeAbsolute(std::string &in)
	{
		std::string path = getCurrentWorkingDirectory();
		path += "/";
		path += in;
		in = path;
	}
	
#endif
	
/** public ******************************************************************************************************/

#ifdef _WIN32

	std::string OsAbstractionHelper::getCurrentWorkingDirectory()
	{
		char buffer[512];
		GetCurrentDirectory(512, buffer);
		return std::string(buffer);
	}

#else

	std::string OsAbstractionHelper::getCurrentWorkingDirectory()
	{
		char buffer[512];
		return std::string(getcwd(buffer, 512));
	}

#endif
	
} // namespaces













