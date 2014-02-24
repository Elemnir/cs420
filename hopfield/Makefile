CC=g++ --std=c++11 -O3

PROG=-o hopfield

all: hopfield 

hopfield: main.cpp
	$(CC) $(PROG) main.cpp

debug:
	g++ --std=c++11 -g -Wall -Wextra $(PROG) main.cpp

clean:
	rm -rf *.o hopfield
