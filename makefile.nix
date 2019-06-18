
include makefile.rules.gcc

ifdef DEBUG
 MAKEFLAGS += DEBUG=1
endif

BIN = dbtesma
BIN_TEST = dbtesma_test

DIRS = conf data data/wrapper helper
DIRS_TEST = tests

OBJ = main.o
OBJ_TEST = tests/test_main.o tests/test.o tests/stringhelpertest.o

OBJ_ALL = conf/parser.o conf/validator.o \
	data/column.o data/funcdep.o data/funcdepgraph.o data/table.o data/schema.o data/condincdep.o \
	data/wrapper/charwrapper.o data/wrapper/datatypewrapper.o data/wrapper/intwrapper.o data/wrapper/varcharwrapper.o \
	helper/cliargs.o helper/file.o helper/fontcolor.o helper/strings.o helper/ui.o

.PHONY: all clean rebuild check

ifdef TESTS
all: $(OBJ_TEST)
	for i in $(DIRS) $(DIRS_TEST); do $(MAKE) TESTS=1 -C $$i; done
	$(GCC) $(CFLAGS) -o $(BIN_TEST) $(OBJ_TEST) $(OBJ_ALL)

else
all: $(OBJ)
	for i in $(DIRS); do $(MAKE) -C $$i; done
	$(GCC) $(CFLAGS) -o $(BIN) $(OBJ) $(OBJ_ALL)

endif

clean:
	$(RM) -r $(BIN) $(BIN_TEST) $(OBJ) $(OBJ_TEST) $(OBJ_ALL)

rebuild: clean all

check: clean
	$(MAKE) TESTS=1
