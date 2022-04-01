CC=g++
CFLAGS=-c -Wall
LDFLAGS=-O2 -ftrapv -fsanitize=undefined,address -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wmissing-field-initializers -Wcast-qual -Wwrite-strings -lm
SOURCES=IterableSquareMatrix.cpp Info.cpp IterableDenseMatrix.cpp IterableSparseMatrix.cpp DenseVector.cpp SparseVector.cpp RowIterator.cpp ColumnIterator.cpp GraphAlgorithms.cpp
HEADERS=IterableSquareMatrix.hpp IterableDenseMatrix.hpp IterableSparseMatrix.hpp DenseVector.hpp SparseVector.hpp GraphAlgorithms.hpp
OBJECTS=$(SOURCES:.cpp=.o)  
compile: prog.o $(OBJECTS) 
	$(CC) $(LDFLAGS) prog.o $(OBJECTS) -o prog
run: compile
	./prog
prog.o: main.cpp $(HEADERS)
	$(CC) $(CFLAGS) $< -o prog.o
show: sparse1 sparse2 dense1 dense2
sparse1: sparse1.cpp $(HEADERS) $(OBJECTS)
	$(CC) $(LDFLAGS) $< $(OBJECTS) -o sparse1
sparse2: sparse2.cpp $(HEADERS) $(OBJECTS)
	$(CC) $(LDFLAGS) $< $(OBJECTS) -o sparse2
dense1: dense1.cpp $(HEADERS) $(OBJECTS)
	$(CC) $(LDFLAGS) $< $(OBJECTS) -o dense1
dense2: dense2.cpp $(HEADERS) $(OBJECTS)
	$(CC) $(LDFLAGS) $< $(OBJECTS) -o dense2
IterableSquareMatrix.o: IterableSquareMatrix.cpp IterableSquareMatrix.hpp
	$(CC) $(CFLAGS) $<
Info.o: Info.cpp IterableSquareMatrix.hpp
	$(CC) $(CFLAGS) $<
IterableDenseMatrix.o: IterableDenseMatrix.cpp IterableDenseMatrix.hpp
	$(CC) $(CFLAGS) $<
IterableSparseMatrix.o: IterableSparseMatrix.cpp IterableSparseMatrix.hpp
	$(CC) $(CFLAGS) $<
DenseVector.o: DenseVector.cpp DenseVector.hpp
	$(CC) $(CFLAGS) $<
SparseVector.o: SparseVector.cpp SparseVector.hpp
	$(CC) $(CFLAGS) $<
RowIterator.o: RowIterator.cpp IterableSparseMatrix.hpp
	$(CC) $(CFLAGS) $<
ColumnIterator.o: ColumnIterator.cpp IterableSparseMatrix.hpp
	$(CC) $(CFLAGS) $<
GraphAlgorithms.o: GraphAlgorithms.cpp GraphAlgorithms.hpp
	$(CC) $(CFLAGS) $<
vd: main.cpp $(SOURCES) $(HEADERS)
	$(CC) $< $(SOURCES)
	valgrind ./a.out
clean:
	rm *.o prog a.out sparse1 sparse2 dense1 dense2