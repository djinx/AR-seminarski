PROGRAM=dp
CXX       = g++
CXXFLAGS  = -g -std=c++0x -Wall

$(PROGRAM): dp.o
	$(CXX) -o $(PROGRAM) dp.o

.PHONY: clean dist

clean:
	-rm *.o $(PROGRAM) *core

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)


