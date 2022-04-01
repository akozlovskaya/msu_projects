#include "IterableSparseMatrix.hpp"
//-------------------------------------------------------------------------
double IterableSparseMatrix::eps = SparseVector::eps;

/******IterableSparseMatrix constructors and destructor******/

IterableSparseMatrix::IterableSparseMatrix(size_t size_) 
    : mtx_size(size_), cur_size(0), cap(0), data(nullptr)
{ 
    cur_row = new size_t(0);
}

IterableSparseMatrix::IterableSparseMatrix(const IterableSparseMatrix &src)
    : mtx_size(src.mtx_size), cur_size(src.cur_size), cap(src.cap)
{
    data = new SparseVector[cap];
    for(size_t i = 0; i < cur_size; ++i) {
        data[i] = src.data[i];
    }
    cur_row = new size_t(*src.cur_row);
}

IterableSparseMatrix::~IterableSparseMatrix()
{
    delete [] data;
    delete cur_row;
}

/******IterableSparseMatrix operators******/

IterableSparseMatrix&
IterableSparseMatrix::operator=(const IterableSparseMatrix &src)
{
    delete [] data;
    mtx_size = src.mtx_size;
    cur_size = src.cur_size;
    cap = src.cap;
    data = new SparseVector[cap];
    for(size_t i = 0; i < cur_size; ++i) {
        data[i] = src.data[i];
    }
    return *this;
}

bool
IterableSparseMatrix::operator==(const IterableSparseMatrix &mtx) const
{
    if (mtx_size != mtx.mtx_size) return false;
    for (size_t i = 0; i < cur_size; ++i) {
        for (size_t j = 0; j < data[i].size; ++j) {
            double val_1 = data[i].data[j].value;
            double val_2 = mtx.get(data[i].row, data[i].data[j].column);
            if(abs(val_1 - val_2) > eps) { return false; }
        }
    }
    for (size_t i = 0; i < mtx.cur_size; ++i) {
        for (size_t j = 0; j < mtx.data[i].size; ++j) {
            double val_1 = mtx.data[i].data[j].value;
            double val_2 = get(mtx.data[i].row, mtx.data[i].data[j].column);
            if(abs(val_1 - val_2) > eps) { return false; }
        }
    }
    return true;
}

bool
IterableSparseMatrix::operator!=(const IterableSparseMatrix &mtx) const
{
    return !(*this == mtx);
}

IterableSparseMatrix
IterableSparseMatrix::operator+(const IterableSparseMatrix &mtx) const
{
    if (mtx_size != mtx.mtx_size) {
        throw std::domain_error{"IterableSparseMatrix::operator+"};
    }
    IterableSparseMatrix sum = mtx;
    for (size_t i = 0; i < cur_size; ++i) {
        bool f = false;
        if(data[i].size > 0) {
            for (size_t j = 0; j < mtx.cur_size; ++j) {
                if (sum.data[j].row == data[i].row) {
                    sum.data[j] = mtx.data[j] + data[i];
                    f = true;
                    break;
                }
            }
            if (!f) { sum.push_back(data[i]); }
        }
    }
    return sum;
}

IterableSparseMatrix
IterableSparseMatrix::operator*(const IterableSparseMatrix &mtx) const
{
    if (mtx_size != mtx.mtx_size) {
        throw std::domain_error{"IterableSparseMatrix::operator*"};
    }
    IterableSparseMatrix cmps(mtx_size);
    for (size_t clmn = 0; clmn < mtx.mtx_size; ++clmn) {
        for (size_t i = 0; i < cur_size; ++i) {
            double sum = 0.0;
            for (size_t j = 0; j < data[i].size; ++j) {
                double mulpr_1 = data[i].data[j].value;
                double mulpr_2 = mtx.get(data[i].data[j].column, clmn);
                sum += mulpr_1 * mulpr_2;
            }
            if (abs(sum) > eps) { cmps.set(data[i].row, clmn, sum); }
        }
    }
    return cmps;
}

const SparseVector&
IterableSparseMatrix::operator[](size_t row_) const
{
    if (row_ + 1 > mtx_size ) {
        throw std::out_of_range{"IterableSparseMatrix::operator[]"};
    }
    for (size_t i = 0; i < cur_size; ++i) {
        if (data[i].row == row_) return data[i];
    }
    return empty_row;
}

SparseVector&
IterableSparseMatrix::operator[](size_t row_)
{
    if (row_ + 1 > mtx_size) {
        throw std::out_of_range{"IterableSparseMatrix::operator[]"};
    }
    for (size_t i = 0; i < cur_size; ++i) {
        if (data[i].row == row_) return data[i];
    }
    
    SparseVector tmp(row_, mtx_size);
    push_back(tmp);
    
    return data[cur_size-1];
}

const SparseVector&
IterableSparseMatrix::operator*() const
{
    if (*cur_row + 1 > mtx_size) {
        throw std::out_of_range{"IterableSparseMatrix::operator*"};
    }
    for (size_t i = 0; i < cur_size; ++i) {
        if (data[i].row == *cur_row) { 
            *cur_row = 0;
            return data[i];
        }
    }
    return empty_row;
    
}

SparseVector&
IterableSparseMatrix::operator*()
{
    if (*cur_row + 1 > mtx_size) {
        throw std::out_of_range{"IterableSparseMatrix::operator*"};
    }
    for (size_t i = 0; i < cur_size; ++i) {
        if (data[i].row == *cur_row) {
            *cur_row = 0;
            return data[i];
        }
    }
    
    SparseVector tmp(*cur_row, mtx_size);
    push_back(tmp);
    
    return data[cur_size-1];
}

const IterableSparseMatrix&
IterableSparseMatrix::operator+(const size_t shft) const
{
    if (*cur_row + shft + 1 > mtx_size) {
        throw std::out_of_range{"IterableSparseMatrix::operator+"};
    }
    *cur_row += shft;
    return *this;
}

IterableSparseMatrix&
IterableSparseMatrix::operator+(const size_t shft)
{
    if (*cur_row + shft + 1 > mtx_size) {
        throw std::out_of_range{"IterableSparseMatrix::operator+"};
    }
    *cur_row += shft;
    return *this;
}

/******IterableSparseMatrix methods******/

void IterableSparseMatrix::push_back(SparseVector& vtr)
{
    if (cur_size + 1 > cap) {
        cap = cap == 0 ? 2 : cap * 2;
        SparseVector *tmp = new SparseVector[cap];
        for (size_t i = 0; i < cur_size; ++i) {
            tmp[i] = data[i];
        }
        delete []data;
        data = tmp;
    }
    data[cur_size++] = vtr;
}

void
IterableSparseMatrix::set(size_t row_, size_t column_, double val_)
{
    if (row_ + 1 > mtx_size || column_ + 1 > mtx_size) {
        throw std::out_of_range{"IterableSparseMatrix::set"};
    }
    for (size_t i = 0; i < cur_size; ++i) {
        if (data[i].row == row_) {
            data[i].set(column_, val_);
            return;
        }
    }
    if (abs(val_) > eps) {
        SparseVector tmp(row_, mtx_size);
        tmp.set(column_, val_);
        push_back(tmp);
    }
}

double
IterableSparseMatrix::get(size_t row_, size_t column_) const
{
    if (row_ + 1 > mtx_size || column_ + 1 > mtx_size) {
        throw std::out_of_range{"IterableSparseMatrix::get"};
    }
    for (size_t i = 0; i < cur_size; ++i) {
        if (data[i].row == row_) { return data[i].get(column_); }
    }
    return 0.0;
}

void
IterableSparseMatrix::print() const
{
    std::cout << std::endl;
    for (size_t i = 0; i < mtx_size; ++i) {
        for (size_t j = 0; j < mtx_size; ++j) {
            std::cout << "  " << get(i, j);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
//-------------------------------------------------------------------------
size_t
IterableSparseMatrix::size() const
{
    return mtx_size;
}

IterableSquareMatrix::RowIterator
IterableSparseMatrix::iterate_rows(size_t row_) const
{
    Info tmp(row_, mtx_size, mtx_size, this, 0);
    
    for (size_t i = 0; i < cur_size; ++i) {
        if (data[i].row == row_) {
            
            //ищем следующий min столбец
            size_t min_clmn = mtx_size;
            
            for (size_t j = 0; j < data[i].size; ++j) {
                size_t cur_clmn = data[i].data[j].column;
                if (cur_clmn < min_clmn && abs(data[i].data[j].value) > eps) {
                    min_clmn = cur_clmn;
                    tmp.val = data[i].data[j].value;
                }
            }
            
            tmp.column = min_clmn;
            return IterableSquareMatrix::RowIterator(tmp);
        }
    }
    return this->row_end(row_);
}

IterableSquareMatrix::ColumnIterator
IterableSparseMatrix::iterate_columns(size_t column_) const
{
    Info tmp(mtx_size, column_, mtx_size, this, 0);
    
    //ищем следующую min строку
    
    size_t min_row = mtx_size;
    for (size_t i = 0; i < cur_size; ++i) {
        
        size_t cur_row = data[i].row;
        
        if (cur_row < min_row && abs(data[i].get(column_)) > eps)
        {
            min_row = cur_row;
            tmp.val = data[i].get(column_);
        }
    }
    
    tmp.row = min_row;
    return IterableSquareMatrix::ColumnIterator(tmp);
}
//-------------------------------------------------------------------------
IterableSquareMatrix::RowIterator
IterableSparseMatrix::row_end(size_t row_) const
{
    Info tmp(row_, mtx_size, mtx_size, this, 0);
    return IterableSquareMatrix::RowIterator(tmp);
}

IterableSquareMatrix::ColumnIterator
IterableSparseMatrix::column_end(size_t column_) const
{
    Info tmp(mtx_size, column_, mtx_size, this, 0);
    return IterableSquareMatrix::ColumnIterator(tmp);
}
//-------------------------------------------------------------------------
double
IterableSparseMatrix::next_elem_row(size_t row_, size_t &column_) const
{
    for (size_t i = 0; i < cur_size; ++i) {
        if (data[i].row == row_) {
            
            //ищем следующий min столбец
            size_t min_clmn = mtx_size;
            double val = 0;
            
            for (size_t j = 0; j < data[i].size; ++j) {
                size_t cur_clmn = data[i].data[j].column;
                if (cur_clmn < min_clmn && cur_clmn > column_ && abs(data[i].data[j].value) > eps) {
                    min_clmn = cur_clmn;
                    val = data[i].data[j].value;
                }
            }
            column_ = min_clmn;
            return val;
        }
    }
    column_ = mtx_size;
    return 0;
}

double
IterableSparseMatrix::next_elem_column(size_t &row_, size_t column_) const
{
    //ищем следующую min строку
    size_t min_row = mtx_size;
    double val = 0;
    
    for (size_t i = 0; i < cur_size; ++i) {
        size_t cur_row = data[i].row;
        if (cur_row > row_ && cur_row < min_row &&
                abs(data[i].get(column_)) > eps)
        {
            min_row = cur_row;
            val = get(cur_row, column_);
        }
    }
    row_ = min_row;
    return val;
}
//---------------------------------------------------------------------