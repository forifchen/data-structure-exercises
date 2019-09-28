CXXFLAGS := -std=c++17 -g
SRC := $(wildcard *.cpp)
OBJ := $(patsubst %.cpp, %.o, $(SRC))
CXX := g++

.PHONY: clean test.output
%.runtest: %.test makefile
	./$<

%.test: %.spec.o catch.o %.o makefile
	$(CXX) $(CXXFLAGS) $*.spec.o $*.o catch.o -o $*.test

# pull in dependency info for *existing* .o files
-include $(OBJ:.o=.dep)

# compile and generate dependency info
%.o: %.cpp makefile
	cppcheck --enable=all $*.cpp
	$(CXX) -c $(CXXFLAGS) $*.cpp
	$(CXX) -MM $(CXXFLAGS) $*.cpp > $*.dep

clean: makefile
	@rm -f *.o *.dep test



