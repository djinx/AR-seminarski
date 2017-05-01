

dp: dp.o parser.o lexer.o
	g++ -std=c++11 -o dp dp.o parser.o lexer.o

dp.o: dp.cpp fol.hpp
	g++ -std=c++11 -c -o dp.o dp.cpp

parser.o: parser.cpp fol.hpp
	g++ -std=c++11 -c -o parser.o parser.cpp

lexer.o: lexer.cpp parser.hpp
	g++ -std=c++11 -c -o lexer.o lexer.cpp

parser.cpp: parser.ypp
	bison -d -o parser.cpp parser.ypp

lexer.cpp: lexer.lpp
	flex -o lexer.cpp lexer.lpp

clean:
	rm -f *.o
	rm -f *~
	rm -f parser.cpp
	rm -f lexer.cpp
	rm -f parser.hpp
	rm -f dp
