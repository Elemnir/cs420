CC=g++ --std=c++11 -O3

PROG=genetics

all: main 

main: main.cpp
	$(CC) -o $(PROG) main.cpp

debug:
	g++ --std=c++11 -g -Wall -Wextra -o $(PROG) main.cpp

clean:
	rm -rf *.o genetics
