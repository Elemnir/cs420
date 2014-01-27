CC=g++ --std=c++11

PROG=network.cpp -o neuralnet
DEPS=neuron.cpp

all: main

init: param gen_problem_files

gen_problem_files:
	python setgen.py one
	python setgen.py two

main: $(DEPS) network.cpp
	$(CC) $(DEPS) $(PROG)

param:
	cp config.template config.param

clean:
	rm -rf *.o neuralnet *.training *.testing *.validate
