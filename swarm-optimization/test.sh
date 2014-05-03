#!/bin/bash
for i in `seq 1 5`;
do
    ./pso 200 30 0.6 2 2
    convert -delay 50 frame*.pgm run${i}.gif
    rm frame*.pgm
done
