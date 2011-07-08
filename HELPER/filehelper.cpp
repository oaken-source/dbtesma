
#include "filehelper.h"

namespace HELPER {

/** public ******************************************************************************************************/

	bool FileHelper::isExistingFile(std::string& filename)
	{
		std::ifstream file(filename.c_str());
		return(file);
	}

	bool FileHelper::writeRaw(std::string& filename, const char in[])
	{
		bool good = true;
		std::ofstream tesmafile;
		tesmafile.open(filename.c_str());
		if(tesmafile.good())
			tesmafile << in;
		else
			good = false;
		tesmafile.close();
		return good;
	}
		
} // namespaces













