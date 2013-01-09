COMPILER=g++

INCLUDE_DIRS = $(ICE_HOME)/include

LIB_DIRS = $(MCPP_HOME)/lib

%.o: %.cc
	$(COMPILER) -I $(INCLUDE_DIRS) -g -c $<

slice2xml: main.o XMLGen.o
	$(COMPILER) -I $(INCLUDE_DIRS) -o $@ *.o -L$(MCPP_HOME)/lib -Xlinker -R -Xlinker $(MCPP_HOME)/lib -L$(ICE_HOME)/lib -Xlinker -R -Xlinker $(ICE_HOME)/lib -lIceUtil -lSlice -lmcpp

XMLGen.o: XMLGen.cc XMLGen.h

main.o: main.cc

clean:
	rm -f *.o core slice2xml
