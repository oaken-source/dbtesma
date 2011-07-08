#ifndef CONFIGVALIDATOR_H
#define CONFIGVALIDATOR_H

#include "../DATA/config.h"

namespace CONF {

class Configvalidator
{

	/**
	Config Validation Class
	tasks:
		validates data in config
	todo:
		this class is a monster! tame it!
	**/

public:
	Configvalidator() : _conf(NULL) {};
	Configvalidator(const Configvalidator &obj) : _conf(obj._conf) {};
	Configvalidator& operator=(const Configvalidator&);
	~Configvalidator();

	/** top level validation - calls medium level validation methods **/
	bool validate(DATA::Config*);

private:

	/** table related validation **/
	bool validateTableCount();
	bool validateTableNames();
	bool validateTableAttributes();
	bool validateTableColumns();
	bool validateTableFuncdeps();

	/** column related validation **/
	bool validateColumnCount(DATA::Table*);
	bool validateColumnNames(DATA::Table*);
	bool validateColumnDatatypes(DATA::Table*);
	bool validateColumnSize(DATA::Table*);

	/** functional dependencies - first pass **/
	bool validateFuncdepColumns(DATA::Table*);
	void explodeFuncdeps(DATA::Table*);
	bool validateFuncdeps(DATA::Table*);
	
	/** functional dependencies - second pass **/
	bool revalidateFuncdeps();
	bool revalidateTableFuncdeps(DATA::Table*);
	
	/** datatype preparation **/
	void populateDatatypeWrappers(DATA::Table*);

	/** bottom-level column related validation **/
	bool validateColumnKey(DATA::Table*);
	bool validateColumnBasevalue(DATA::Table*);
	bool validateColumnUniqueCount(DATA::Table*);

	/** foreign key validation **/
	bool validateForeignKeyConstraints();
	bool validateForeignKeyConstraints(DATA::Table*);

	/** harden fd validation - more strict than basic fd validation **/
	bool validateHardenedFds();
	bool validateHardenedFds(DATA::Table*);
	
	DATA::Config* _conf;
	
};

} // namespaces

#endif // CONFIGVALIDATOR_H
