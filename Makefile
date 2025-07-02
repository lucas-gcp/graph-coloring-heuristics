CPP = g++ -O3 -Wall -Wextra --pedantic

all: program

program: graph.o gcp.o main.o
	$(CPP) graph.o gcp.o main.o -o program

graph.o: graph.cpp graph.h
	$(CPP) -c graph.cpp

gcp.o: gcp.cpp gcp.h
	$(CPP) -c gcp.cpp

main.o: main.cpp
	$(CPP) -c main.cpp