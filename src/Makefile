COMPILER=g++

INCLUDE_DIRS = $(ICE_HOME)/include

LIB_DIRS = $(MCPP_HOME)/lib

%.o: %.cpp
	$(COMPILER) -Wall -I $(INCLUDE_DIRS) -g -c $<

slice2rst: main.o RstGen.o SliceCommentParser.o
	$(COMPILER) -Wall -I $(INCLUDE_DIRS) -o $@ *.o -L$(MCPP_HOME)/lib -L$(MCPP_HOME)/lib -L$(ICE_HOME)/lib -L$(ICE_HOME)/lib -lIceUtil -lSlice -lmcpp

RstGen.o: RstGen.cpp RstGen.hpp SliceCommentParser.hpp

main.o: main.cpp

clean:
	rm -f *.o core slice2rst
