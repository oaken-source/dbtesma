
include config.mk

BIN = dbtesma

OBJ = src/main.o \
      src/conf/parser.o src/conf/validator.o \
      src/data/column.o src/data/funcdep.o src/data/funcdepgraph.o src/data/table.o src/data/schema.o src/data/condincdep.o \
      src/data/wrapper/charwrapper.o src/data/wrapper/datatypewrapper.o src/data/wrapper/intwrapper.o src/data/wrapper/varcharwrapper.o \
      src/helper/cliargs.o src/helper/file.o src/helper/fontcolor.o src/helper/strings.o src/helper/ui.o

.PHONY: all clean

all: $(BIN)

clean:
	$(RM) $(BIN) $(BIN_TEST) $(OBJ) $(OBJ_TEST) $(OBJ_ALL)

$(BIN): $(OBJ) $(OBJ_ALL)
	$(CXX) $(CXXFLAGS) -o $@ $^

