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
