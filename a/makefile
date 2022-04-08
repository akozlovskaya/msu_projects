CC=g++-11
FLAGS=-std=c++20 -Wall -pedantic -Wextra -Werror
TEST_LIBS=-lgtest_main -lgtest -lpthread

.PHONY: test clean

all: main test_bin

main: Parser.o main.cpp
	$(CC) $(FLAGS) Parser.o main.cpp -o main

Parser.o: Parser.cpp Parser.hpp
	$(CC) $(FLAGS) Parser.cpp -c

test_bin: Parser.o tests.cpp
	$(CC) $(FLAGS) Parser.o tests.cpp -o test_bin $(TEST_LIBS)

test:
	./test_bin

clean:
	rm -f *.o main test_bin