#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include "../DATA/config.h"
#include "../HELPER/uihelper.h"

namespace GEN {

class DataGenerator
{

	/**
		main data generation class
		tasks:
			starts the data generation process for each table
	**/

public:

	DataGenerator(DATA::Config *conf) : _conf(conf) { srand(time(NULL)); };
	DataGenerator(const DataGenerator &obj) : _conf(obj._conf) {};
	DataGenerator& operator=(const DataGenerator&);
	~DataGenerator();

	/** standard table processing **/
	void processTables(bool);
	/** offset-only table processing **/
	void processTablesOffsetsOnly(bool);

private:

	/** config **/ 
	DATA::Config *_conf;
};

} // namespaces

#endif // DATAGENERATOR_H
