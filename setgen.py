from math import *
from sys import argv
from random import uniform

def f1(x, y):
	return (1 + (sin( pi * x / 2.0 ) * cos( pi * y / 2.0 ))) / 2.0

def f2(x, y, z):
	return (3.0 / 13) * ((x * x / 2.0 ) + (y * y / 3.0) + (z * z / 4.0))

if len(argv) == 1:
	print "Valid problem sets are: one two\n"
	exit()

script, problem = argv

train = open(problem + '.training', 'w')
test = open(problem + '.testing', 'w')
valid = open(problem + '.validate', 'w')
	
if problem == 'one':
	for i in range(0, 200):
		x = uniform(-2.0, 2.0)
		y = uniform(-2.0, 2.0)
		line = str(x) + ' ' + str(y) + ' ' + str( f1(x,y) ) + '\n'
		train.write(line)

	for i in range(0, 100):
		x = uniform(-2.0, 2.0)
		y = uniform(-2.0, 2.0)
		line = str(x) + ' ' + str(y) + ' ' + str( f1(x,y) ) + '\n'
		test.write(line)

	for i in range(0, 50):
		x = uniform(-2.0, 2.0)
		y = uniform(-2.0, 2.0)
		line = str(x) + ' ' + str(y) + ' ' + str( f1(x,y) ) + '\n'
		valid.write(line)
	
elif problem == 'two':
	for i in range(0, 200):
		x = uniform(-2.0, 2.0)
		y = uniform(-2.0, 2.0)
		z = uniform(-2.0, 2.0)
		line = str(x) + " " + str(y) + " " + str(z) + " " + str( f2(x,y,z) ) + "\n"
		train.write(line)

	for i in range(0, 100):
		x = uniform(-2.0, 2.0)
		y = uniform(-2.0, 2.0)
		z = uniform(-2.0, 2.0)
		line = str(x) + " " + str(y) + " " + str(z) + " " + str( f2(x,y,z) ) + "\n"
		test.write(line)
	
	for i in range(0, 50):
		x = uniform(-2.0, 2.0)
		y = uniform(-2.0, 2.0)
		z = uniform(-2.0, 2.0)
		line = str(x) + " " + str(y) + " " + str(z) + " " + str( f2(x,y,z) ) + "\n"
		valid.write(line)

train.close()
test.close()
valid.close()

