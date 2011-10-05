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

#include "CONF/parser.h"
#include "DATA/schema.h"
#include "HELPER/ui.h"
#include "HELPER/cliargs.h"
#include "HELPER/file.h"

  /** enumeration of cli parameters **/
  enum e_CliParams
  {
    CP_F,
    CP_Verbose,
    CP_Generate,
    CP_Schema,
    CP_Hidden,
    CP_AsJson,
    CP_NoHeader,
    CP_HardenFds,
    CP_Help,
    CP_Version,
    CP_About
  };

  void setupCliArgs(HELPER::CliArgs*);
  
