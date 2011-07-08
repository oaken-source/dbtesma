#include "datagenerator.h"

namespace GEN {

/** public ******************************************************************************************************/

	DataGenerator& DataGenerator::operator=(const DataGenerator &rhs)
	{
		_conf = rhs._conf;
		return *this;
	}

	DataGenerator::~DataGenerator()
	{

	}

	void DataGenerator::processTables(bool noHeader)
	{
		std::vector<DATA::Table*>::iterator i;
		for(i = _conf->begin(); i != _conf->end(); i++)
		{
			std::string name;
			(*i)->getAttribute(DATA::Table::ATTR_NAME, name);
			HELPER::UiHelper::startProgress(name.c_str());
			(*i)->print(noHeader, _conf->hasHardenedFds());
			HELPER::UiHelper::overrok();			
		}
	}

	void DataGenerator::processTablesOffsetsOnly(bool noHeader)
	{
		std::vector<DATA::Table*>::iterator i;
		for(i = _conf->begin(); i != _conf->end(); i++)
			(*i)->printOffsetsOnly(noHeader, _conf->hasHardenedFds());	
	}

/** private *****************************************************************************************************/

} // namespaces
