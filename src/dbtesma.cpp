 /******************************************************************************
 *                          dbtesma data generator                             *
 *                                                                             *
 *    Copyright (C) 2011 - 2019  Andreas Grapentin                             *
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

#include "dbtesma.h"

#include "helper/ui.h"
#include "helper/file.h"
#include "data/schema.h"
#include "conf/parser.h"

int
main(int argc, char *argv[])
{
  // process command line arguments
  struct arguments args = { "tesmafile", 0, 0 };
  argp_parse(&argp, argc, argv, 0, 0, &args);

  std::string file(args.tesmafile);
  bool exists = helper::File::exists(file);

  if(args.flags & FLAG_SCHEMA)
  {
    // print schema information to stdout
    if(!exists)
      helper::Ui::printerr("'%s' not found", file.c_str());

    data::Schema* config = new data::Schema();
    conf::Parser* cp = new conf::Parser(file, config);

    srand(time(NULL));

    /** process schema config **/
    if(cp->parseAndValidate())
    {
      /** valid schema - print schema information to stdout **/
      if(args.flags & FLAG_HIDDEN)
        config->buildSchemaWithoutDatatypes();
      else if(args.flags & FLAG_JSON)
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

    if(!exists && !(args.flags & FLAG_GENERATE)) // file not found warning
      helper::Ui::printwrn("'%s' not found", file.c_str());
    else
      helper::Ui::printok();

    if(args.flags & FLAG_GENERATE || !exists) // generate example tesmafile
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

      config->setHardenFdFlag(args.flags & FLAG_HARDENFDEPS);

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
          (*i)->print(args.flags & FLAG_NOHEADER, config->hasHardenedFds());
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
