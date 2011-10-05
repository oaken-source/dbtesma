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

#include "main.h"

int main(int argc, char *argv[])
{
  /** process command line parameters **/
  HELPER::CliArgs *cah = new HELPER::CliArgs(0, 0);
  setupCliArgs(cah);

  cah->parse(argc, argv);

  std::vector<std::string> values;

  std::string error;
  bool good = true;

  std::string tesmafile = cah->pair(CP_F);
    
  /** file exists? **/
  bool file_exists = HELPER::FileHelper::isExistingFile(tesmafile);
    
  /** dispatch application task **/
  if(cah->flag(CP_Help))
  {
    /** --help was set **/
    HELPER::UiHelper::printraw(USAGE_STR);
  }
  else if(cah->flag(CP_Version))
  {
    /** --version was set **/
    HELPER::UiHelper::println(VERSION_STR);
  }
  else if(cah->flag(CP_About))
  {
    /** --about was set **/
    HELPER::UiHelper::println(ABOUT_STR);
  }
  else if(cah->flag(CP_Schema))
  {
    /** --schema was set **/
    DATA::Schema* config = new DATA::Schema();
    CONF::Configparser* cp = new CONF::Configparser(tesmafile);
    
    srand(time(NULL));
    
    /** procss schema config **/ 
    if(cp->parseAndValidate(config))
    {
      /** valid schema - print schema information to stdout **/
      if(cah->flag(CP_Hidden))
        config->buildSchemaWithoutDatatypes();
      else if(cah->flag(CP_AsJson))
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
  else if(cah->flag(CP_Offsets))
  {
    /** --offsets was set **/
    DATA::Schema* config = new DATA::Schema();
    CONF::Configparser* cp = new CONF::Configparser(tesmafile);
    
    srand(time(NULL));
    
    config->setHardenFdFlag(cah->flag(CP_HardenFds));
    
    /** process schema config **/
    if(cp->parseAndValidate(config))
    {
      /** valid schema - start data generation for offets only **/
      GEN::DataGenerator* dg = new GEN::DataGenerator(config);      
      
      config->buildSchemaAsJSON();

      bool noHeader = (cah->flag(CP_NoHeader));
      
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
    else if(!file_exists && !cah->flag(CP_Generate))
    {
      error = "\"" + tesmafile + "\" was not found. Trying to generate...";
      HELPER::UiHelper::printwrn(error.c_str());
    }
    else
      HELPER::UiHelper::printok();

    /** need to generate example tesmafile? then do it. **/
    if(cah->flag(CP_Generate) || !file_exists)
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

      config->setHardenFdFlag(cah->flag(CP_HardenFds));
      
      /** process schema config **/
      if(cp->parseAndValidate(config))
      {
        /** valid schema configuration - start generation **/
        GEN::DataGenerator* dg = new GEN::DataGenerator(config);      
        
        if(DEBUG)
          config->dumpToStdout();

        HELPER::UiHelper::printok();
        HELPER::UiHelper::println(" - generating tables...");  

        bool noHeader = (cah->flag(CP_NoHeader));
        
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

/** misc functions ************************************************************/

void setupCliArgs(HELPER::CliArgs* cah)
{
  cah->addFlag("--verbose", CP_Verbose); 
  cah->addFlag("--generate", CP_Generate);
  cah->addFlag("--schema", CP_Schema);
  cah->addFlag("--hidden", CP_Hidden);
  cah->addFlag("--asJSON", CP_AsJson);
  cah->addFlag("--offsets", CP_Offsets);
  cah->addFlag("--noheader", CP_NoHeader);
  cah->addFlag("--hardenFDs", CP_HardenFds);
  cah->addFlag("--help", CP_Help);  
  cah->addFlag("--version", CP_Version);  
  cah->addFlag("--about", CP_About);  
  
  cah->addPair("-f", CP_F, "tesmafile");
}


