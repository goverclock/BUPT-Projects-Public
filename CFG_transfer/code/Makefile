# -*- MakeFile -*-

all: main

main: main.o
	g++ main.o -o main
	
main.o: main.cpp
	g++ -c main.cpp

# Windows
clean:
	del *.o main.exe
