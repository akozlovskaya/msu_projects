#ifndef ITRBLDENSEMATRIX_H_
#define ITRBLDENSEMATRIX_H_

#include <cstring>
#include <cstdlib>
#include <iostream>

#include "IterableSquareMatrix.hpp"
#include "DenseVector.hpp"

class IterableDenseMatrix : public IterableSquareMatrix {
    size_t mtx_size;
    DenseVector *data;
    
    size_t *cur_row;
    
    static double eps;
    
    double next_elem_row(size_t, size_t&) const;
    double next_elem_column(size_t&, size_t) const;
    
public:
    
    IterableDenseMatrix(size_t);
    IterableDenseMatrix(const IterableDenseMatrix&);
    ~IterableDenseMatrix();  
    
    size_t size() const;
    void set(size_t, size_t, double);
    double get(size_t, size_t) const;
    
    IterableDenseMatrix& operator=(const IterableDenseMatrix&);
    
    bool operator==(const IterableDenseMatrix&) const;
    bool operator!=(const IterableDenseMatrix&) const;
    
    IterableDenseMatrix operator+(const IterableDenseMatrix &) const;
    IterableDenseMatrix operator*(const IterableDenseMatrix &) const;
    
    const IterableDenseMatrix& operator+(const size_t) const;
    IterableDenseMatrix& operator+(const size_t);
    
    const DenseVector& operator*() const;
    DenseVector& operator*();
    
    const DenseVector& operator[](size_t) const;
    DenseVector& operator[](size_t);
    
    IterableSquareMatrix::RowIterator iterate_rows(size_t) const;
    IterableSquareMatrix::ColumnIterator iterate_columns(size_t) const;
    
    IterableSquareMatrix::RowIterator row_end(size_t) const;
    IterableSquareMatrix::ColumnIterator column_end(size_t) const;
    
    void print() const;
};
#endif /* ITRBLDENSEMATRIX_H_ */