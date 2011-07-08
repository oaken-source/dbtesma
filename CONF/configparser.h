#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include "configvalidator.h"

namespace CONF {

class Configparser
{

	/**
	Config Data Class
	tasks:
		cumulates input from tesmafile
	**/

	/** enumeration containing the possible parsing contexts **/
	enum CONTEXT
	{
		NONE,
		TABLE,
		COLUMN,
		FUNCDEP
	} typedef CONTEXT;

	
public:

	Configparser(std::string &in) : _filename(in), _in(new std::ifstream(in.c_str())), _context(NONE) {};
	Configparser(const Configparser &obj) : _filename(obj._filename), _in(obj._in), _context(obj._context) {};
	Configparser& operator=(const Configparser&);
	~Configparser();

	/** main parsing method - starts parsing, and afterward starts validation **/
	bool parseAndValidate(DATA::Config*);

private:
	
	/** top level parsing **/
	bool parse(DATA::Config*);

	/** line-scoped parsing - dispatches to context line-scoped methods **/
	bool processLine(std::string&, DATA::Config*);
	
	/** contexted line-scoped parsing methods **/
	bool processLineContextNone(std::string&, DATA::Config*);
	bool processLineContextTable(std::string&, DATA::Config*);
	bool processLineContextColumn(std::string&, DATA::Config*);
	bool processLineContextFuncDep(std::string&, DATA::Config*);

	std::string _filename;
	std::ifstream *_in;
	CONTEXT _context;
};

} // namespaces

#endif // CONFIGPARSER_H
