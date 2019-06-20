
include config.mk

BIN = dbtesma

OBJ = main.o \
      conf/parser.o conf/validator.o \
      data/column.o data/funcdep.o data/funcdepgraph.o data/table.o data/schema.o data/condincdep.o \
      data/wrapper/charwrapper.o data/wrapper/datatypewrapper.o data/wrapper/intwrapper.o data/wrapper/varcharwrapper.o \
      helper/cliargs.o helper/file.o helper/fontcolor.o helper/strings.o helper/ui.o

.PHONY: all clean

all: $(BIN)

clean:
	$(RM) $(BIN) $(BIN_TEST) $(OBJ) $(OBJ_TEST) $(OBJ_ALL)

$(BIN): $(OBJ) $(OBJ_ALL)
	$(CXX) $(CXXFLAGS) -o $@ $^

