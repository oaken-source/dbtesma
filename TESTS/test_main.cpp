
#include "stringhelpertest.h"

int main()
{
	/** entry point **/
	HELPER::UiHelper::printTime();

	HELPER::UiHelper::println(" DBTesMa data generator testing suite");
	HELPER::UiHelper::println(" - starting up...");

	/** prepare test vector **/
	std::vector<TESTS::Test*> tests = std::vector<TESTS::Test*>();
		
	/** register testclasses **/
	tests.push_back(new TESTS::StringHelperTest());
	
	HELPER::UiHelper::printok();

	HELPER::UiHelper::println(" - running tests...");	
	
	/** run tests **/
	std::vector<TESTS::Test*>::iterator i = tests.begin();
	for(;i != tests.end(); ++i) 
		(*i)->run();	
		
	/** shut down **/	
	HELPER::UiHelper::println(" - shutting down...");
	HELPER::UiHelper::printok();
	HELPER::UiHelper::println(" DBTesMa tests finished");

	/** finished **/
	HELPER::UiHelper::printTime();
	
	return 0;
}

