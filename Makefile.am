
## process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = dbtesma

dbtesma_SOURCES = \
  src/main.cpp \
  src/conf/parser.cpp src/conf/validator.cpp \
  src/data/column.cpp src/data/funcdep.cpp src/data/funcdepgraph.cpp \
  src/data/table.cpp src/data/schema.cpp src/data/condincdep.cpp \
  src/data/wrapper/charwrapper.cpp src/data/wrapper/datatypewrapper.cpp \
  src/data/wrapper/intwrapper.cpp src/data/wrapper/varcharwrapper.cpp \
  src/helper/cliargs.cpp src/helper/file.cpp src/helper/fontcolor.cpp \
  src/helper/strings.cpp src/helper/ui.cpp
