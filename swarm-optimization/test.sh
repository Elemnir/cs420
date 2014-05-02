#!/bin/bash
for i in `seq 1 5`;
do
    ./pso 200 30 0.8 2 2
    convert -delay 250 -loop 0 frame*.pgm run${i}.gif
done
