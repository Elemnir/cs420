#!/bin/bash
for m in `seq 1 5`;
do
	./aica 1 -0.1 0 1 2 test_${m} 
	./aica 1 -0.1 -4 1 5 test_${m}
	./aica 1 -0.1 -2 1 5 test_${m} 
	./aica 1 -0.1 0 1 5 test_${m}
	./aica 1 -0.1 -6 1 9 test_${m}
	./aica 1 -0.1 -3 1 9 test_${m}
	./aica 1 -0.1 0 1 9 test_${m}
	./aica 1 -0.1 0 1 14 test_${m}
	./aica 1 -0.1 -1 3 5 test_${m}
	./aica 1 -0.1 0 3 5 test_${m}
	./aica 1 -0.1 -6 3 9 test_${m}
	./aica 1 -0.1 -3 3 9 test_${m}
	./aica 1 -0.1 0 3 9 test_${m}
	./aica 1 -0.1 0 3 14 test_${m}
	./aica 1 -0.1 -6 7 9 test_${m}
	./aica 1 -0.1 -3 7 14 test_${m}
	./aica 1 -0.1 0 12 14 test_${m}
done
