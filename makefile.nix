
include makefile.rules.gcc

ifdef DEBUG
 MFLAGS = DEBUG=1
else
 MFLAGS =
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

ifdef TESTS
all: $(OBJ_TEST)
	@for i in $(DIRS) $(DIRS_TEST); do make $(MFLAGS) TESTS=1 -C $$i; done
	$(GCC) $(CFLAGS) -o $(BIN_TEST) $(OBJ_TEST) $(OBJ_ALL)

else
all: $(OBJ)
	@for i in $(DIRS); do make $(MFLAGS) -C $$i; done
	$(GCC) $(CFLAGS) -o $(BIN) $(OBJ) $(OBJ_ALL)

endif

clean:
	@rm -rf $(BIN) $(BIN_TEST) $(OBJ) $(OBJ_TEST) $(OBJ_ALL)

rebuild: clean all

tests: clean
	@make $(MFLAGS) TESTS=1
