
#include "stringhelpertest.h"

namespace TESTS {

/** public ******************************************************************************************************/

/** private *****************************************************************************************************/

	void StringHelperTest::registerTests()
	{
		REGISTER_TESTCASE(&TESTS::StringHelperTest::test_trim, "testing trim() functions");
	}
	
	bool StringHelperTest::test_trim()
	{
		return true;
	}
		
} // namespaces
