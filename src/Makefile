COMPILER=g++

ifneq ($(strip $(ICE_HOME)),)
	INCLUDE_DIRS += -I$(ICE_HOME)/include
	LIB_DIRS += -L$(ICE_HOME)/include
endif

ifneq ($(strip $(MCPP_HOME)),)
	INCLUDE_DIRS += -I$(MCPP_HOME)/include
	LIB_DIRS += -L$(MCPP_HOME)/include
endif

COMMON_OBJS = RstGen.o SliceCommentParser.o LinkHandler.o
HEADERS = $(wildcard *.hpp)

%.o: %.cpp
	$(COMPILER) -Wall $(INCLUDE_DIRS) -g -c $<

slice2rst: main.o $(COMMON_OBJS)
	$(COMPILER) -Wall $(INCLUDE_DIRS) -o $@ $^ $(LIB_DIRS) -lIceUtil -lSlice -lmcpp

test: testParser.o $(COMMON_OBJS)
	$(COMPILER) -Wall $(INCLUDE_DIRS) -o $@ $^ $(LIB_DIRS) -lIceUtil -lSlice -lmcpp


RstGen.o: RstGen.cpp $(HEADERS)
SliceCommentParser.o: SliceCommentParser.cpp $(HEADERS)
LinkHandler.o: LinkHandler.cpp $(HEADERS)

clean:
	rm -f *.o slice2rst test

