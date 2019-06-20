
include config.mk

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

.PHONY: all check clean

all: $(BIN)

check: $(BIN_TEST)
	./$(BIN_TEST)

clean:
	$(RM) $(BIN) $(BIN_TEST) $(OBJ) $(OBJ_TEST) $(OBJ_ALL)

$(BIN): $(OBJ) $(OBJ_ALL)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BIN_TEST): $(OBJ_TEST) $(OBJ_ALL)
	$(CXX) $(CXXFLAGS) -o $@ $^

