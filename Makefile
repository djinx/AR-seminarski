PROGRAM   = dp
CXX       = g++
CXXFLAGS  = -g -std=c++11 -Wall

$(PROGRAM): dp.o
	$(CXX) -o $(PROGRAM) dp.o

.PHONY: clean

clean:
	rm *.o $(PROGRAM) *core


