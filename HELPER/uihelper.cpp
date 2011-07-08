
#include "uihelper.h"

namespace HELPER {

/** public ******************************************************************************************************/

	void UiHelper::println(const char in[])
	{
		std::cout << in << std::endl;
	}

	void UiHelper::printlnspaced(const char in[], unsigned int space)
	{
		unsigned int i;
		for(i = 0; i < space; i++)
			std::cout << " ";
		std::cout << in << std::endl;
	}
	
	void UiHelper::overrln(const char in[])
	{
		std::cout << "\r" << in << std::endl;
	}

	void UiHelper::emptyln()
	{
		std::cout << std::endl;
	}
	
	void UiHelper::printraw(const char in[])
	{
		std::cout << in << std::flush;
	}

	void UiHelper::printerr(const char in[])
	{
		std::cout << "      [";
		HELPER::OsAbstractionHelper::setColorRed();
		std::cout << " !! ";
		HELPER::OsAbstractionHelper::unsetColor();
		std::cout << "]  -  Error: " << in << std::endl;
	}

	void UiHelper::printwrn(const char in[])
	{
		std::cout << "      [";
		HELPER::OsAbstractionHelper::setColorYellow();
		std::cout << " -- ";
		HELPER::OsAbstractionHelper::unsetColor();
		std::cout << "]  -  Warning: " << in << std::endl;
	}

	void UiHelper::printok()
	{
		std::cout << "      [";
		HELPER::OsAbstractionHelper::setColorGreen();
		std::cout << " ok ";
		HELPER::OsAbstractionHelper::unsetColor();
		std::cout << "]" << std::endl;
	}

	void UiHelper::overrok()
	{
		std::cout << "\r" << "      [";
		HELPER::OsAbstractionHelper::setColorGreen();
		std::cout << " ok ";
		HELPER::OsAbstractionHelper::unsetColor();
		std::cout << "]" << std::endl;
	}

	void UiHelper::overrfail()
	{
		std::cout << "\r" << "      [";
		HELPER::OsAbstractionHelper::setColorRed();
		std::cout << " !! ";
		HELPER::OsAbstractionHelper::unsetColor();
		std::cout << "]" << std::endl;
	}
	
	void UiHelper::printTime()
	{
		struct tm *current;
		time_t now;
	
		time(&now);
		current = localtime(&now);

		std::cout << "  [" << std::setfill('0') << std::setw(2) << current->tm_hour << ":" << std::setw(2) << current->tm_min << ":" << std::setw(2) << current->tm_sec << "]" << std::endl;
	}

	void UiHelper::startProgress(const char name[])
	{
		std::cout << "      [    ] " << name  << std::flush;
	}

} // namespaces













