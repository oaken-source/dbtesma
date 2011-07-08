#ifndef OSABSTRACTIONHELPER_H
#define OSABSTRACTIONHELPER_H

#include <string>

#ifdef _WIN32
	#include <windows.h>
#else
	#include <unistd.h>
	#include <iostream>
#endif

namespace HELPER {

class OsAbstractionHelper
{

	/**
	Operating Systems Abstraction helper class
	tasks:
		handles all the operating system dependent code, e.g. coloured output to stdout
	note:
		every method declared here has different implementations for windows/linux. see the .cpp file for details
	**/

public:

	/** stdout color handling **/
	static void setColorRed();
	static void setColorGreen();
	static void setColorYellow();
	static void unsetColor();
	
	/** makes a relative file path originating from the current working directory absolute **/ 
	static void makeAbsolute(std::string&);

private:

	/** get the current working directory **/
	static std::string getCurrentWorkingDirectory();
	
};

} // namespaces

#endif // OSABSTRACTIONHELPER_H
