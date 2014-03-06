#!/bin/bash

for i in {50..500..50}
do
	time ./hopfield 1000 $i
	cp graph1.csv output/graph1_$i.csv
	cp graph2.csv output/graph2_$i.csv
done
