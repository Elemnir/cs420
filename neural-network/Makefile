CC=g++ --std=c++11 -O3

PROG=network.cpp -o neuralnet

all: neuralnet init 

init: param gen_problem_files

gen_problem_files:
	python setgen.py one
	python setgen.py two

neuralnet: network.cpp
	$(CC) $(PROG)

param:
	cp config.template config.param

clean:
	rm -rf *.o neuralnet *.training *.testing *.validate
