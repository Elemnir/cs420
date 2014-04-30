CC=g++ --std=c++11

PROG=pso

all: main 

main: main.o particle.o
	$(CC) -o $(PROG) main.o particle.o

main.o: main.cpp
	$(CC) -c main.cpp

particle.o: particle.h particle.cpp
	$(CC) -c particle.cpp

clean:
	rm *.o $(PROG)
