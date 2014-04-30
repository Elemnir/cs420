CC=g++ --std=c++11

PROG=pso

all: main 

main: main.cpp particle
	$(CC) -o $(PROG) main.cpp

particle: particle.cpp
	$(CC) -c particle.cpp

debug:
	$(CC) -g -Wall -Wextra $(PROG) main.cpp

clean:
	rm *.o $(PROG)
