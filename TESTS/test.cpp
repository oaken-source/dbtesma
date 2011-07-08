#include "test.h"

namespace TESTS {

/** public ******************************************************************************************************/

	void Test::run()
	{
		registerTests();
		
		HELPER::UiHelper::printlnspaced(_className.c_str(), 5);
		std::vector<std::pair<bool (Test::*)(), std::string> >::iterator i = _testCases.begin();
		for(;i != _testCases.end(); i++)
		{
			HELPER::UiHelper::startProgress((*i).second.c_str());
			if((*this.*(*i).first)())
				HELPER::UiHelper::overrok();
			else
				HELPER::UiHelper::overrfail();
		}
	}

/** protected ***************************************************************************************************/


} // namespaces
