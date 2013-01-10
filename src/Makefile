COMPILER=g++

INCLUDE_DIRS = $(ICE_HOME)/include

LIB_DIRS = $(MCPP_HOME)/lib

%.o: %.cc
	$(COMPILER) -Wall -I $(INCLUDE_DIRS) -g -c $<

slice2xml: main.o RstGen.o
	$(COMPILER) -Wall -I $(INCLUDE_DIRS) -o $@ *.o -L$(MCPP_HOME)/lib -L$(MCPP_HOME)/lib -L$(ICE_HOME)/lib -L$(ICE_HOME)/lib -lIceUtil -lSlice -lmcpp

RstGen.o: RstGen.cpp RstGen.hpp

main.o: main.cpp

clean:
	rm -f *.o core slice2xml
