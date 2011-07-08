#ifndef TEST_H
#define TEST_H

#define REGISTER_TESTCASE(FUNC, NAME) _testCases.push_back(std::pair<bool(Test::*)(), std::string>((bool (TESTS::Test::*)())FUNC, std::string(NAME)))

#include "../HELPER/uihelper.h"

#include <vector>

namespace TESTS {

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
