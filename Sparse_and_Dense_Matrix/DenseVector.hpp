#ifndef DENSEVECTOR_H_
#define DENSEVECTOR_H_

#include <cstring>
#include <cstdlib>
#include <stdexcept>

class DenseVector {
private:
    size_t columns;
    size_t row;
    
    double *data;
    
    size_t *cur_column;
    
    static double eps;
    
public:

    friend class IterableDenseMatrix;
    
    DenseVector();
    DenseVector(size_t, size_t);
    DenseVector(const DenseVector&);
    ~DenseVector();
    
    DenseVector& operator=(const DenseVector&);
    
    const DenseVector& operator+(const size_t) const;
    DenseVector& operator+(const size_t);
    
    double operator[](size_t) const;
    double& operator[](size_t);
    
    double operator*() const;
    double& operator*();
};

#endif /* DENSEVECTOR_H_ */