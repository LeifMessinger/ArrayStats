output: ArrayStats.o main.o
	g++ -std=c++17 -O2  ArrayStats.o main.o -o output 

ArrayStats.o: ArrayStats.cpp ArrayStats.h
	g++ -std=c++17 -O2 -c ArrayStats.cpp

main.o: main.cpp ArrayStats.h
	g++ -std=c++17 -O2 -c main.cpp

clean:
	rm -f -v *.o output

run:
	./output

debug:
	g++ -std=c++17 -O2 -g  ArrayStats.cpp main.cpp  -o output

