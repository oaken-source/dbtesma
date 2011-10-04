 /******************************************************************************
 *                          dbtesma data generator                             *
 *                                                                             *
 *    Copyright (C) 2011  Andreas Henning                                      *
 *                                                                             *
 *    This program is free software: you can redistribute it and/or modify     *
 *    it under the terms of the GNU General Public License as published by     *
 *    the Free Software Foundation, either version 3 of the License, or        *
 *    (at your option) any later version.                                      *
 *                                                                             *
 *    This program is distributed in the hope that it will be useful,          *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *    GNU General Public License for more details.                             *
 *                                                                             *
 *    You should have received a copy of the GNU General Public License        *
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
 ******************************************************************************/

#include "CONF/configparser.h"
#include "GEN/datagenerator.h"
#include "HELPER/cliargshelper.h"
#include "HELPER/filehelper.h"

	/** enumeration for all possible command line interface (cli) arguments **/
	enum e_CliParams
	{
		CP_F,
		CP_Verbose,
		CP_Generate,
		CP_Schema,
		CP_Hidden,
		CP_AsJson,
		CP_Offsets,
		CP_NoHeader,
		CP_HardenFds,
		CP_Help,
		CP_Version,
		CP_About
	}; // CP = CliParams

int main(int argc, char *argv[])
{
	/** entry point **/

	/** process command line arguments **/
	HELPER::CliArgsHelper *cah = new HELPER::CliArgsHelper(0, 0);
	
	cah->addParam("--verbose", CP_Verbose, HELPER::CliArgsHelper::FLAG_PARAM); 
	cah->addParam("--generate", CP_Generate, HELPER::CliArgsHelper::FLAG_PARAM);
	cah->addParam("--schema", CP_Schema, HELPER::CliArgsHelper::FLAG_PARAM);
	cah->addParam("--hidden", CP_Hidden, HELPER::CliArgsHelper::FLAG_PARAM);
	cah->addParam("--asJSON", CP_AsJson, HELPER::CliArgsHelper::FLAG_PARAM);
	cah->addParam("--offsets", CP_Offsets, HELPER::CliArgsHelper::FLAG_PARAM);
	cah->addParam("--noheader", CP_NoHeader, HELPER::CliArgsHelper::FLAG_PARAM);
	cah->addParam("--hardenFDs", CP_HardenFds, HELPER::CliArgsHelper::FLAG_PARAM);
	cah->addParam("--help", CP_Help, HELPER::CliArgsHelper::FLAG_PARAM);	
	cah->addParam("--version", CP_Version, HELPER::CliArgsHelper::FLAG_PARAM);	
	cah->addParam("--about", CP_About, HELPER::CliArgsHelper::FLAG_PARAM);	
	
	cah->addParam("-f", CP_F, HELPER::CliArgsHelper::PAIR_PARAM);

	std::map<int, bool> flags;
	std::map<int, std::string> pairs;
	std::vector<std::string> values;

	std::string error;

	bool good = cah->parse(flags, pairs, values, argc, argv, error);

	/** set standard tesmafile **/
	std::string tesmafile = "tesmafile";
	if(pairs.find(CP_F) != pairs.end())
		tesmafile = pairs[CP_F];
		
	/** file exists? **/
	bool file_exists = HELPER::FileHelper::isExistingFile(tesmafile);
		
	/** dispatch application task **/
	if(flags.find(CP_Help) != flags.end())
	{
		/** --help was set **/
		HELPER::UiHelper::printraw(USAGE_STR);
	}
	else if(flags.find(CP_Version) != flags.end())
	{
		/** --version was set **/
		HELPER::UiHelper::println(VERSION_STR);
	}
	else if(flags.find(CP_About) != flags.end())
	{
		/** --about was set **/
		HELPER::UiHelper::println(ABOUT_STR);
	}
	else if(flags.find(CP_Schema) != flags.end())
	{
		/** --schema was set **/
		DATA::Schema* config = new DATA::Schema();
		CONF::Configparser* cp = new CONF::Configparser(tesmafile);
		
		srand(time(NULL));
		
		/** procss schema config **/ 
		if(cp->parseAndValidate(config))
		{
			/** valid schema - print schema information to stdout **/
			if(flags.find(CP_Hidden) != flags.end())
				config->buildSchemaWithoutDatatypes();
			else if(flags.find(CP_AsJson) != flags.end())
				config->buildSchemaAsJSON();
			else
				config->buildSchema();
		}
		else
		{
			/** schema was not valid **/
			HELPER::UiHelper::printerr(config->getErrorString().c_str());
		}
	}
	else if(flags.find(CP_Offsets) != flags.end())
	{
		/** --offsets was set **/
		DATA::Schema* config = new DATA::Schema();
		CONF::Configparser* cp = new CONF::Configparser(tesmafile);
		
		srand(time(NULL));
		
		config->setHardenFdFlag(flags.find(CP_HardenFds) != flags.end());
		
		/** process schema config **/
		if(cp->parseAndValidate(config))
		{
			/** valid schema - start data generation for offets only **/
			GEN::DataGenerator* dg = new GEN::DataGenerator(config);			
			
			config->buildSchemaAsJSON();

			bool noHeader = (flags.find(CP_NoHeader) != flags.end());
			
			dg->processTablesOffsetsOnly(noHeader);	
		}
		else
		{
			/** schema was not valid **/
			HELPER::UiHelper::printerr(config->getErrorString().c_str());
		}
	}
	else
	{
		/** no additional generation flags set - standard procedure **/
		HELPER::UiHelper::printTime();
		
		HELPER::UiHelper::println(" DBTesMa data generator");
		HELPER::UiHelper::println(" - starting up...");
		
		/** test for invalid cli args configuration **/
		if(!good)
		{
			HELPER::UiHelper::printerr(error.c_str());
			return 1;
		}
		else if(!file_exists && flags.find(CP_Generate) == flags.end())
		{
			error = "\"" + tesmafile + "\" was not found. Trying to generate...";
			HELPER::UiHelper::printwrn(error.c_str());
		}
		else
			HELPER::UiHelper::printok();

		/** need to generate example tesmafile? then do it. **/
		if(flags.find(CP_Generate) != flags.end() || !file_exists)
		{
			HELPER::UiHelper::println(" - generating tesmafile...");
			if(HELPER::FileHelper::writeRaw(tesmafile, TESMAFILE_STR))
				HELPER::UiHelper::printok();
			else
			{
				error = "failed to write to file \"" + tesmafile + "\"";
				HELPER::UiHelper::printerr(error.c_str());
			}
		}
		else
		{
			/** all signals green **/ 
			HELPER::UiHelper::println(" - parsing configuration...");

			DATA::Schema* config = new DATA::Schema();
			CONF::Configparser* cp = new CONF::Configparser(tesmafile);
			
			srand(time(NULL));

			config->setHardenFdFlag(flags.find(CP_HardenFds) != flags.end());
			
			/** process schema config **/
			if(cp->parseAndValidate(config))
			{
				/** valid schema configuration - start generation **/
				GEN::DataGenerator* dg = new GEN::DataGenerator(config);			
				
				if(DEBUG)
					config->dumpToStdout();

				HELPER::UiHelper::printok();
				HELPER::UiHelper::println(" - generating tables...");	

				bool noHeader = (flags.find(CP_NoHeader) != flags.end());
				
				dg->processTables(noHeader);
			}
			else
			{
				/** schema was not valid **/
				HELPER::UiHelper::printerr(config->getErrorString().c_str());
			}
		}

		HELPER::UiHelper::println(" - shutting down...");
		HELPER::UiHelper::printok();
		HELPER::UiHelper::println(" DBTesMa finished");

		HELPER::UiHelper::printTime();
	}

	/** finished **/
	
	return 0;
}
