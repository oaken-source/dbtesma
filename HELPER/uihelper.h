#ifndef UIHELPER_H
#define UIHELPER_H

#include "../macros.h"
#include "osabstractionhelper.h"

#include <iostream>
#include <iomanip>
#include <time.h>

namespace HELPER {

class UiHelper
{

	/**
	User Interaction class
	tasks:
		handles command line output for convenience and continuity
	**/

public:

	static void println(const char[]);
	static void printlnspaced(const char[], unsigned int);
	static void overrln(const char[]);
	static void emptyln();
	static void printraw(const char[]);

	static void printerr(const char[]);
	static void printwrn(const char[]);
	static void printok();

	static void overrok();
	static void overrfail();

	static void printTime();
	
	static void startProgress(const char[]);

private:


};

} // namespaces

#endif // UIHELPER_H
