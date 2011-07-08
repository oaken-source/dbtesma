
/**
todo:
	fix coding styles! enumeration style is uncool, comment style is not vim standard
	fit width of code lines to std vim screen width with tabsize of 4
**/


#include "CONF/configparser.h"
#include "GEN/datagenerator.h"
#include "HELPER/cliargshelper.h"
#include "HELPER/filehelper.h"

	/** enumeration for all possible command line interface (cli) arguments **/
	enum PARAMS
	{
		F,
		VERBOSE,
		GENERATE,
		SCHEMA,
		HIDDEN,
		ASJSON,
		OFFSETS,
		NOHEADER,
		HARDENFDS,
		HELP,
		VERSION,
		ABOUT
	} typedef PARAMS;

int main(int argc, char *argv[])
{
	/** entry point **/

	/** process command line arguments **/
	HELPER::CliArgsHelper *cah = new HELPER::CliArgsHelper(0, 0);
	
	cah->addParam("--verbose", VERBOSE, HELPER::CliArgsHelper::FLAG_PARAM); 
	cah->addParam("--generate", GENERATE, HELPER::CliArgsHelper::FLAG_PARAM);
	cah->addParam("--schema", SCHEMA, HELPER::CliArgsHelper::FLAG_PARAM);
	cah->addParam("--hidden", HIDDEN, HELPER::CliArgsHelper::FLAG_PARAM);
	cah->addParam("--asJSON", ASJSON, HELPER::CliArgsHelper::FLAG_PARAM);
	cah->addParam("--offsets", OFFSETS, HELPER::CliArgsHelper::FLAG_PARAM);
	cah->addParam("--noheader", NOHEADER, HELPER::CliArgsHelper::FLAG_PARAM);
	cah->addParam("--hardenFDs", HARDENFDS, HELPER::CliArgsHelper::FLAG_PARAM);
	cah->addParam("--help", HELP, HELPER::CliArgsHelper::FLAG_PARAM);	
	cah->addParam("--version", VERSION, HELPER::CliArgsHelper::FLAG_PARAM);	
	cah->addParam("--about", ABOUT, HELPER::CliArgsHelper::FLAG_PARAM);	
	
	cah->addParam("-f", F, HELPER::CliArgsHelper::PAIR_PARAM);

	std::map<int, bool> flags;
	std::map<int, std::string> pairs;
	std::vector<std::string> values;

	std::string error;

	bool good = cah->parse(flags, pairs, values, argc, argv, error);

	/** set standard tesmafile **/
	std::string tesmafile = "tesmafile";
	if(pairs.find(F) != pairs.end())
		tesmafile = pairs[F];
		
	/** file exists? **/
	bool file_exists = HELPER::FileHelper::isExistingFile(tesmafile);
		
	/** dispatch application task **/
	if(flags.find(HELP) != flags.end())
	{
		/** --help was set **/
		HELPER::UiHelper::printraw(USAGE_STR);
	}
	else if(flags.find(VERSION) != flags.end())
	{
		/** --version was set **/
		HELPER::UiHelper::println(VERSION_STR);
	}
	else if(flags.find(ABOUT) != flags.end())
	{
		/** --about was set **/
		HELPER::UiHelper::println(ABOUT_STR);
	}
	else if(flags.find(SCHEMA) != flags.end())
	{
		/** --schema was set **/
		DATA::Config* config = new DATA::Config();
		CONF::Configparser* cp = new CONF::Configparser(tesmafile);
		
		srand(time(NULL));
		
		/** procss schema config **/ 
		if(cp->parseAndValidate(config))
		{
			/** valid schema - print schema information to stdout **/
			if(flags.find(HIDDEN) != flags.end())
				config->buildSchemaWithoutDatatypes();
			else if(flags.find(ASJSON) != flags.end())
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
	else if(flags.find(OFFSETS) != flags.end())
	{
		/** --offsets was set **/
		DATA::Config* config = new DATA::Config();
		CONF::Configparser* cp = new CONF::Configparser(tesmafile);
		
		srand(time(NULL));
		
		config->setHardenFdFlag(flags.find(HARDENFDS) != flags.end());
		
		/** process schema config **/
		if(cp->parseAndValidate(config))
		{
			/** valid schema - start data generation for offets only **/
			GEN::DataGenerator* dg = new GEN::DataGenerator(config);			
			
			config->buildSchemaAsJSON();

			bool noHeader = (flags.find(NOHEADER) != flags.end());
			
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
		else if(!file_exists && flags.find(GENERATE) == flags.end())
		{
			error = "\"" + tesmafile + "\" was not found. Trying to generate...";
			HELPER::UiHelper::printwrn(error.c_str());
		}
		else
			HELPER::UiHelper::printok();

		/** need to generate example tesmafile? then do it. **/
		if(flags.find(GENERATE) != flags.end() || !file_exists)
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

			DATA::Config* config = new DATA::Config();
			CONF::Configparser* cp = new CONF::Configparser(tesmafile);
			
			srand(time(NULL));

			config->setHardenFdFlag(flags.find(HARDENFDS) != flags.end());
			
			/** process schema config **/
			if(cp->parseAndValidate(config))
			{
				/** valid schema configuration - start generation **/
				GEN::DataGenerator* dg = new GEN::DataGenerator(config);			
				
				if(DEBUG)
					config->dumpToStdout();

				HELPER::UiHelper::printok();
				HELPER::UiHelper::println(" - generating tables...");	

				bool noHeader = (flags.find(NOHEADER) != flags.end());
				
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
