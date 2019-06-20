 /******************************************************************************
 *                          dbtesma data generator                             *
 *                                                                             *
 *    Copyright (C) 2011  Andreas Grapentin                                    *
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
  helper::CliArgs *cah = new helper::CliArgs(0, 0);
  setupCliArgs(cah);

  cah->parse(argc, argv);

  std::string file = cah->pair(CP_F);
  bool exists = helper::File::exists(file);


  if(cah->flag(CP_Help)) // print help
    helper::Ui::printraw(USAGE_STR);
  else if(cah->flag(CP_Version)) // print version
    helper::Ui::println(VERSION_STR);
  else if(cah->flag(CP_About)) // print 'about' stuff
    helper::Ui::println(ABOUT_STR);
  else if(cah->flag(CP_Schema)) // print schema information to stdout
  {
    if(!exists)
      helper::Ui::printerr("'%s' not found", file.c_str());
    if(cah->hasError() || !exists) // quit on error
    {
      std::pair<std::string, bool> msg = cah->popMsg();
      while(msg.first.length() > 0)
      {
        if(msg.second)
          helper::Ui::printerr(msg.first.c_str());
        else
          helper::Ui::printwrn(msg.first.c_str());

        msg = cah->popMsg();
      }
      return 1;
    }

    data::Schema* config = new data::Schema();
    conf::Parser* cp = new conf::Parser(file, config);

    srand(time(NULL));

    /** procss schema config **/
    if(cp->parseAndValidate())
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
      helper::Ui::printerr(config->getErrorString().c_str());
  }
  else // real data generation
  {
    helper::Ui::printTime();

    helper::Ui::println(" DBTesMa data generator");
    helper::Ui::println(" - starting up...");

    if(cah->hasMsg()) // errors/warnings during parsing?
    {
      std::pair<std::string, bool> msg = cah->popMsg();
      while(msg.first.length() > 0)
      {
        if(msg.second)
          helper::Ui::printerr(msg.first.c_str());
        else
          helper::Ui::printwrn(msg.first.c_str());

        msg = cah->popMsg();
      }
    }
    if(cah->hasError()) // quit on error
    {
      helper::Ui::printraw(USAGE_STR);
      return 1;
    }

    if(!exists && !cah->flag(CP_Generate)) // file not found warning
      helper::Ui::printwrn("'%s' not found", file.c_str());
    else
      helper::Ui::printok();

    if(cah->flag(CP_Generate) || !exists) // generate example tesmafile
    {
      helper::Ui::println(" - generating tesmafile...");
      if(helper::File::writeRaw(file, TESMAFILE_STR))
        helper::Ui::printok();
      else
        helper::Ui::printerr("failed to write to file '%s'", file.c_str());
    }
    else // initiate generation
    {
      helper::Ui::println(" - parsing configuration...");

      data::Schema* config = new data::Schema();
      conf::Parser* cp = new conf::Parser(file, config);

      srand(time(NULL));

      config->setHardenFdFlag(cah->flag(CP_HardenFds));

      /** process schema config **/
      if(cp->parseAndValidate())
      {
        /** valid schema configuration - start generation **/
        if(DEBUG)
          config->dumpToStdout();

        helper::Ui::printok();
        helper::Ui::println(" - generating tables...");

        std::vector<data::Table*>::iterator i;
        for(i = config->begin(); i != config->end(); i++)
        {
          std::string name;
          (*i)->getAttribute(data::Table::A_Name, name);
          helper::Ui::startProgress(name.c_str());
          (*i)->print(cah->flag(CP_NoHeader), config->hasHardenedFds());
          helper::Ui::overrok();
        }
      }
      else
      {
        /** schema was not valid **/
        helper::Ui::printerr(config->getErrorString().c_str());
      }
    }

    helper::Ui::println(" - shutting down...");
    helper::Ui::printok();
    helper::Ui::println(" DBTesMa finished");

    helper::Ui::printTime();
  }

  return 0;
}

/** misc functions ************************************************************/

void setupCliArgs(helper::CliArgs* cah)
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

