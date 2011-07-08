#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <string>
#include <fstream>

namespace HELPER {

class FileHelper
{

	/**
	Files helper class
	tasks:
		helper class for file management methods
	todo:
		Yes, it's a lazy class. More file functionality has to be outsourced in the future.
	**/

public:

	static bool isExistingFile(std::string&);
	static bool writeRaw(std::string&, const char[]);

private:

};

} // namespaces

#endif // FILEHELPER_H
