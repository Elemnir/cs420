CC=g++ --std=c++11

PROG=aica

all: main 

main: main.cpp
	$(CC) -o $(PROG) main.cpp

debug:
	$(CC) -g -Wall -Wextra $(PROG) main.cpp

clean:
	rm $(PROG)
