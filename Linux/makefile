all: battleships

battleships: main.o CoreFuncs.o
	g++ main.o CoreFuncs.o -o battleships

main.o: main.cpp
	g++ -c main.cpp CoreFuncs.hpp

CoreFuncs.o: CoreFuncs.cpp CoreFuncs.hpp
	g++ -c CoreFuncs.cpp

clean:
	rm -rf *.o hello