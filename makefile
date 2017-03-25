
catapults.out: catapults.o test.o
	gcc catapults.o test.o -o catapults.out && rm *.o

catapults.o: catapults.c
	gcc -c catapults.c

test.o: model/test.c model/test.h
	gcc -c model/test.c
