#ifndef ITRBLSQUAREMATRIX_H_
#define ITRBLSQUAREMATRIX_H_

#include <cstdlib>

//-----------------------------------
class IterableSquareMatrix;

typedef struct Info {
    size_t row;
    size_t column;
    size_t size;
    const IterableSquareMatrix* mtx;
    double val;
    
    Info(size_t, size_t, size_t, const IterableSquareMatrix*, double);
    bool operator==(const Info&) const;
} Info;
//-----------------------------------
class IterableSquareMatrix {
    
    virtual double next_elem_row(size_t, size_t&) const = 0;
    virtual double next_elem_column(size_t&, size_t) const = 0;
    
public:

    class RowIterator {
        Info info;
    public:
        RowIterator(const Info&);
        RowIterator(const RowIterator&);
        ~RowIterator();
        
        double operator*() const;     //получение значения
        const Info* operator->() const;
        
        RowIterator& operator++();    // версия префикс
        RowIterator operator++(int);       // версия постфикс
        
        bool operator==(const RowIterator&) const;
        bool operator!=(const RowIterator&) const;
    };
    
    class ColumnIterator {
        Info info;
    public:
        ColumnIterator(const Info&);
        ColumnIterator(const ColumnIterator&);
        ~ColumnIterator();
        
        double operator*() const;     //получение значения
        const Info* operator->() const;
        
        ColumnIterator& operator++();         // префикс
        ColumnIterator operator++(int);       // постфикс
        
        bool operator==(const ColumnIterator&) const;
        bool operator!=(const ColumnIterator&) const;
    };
    
    virtual ~IterableSquareMatrix();  
    
    virtual RowIterator iterate_rows(size_t) const = 0;
    virtual ColumnIterator iterate_columns(size_t) const = 0;
    
    virtual RowIterator row_end(size_t) const = 0;
    virtual ColumnIterator column_end(size_t) const = 0;
    
    virtual size_t size() const = 0;
    
};

#endif /* ITRBLSQUAREMATRIX_H_ */