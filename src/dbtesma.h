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

#ifndef DBTESMA_H
#define DBTESMA_H

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <argp.h>

const char *argp_program_version = PACKAGE_STRING;
const char *argp_program_bug_address = PACKAGE_BUGREPORT;

const char doc[] = PACKAGE_NAME " - a database testdata maker";

static struct argp_option options[] =
{
  {"tesmafile", 'f', "FILE", 0, "read FILE as tesmafile", 0},
  {"generate", 1, 0, 0, "generate default tesmafile in current directory", 0},
  {"schema", 'S', 0, 0, "print parsed schema to stdout", 0},
  {"hidden", 'H', 0, 0, "implies --schema, omits datatype information", 0},
  {"json", 'J', 0, 0, "implies --schema, prints schma information as JSON", 0},
  {"noheader", 'n', 0, 0, "omits header when generating tables", 0},
  {"harden-fdeps", 'h', 0, 0, "try to eliminate false positive functional dependencies", 0},
  {"verbose", 'v', 0, 0, "be more verbose", 0},
  {"quiet", 'q', 0, 0, "be less verbose", 0},
  {0, 0, 0, 0, 0, 0}
};

enum dbtesma_flags
{
  FLAG_GENERATE    = 00001,
  FLAG_SCHEMA      = 00002,
  FLAG_HIDDEN      = 00004,
  FLAG_JSON        = 00010,
  FLAG_NOHEADER    = 00020,
  FLAG_HARDENFDEPS = 00040
};

struct arguments
{
  const char *tesmafile;
  int flags;
  int verbosity;
};

static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  struct arguments *args = (struct arguments*)state->input;

  switch (key)
    {
    case 'f':
      args->tesmafile = arg;
      break;
    case 1:
      args->flags |= FLAG_GENERATE;
      break;
    case 'S':
      args->flags |= FLAG_SCHEMA;
      break;
    case 'H':
      args->flags |= FLAG_HIDDEN | FLAG_SCHEMA;
      break;
    case 'J':
      args->flags |= FLAG_JSON | FLAG_SCHEMA;
      break;
    case 'n':
      args->flags |= FLAG_NOHEADER;
      break;
    case 'h':
      args->flags |= FLAG_HARDENFDEPS;
      break;
    case 'v':
      args->verbosity++;
      break;
    case 'q':
      args->verbosity--;
      break;
    case ARGP_KEY_ARG:
      argp_usage(state);
      break;
    case ARGP_KEY_END:
      if (state->arg_num)
        argp_usage(state);
      break;
    default:
      return ARGP_ERR_UNKNOWN;
    }

  return 0;
}

static struct argp argp = {options, &parse_opt, 0, doc, 0, 0, 0};

#endif
