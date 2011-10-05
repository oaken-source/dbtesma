
include makefile.rules.gcc

ifdef DEBUG
 MFLAGS = DEBUG=1
else
 MFLAGS = 
endif

BIN = dbtesma
BIN_TEST = dbtesma_test

DIRS = CONF DATA DATA/WRAPPER GEN HELPER
DIRS_TEST = TESTS

OBJ = main.o
OBJ_TEST = TESTS/test_main.o TESTS/test.o TESTS/stringhelpertest.o

OBJ_ALL = CONF/configparser.o CONF/configvalidator.o \
	DATA/column.o DATA/funcdep.o DATA/funcdepgraph.o DATA/table.o DATA/schema.o \
	DATA/WRAPPER/charwrapper.o DATA/WRAPPER/datatypewrapper.o DATA/WRAPPER/intwrapper.o DATA/WRAPPER/varcharwrapper.o \
	GEN/datagenerator.o \
	HELPER/cliargs.o HELPER/file.o HELPER/osabstractionhelper.o HELPER/stringhelper.o HELPER/ui.o

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
