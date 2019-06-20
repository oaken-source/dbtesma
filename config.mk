
CXX        ?= g++
CXXFLAGS   ?= -Wall -Wextra -pedantic -std=c++0x -Weffc++ -O3 -march=native -pipe

ifdef DEBUG
  CXXFLAGS += -DHAVE_DEBUG
endif
