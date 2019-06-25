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

#ifndef MACROS_H
#define MACROS_H

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#define DEFAULT_VALUE_LENGTH 11

#define TESMAFILE_STR  "\
###############################################################\n\
#       example structure file for dbtesma v. >= 1.0.3        #\n\
###############################################################\n\
# table structure:                                            #\n\
# extern_info(id1, id2, comment, extra) with:                 #\n\
#   - primary key: (id1, id2)                                 #\n\
#   - functional dependency: comment -> extra                 #\n\
# main(id, fkey_id1, fkey_id2, fd1, fd2, fd3, fd4, fd5) with: #\n\
#   - primary key: (id)                                       #\n\
#   - foreign key: (fkey_id1, fkey_id2) -> (id1, id2)         #\n\
#   - functional dependency: fd1, fd2 -> fd3                  #\n\
#   - functional dependency: fd5 -> fd3                       #\n\
#   - functional dependency: fd3 -> fd4                       #\n\
###############################################################\n\
\n\
table={\n\
  name=\"extern_info\"\n\
  rows=\"500\"\n\
  column={\n\
    name=\"id1\"\n\
    datatype=\"int\"\n\
    length=\"11\"\n\
    key=\"primary\"\n\
    key_group=\"1\"\n\
  }\n\
  column={\n\
    name=\"id2\"\n\
    datatype=\"int\"\n\
    length=\"11\"\n\
    key=\"primary\"\n\
    key_group=\"1\"\n\
  }\n\
  column={\n\
    name=\"comment\"\n\
    datatype=\"varchar\"\n\
    length=\"50\"\n\
    unique=\"10\"\n\
  }\n\
  column={\n\
    name=\"extra\"\n\
    datatype=\"int\"\n\
    length=\"5\"\n\
  }\n\
  functional_dep={\n\
    lhs=\"comment\"\n\
    rhs=\"extra\"\n\
  }\n\
}\n\
\n\
table={\n\
  name=\"main\"\n\
  rows=\"1000000\"\n\
  column={\n\
    name=\"id\"\n\
    datatype=\"int\"\n\
    length=\"11\"\n\
    basevalue=\"1\"\n\
    key=\"primary\"\n\
  }\n\
  column={\n\
    name=\"fkey_id1\"\n\
    datatype=\"int\"\n\
    length=\"11\"\n\
    foreignkey=\"extern_info:id1\"\n\
  }\n\
  column={\n\
    name=\"fkey_id2\"\n\
    datatype=\"int\"\n\
    length=\"11\"\n\
    foreignkey=\"extern_info:id2\"\n\
  }\n\
  column={\n\
    name=\"fd1\"\n\
    datatype=\"int\"\n\
    length=\"11\"\n\
    unique=\"10\"\n\
  }\n\
  column={\n\
    name=\"fd2\"\n\
    datatype=\"int\"\n\
    length=\"11\"\n\
    unique=\"10\"\n\
  }\n\
  column={\n\
    name=\"fd3\"\n\
    datatype=\"varchar\"\n\
    length=\"11\"\n\
  }\n\
  column={\n\
    name=\"fd4\"\n\
    datatype=\"int\"\n\
    length=\"11\"\n\
  }\n\
  column={\n\
    name=\"fd5\"\n\
    datatype=\"int\"\n\
    length=\"11\"\n\
  }\n\
  functional_dep={\n\
    lhs=\"fd1,fd2\"\n\
    rhs=\"fd3\"\n\
  }\n\
  functional_dep={\n\
    lhs=\"fd5\"\n\
    rhs=\"fd3\"\n\
  }\n\
  functional_dep={\n\
    lhs=\"fd3\"\n\
    rhs=\"fd4\"\n\
  }\n\
}\
"

#endif // MACROS_H
