#ifndef SparseVECTOR_H_
#define SparseVECTOR_H_

#include <cstring>
#include <cstdlib>
#include <stdexcept>

struct Item {
    size_t column;
    double value;
};

class SparseVector {
private:
    size_t max;
    size_t row;
    size_t size;
    size_t cap;
    Item *data;
    
    size_t *cur_column;
    
    void push_back(Item&);
    
    static double eps;
    
public:

    friend class IterableSparseMatrix;
    
    SparseVector();
    SparseVector(size_t, size_t);
    SparseVector(const SparseVector&);
    ~SparseVector();
    
    double get(size_t) const;
    void set(size_t, double);
    
    SparseVector& operator=(const SparseVector&);
    SparseVector operator+(const SparseVector&) const;
    
    const SparseVector& operator+(const size_t) const;
    SparseVector& operator+(const size_t);
    
    double operator[](size_t) const;
    double& operator[](size_t);
    
    double operator*() const;
    double& operator*();
};

#endif /* SparseVECTOR_H_ */