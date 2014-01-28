CC=g++ --std=c++11 -Wall -Wextra

PROG=network.cpp -o neuralnet

all: main

init: param gen_problem_files

gen_problem_files:
	python setgen.py one
	python setgen.py two

main: network.cpp
	$(CC) $(PROG)

param:
	cp config.template config.param

clean:
	rm -rf *.o neuralnet *.training *.testing *.validate
