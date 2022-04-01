#include "IterableDenseMatrix.hpp"
//-------------------------------------------------------------------------

double IterableDenseMatrix::eps = DenseVector::eps;

/******IterableDenseMatrix constructors and destructor******/

IterableDenseMatrix::IterableDenseMatrix(size_t size_) 
    : mtx_size(size_)
{ 
    cur_row = new size_t(0);
    data = new DenseVector[mtx_size];
    for(size_t i = 0; i < mtx_size; ++i) {
        DenseVector tmp = DenseVector(i, mtx_size);
        data[i] = tmp;
    }
}

IterableDenseMatrix::IterableDenseMatrix(const IterableDenseMatrix &src)
    : mtx_size(src.mtx_size)
{
    data = new DenseVector[mtx_size];
    for(size_t i = 0; i < mtx_size; ++i) {
        data[i] = src.data[i];
    }
    cur_row = new size_t(*src.cur_row);
}

IterableDenseMatrix::~IterableDenseMatrix()
{
    delete [] data;
    delete cur_row;
}

/******IterableDenseMatrix operators******/

IterableDenseMatrix&
IterableDenseMatrix::operator=(const IterableDenseMatrix &src)
{
    delete [] data;
    mtx_size = src.mtx_size;
    data = new DenseVector[mtx_size];
    for(size_t i = 0; i < mtx_size; ++i) {
        data[i] = src.data[i];
    }
    return *this;
}

bool
IterableDenseMatrix::operator==(const IterableDenseMatrix &mtx) const
{
    if (mtx_size != mtx.mtx_size) return false;
    for (size_t i = 0; i < mtx_size; ++i) {
        for (size_t j = 0; j < mtx_size; ++j) {
            double val_1 = mtx.data[i].data[j];
            double val_2 = data[i].data[j];
            if(abs(val_1 - val_2) > eps) { return false; }
        }
    }
    return true;
}

bool
IterableDenseMatrix::operator!=(const IterableDenseMatrix &mtx) const
{
    return !(*this == mtx);
}

IterableDenseMatrix
IterableDenseMatrix::operator+(const IterableDenseMatrix &mtx) const
{
    if (mtx_size != mtx.mtx_size) {
        throw std::domain_error{"IterableDenseMatrix::operator+"};
    }
    IterableDenseMatrix sum(mtx_size);
    for (size_t i = 0; i < mtx_size; ++i) {
        for (size_t j = 0; j < mtx_size; ++j) {
            sum.data[i][j] = mtx.data[i][j] + data[i][j];
        }
    }
    return sum;
}

IterableDenseMatrix
IterableDenseMatrix::operator*(const IterableDenseMatrix &mtx) const
{
    if (mtx_size != mtx.mtx_size) {
        throw std::domain_error{"IterableDenseMatrix::operator*"};
    }
    IterableDenseMatrix cmps(mtx_size);
    for (size_t i = 0; i < mtx_size; ++i) {
        for (size_t j = 0; j < mtx.mtx_size; ++j) {
            double sum = 0;
            for (size_t k = 0; k < data[i].columns; ++k) {
                sum += data[i].data[k] * mtx.data[k].data[j];
            }
            cmps[i][j] = sum;
        }
    }
    return cmps;
}

const DenseVector&
IterableDenseMatrix::operator[](size_t row_) const
{
    if (row_ >= mtx_size ) {
        throw std::out_of_range{"IterableDenseMatrix::operator[]"};
    }
    return data[row_];
}

DenseVector&
IterableDenseMatrix::operator[](size_t row_)
{
    if (row_ >= mtx_size) {
        throw std::out_of_range{"IterableDenseMatrix::operator[]"};
    }
    return data[row_];
}

const DenseVector&
IterableDenseMatrix::operator*() const
{
    if (*cur_row >= mtx_size) {
        throw std::out_of_range{"IterableDenseMatrix::operator*"};
    }
    size_t tmp_row = *cur_row;
    *cur_row = 0;
    return data[tmp_row];
    
}

DenseVector&
IterableDenseMatrix::operator*()
{
    if (*cur_row >= mtx_size) {
        throw std::out_of_range{"IterableDenseMatrix::operator*"};
    }
    size_t tmp_row = *cur_row;
    *cur_row = 0;
    return data[tmp_row];
}

const IterableDenseMatrix&
IterableDenseMatrix::operator+(const size_t shft) const
{
    if (*cur_row + shft >= mtx_size) {
        throw std::out_of_range{"IterableDenseMatrix::operator+"};
    }
    *cur_row += shft;
    return *this;
}

IterableDenseMatrix&
IterableDenseMatrix::operator+(const size_t shft)
{
    if (*cur_row + shft >= mtx_size) {
        throw std::out_of_range{"IterableDenseMatrix::operator+"};
    }
    *cur_row += shft;
    return *this;
}

/******IterableDenseMatrix methods******/

void
IterableDenseMatrix::set(size_t row_, size_t column_, double val_)
{
    if (row_ >= mtx_size || column_ >= mtx_size) {
        throw std::out_of_range{"IterableDenseMatrix::set"};
    }
    data[row_][column_] = val_;
}

double
IterableDenseMatrix::get(size_t row_, size_t column_) const
{
    if (row_ >= mtx_size || column_ >= mtx_size) {
        throw std::out_of_range{"IterableDenseMatrix::get"};
    }
    return data[row_][column_];
}

void
IterableDenseMatrix::print() const
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
IterableDenseMatrix::size() const
{
    return mtx_size;
}

IterableSquareMatrix::RowIterator
IterableDenseMatrix::iterate_rows(size_t row_) const
{
    for (size_t i = 0; i < size(); ++i) {
        if (abs(get(row_, i)) > eps) {
            Info tmp(row_, i, mtx_size, this, get(row_, i));
            return IterableSquareMatrix::RowIterator(tmp);
        }
    }
    return this->row_end(row_);
}

IterableSquareMatrix::ColumnIterator
IterableDenseMatrix::iterate_columns(size_t column_) const
{
    for (size_t i = 0; i < size(); ++i) {
        if (abs(get(i, column_)) > eps) {
            Info tmp(i, column_, mtx_size, this, get(i, column_));
            return IterableSquareMatrix::ColumnIterator(tmp);
        }
    }
    return this->column_end(column_);
}
//-------------------------------------------------------------------------
IterableSquareMatrix::RowIterator
IterableDenseMatrix::row_end(size_t row_) const
{
    Info tmp(row_, mtx_size, mtx_size, this, 0);
    return IterableSquareMatrix::RowIterator(tmp);
}

IterableSquareMatrix::ColumnIterator
IterableDenseMatrix::column_end(size_t column_) const
{
    Info tmp(mtx_size, column_, mtx_size, this, 0);
    return IterableSquareMatrix::ColumnIterator(tmp);
}
//-------------------------------------------------------------------------
double
IterableDenseMatrix::next_elem_row(size_t row_, size_t &column_) const
{
    for (size_t i = column_ + 1; i < mtx_size; ++i) {
        if (abs(get(row_, i)) > eps) {
            column_ = i;
            return get(row_, i);
        }
    }
    column_ = mtx_size;
    return 0;
}

double
IterableDenseMatrix::next_elem_column(size_t &row_, size_t column_) const
{
    for (size_t i = row_ + 1; i < mtx_size; ++i) {
        if (abs(get(i, column_)) > eps) {
            row_ = i;
            return get(i, column_);
        }
    }
    row_ = mtx_size;
    return 0;
}
//---------------------------------------------------------------------