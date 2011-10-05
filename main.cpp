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

  std::string file = cah->pair(CP_F);
  bool exists = HELPER::File::exists(file);


  if(cah->flag(CP_Help)) // print help
    HELPER::UiHelper::printraw(USAGE_STR);
  else if(cah->flag(CP_Version)) // print version
    HELPER::UiHelper::println(VERSION_STR);
  else if(cah->flag(CP_About)) // print 'about' stuff
    HELPER::UiHelper::println(ABOUT_STR);
  else if(cah->flag(CP_Schema)) // print schema information to stdout
  {    
    if(!exists)
      HELPER::UiHelper::printerr("'%s' not found", file.c_str());
    if(cah->hasError() || !exists) // quit on error
    {
      std::pair<std::string, bool> msg = cah->popMsg();
      while(msg.first.length() > 0)
      {
        if(msg.second)
          HELPER::UiHelper::printerr(msg.first.c_str());
        else
          HELPER::UiHelper::printwrn(msg.first.c_str());
        
        msg = cah->popMsg();
      }
      return 1;
    }

    DATA::Schema* config = new DATA::Schema();
    CONF::Configparser* cp = new CONF::Configparser(file);
    
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
    else // invalid schema
      HELPER::UiHelper::printerr(config->getErrorString().c_str());
  }
  else // real data generation
  {
    HELPER::UiHelper::printTime();
    
    HELPER::UiHelper::println(" DBTesMa data generator");
    HELPER::UiHelper::println(" - starting up...");
    
    if(cah->hasMsg()) // errors/warnings during parsing?
    {
      std::pair<std::string, bool> msg = cah->popMsg();
      while(msg.first.length() > 0)
      {
        if(msg.second)
          HELPER::UiHelper::printerr(msg.first.c_str());
        else
          HELPER::UiHelper::printwrn(msg.first.c_str());
        
        msg = cah->popMsg();
      }
    }
    if(cah->hasError()) // quit on error
    {
      HELPER::UiHelper::printraw(USAGE_STR);
      return 1;
    }
        
    if(!exists && !cah->flag(CP_Generate)) // file not found warning
      HELPER::UiHelper::printwrn("'%s' not found", file.c_str());
    else
      HELPER::UiHelper::printok();

    if(cah->flag(CP_Generate) || !exists) // generate example tesmafile
    {
      HELPER::UiHelper::println(" - generating tesmafile...");
      if(HELPER::File::writeRaw(file, TESMAFILE_STR))
        HELPER::UiHelper::printok();
      else
        HELPER::UiHelper::printerr("failed to write to file '%s'", file.c_str());
    }
    else // initiate generation
    {
      HELPER::UiHelper::println(" - parsing configuration...");

      DATA::Schema* config = new DATA::Schema();
      CONF::Configparser* cp = new CONF::Configparser(file);
      
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
  cah->addFlag("--noheader", CP_NoHeader);
  cah->addFlag("--hardenFDs", CP_HardenFds);
  cah->addFlag("--help", CP_Help);  
  cah->addFlag("--version", CP_Version);  
  cah->addFlag("--about", CP_About);  
  
  cah->addPair("-f", CP_F, "tesmafile");
}


