CC = g++
FLAGS = -fopenmp -std=c++11


.PHONY: all test clean
all :Sampling.o Search.o
	$(CC) $(FLAGS) main2.cpp Sampling.o Search.o -o System2Body
Sampling.o : Sampling.cpp
	$(CC) $(FLAGS) -c Sampling.cpp
Search.o : Search.cpp
	$(CC) $(FLAGS) -c Search.cpp
test: all
	./System2Body

clean:
	rm -f *.o