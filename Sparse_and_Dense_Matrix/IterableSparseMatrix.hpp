#ifndef ITRBLSPARSEMATRIX_H_
#define ITRBLSPARSEMATRIX_H_

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "IterableSquareMatrix.hpp"
#include "SparseVector.hpp"

class IterableSparseMatrix : public IterableSquareMatrix {
private:

    size_t mtx_size;
    size_t cur_size;
    size_t cap;
    SparseVector *data;
    
    size_t *cur_row;
    const SparseVector empty_row;
    
    void push_back(SparseVector&);
    
    static double eps;
    
    double next_elem_row(size_t, size_t&) const;
    double next_elem_column(size_t&, size_t) const;
    
public:

    IterableSparseMatrix(size_t);
    IterableSparseMatrix(size_t, size_t);
    IterableSparseMatrix(const IterableSparseMatrix&);
    ~IterableSparseMatrix(); 
    
    size_t size() const;
    void set(size_t, size_t, double);
    double get(size_t, size_t) const;
    
    IterableSparseMatrix& operator=(const IterableSparseMatrix&);
    
    bool operator==(const IterableSparseMatrix&) const;
    bool operator!=(const IterableSparseMatrix&) const;
    
    IterableSparseMatrix operator+(const IterableSparseMatrix &) const;
    IterableSparseMatrix operator*(const IterableSparseMatrix &) const;
    
    const IterableSparseMatrix& operator+(const size_t) const;
    IterableSparseMatrix& operator+(const size_t);
    
    const SparseVector& operator*() const;
    SparseVector& operator*();
    
    const SparseVector& operator[](size_t) const;
    SparseVector& operator[](size_t);
    
    //-------------------------------------------------------//
    
    IterableSquareMatrix::RowIterator iterate_rows(size_t) const;
    ColumnIterator iterate_columns(size_t) const;
    
    IterableSquareMatrix::RowIterator row_end(size_t) const;
    ColumnIterator column_end(size_t) const;
    
    //-------------------------------------------------------//
    
    void print() const;
};
#endif /* ITRBLSPARSEMATRIX_H_ */