#include "DenseVector.hpp"

double DenseVector::eps = 0.000001;


/******DenseVector constructors and destructor******/

DenseVector::DenseVector() : columns(0), row(0), data(nullptr)
{ 
    cur_column = new size_t(0);
}

DenseVector::DenseVector(size_t row_, size_t columns_)
    : columns(columns_), row(row_)
{
    cur_column = new size_t(0);
    data = new double[columns];
    for(size_t i = 0; i < columns; ++i) {
        data[i] = 0;
    }
}

DenseVector::DenseVector(const DenseVector &src)
    : columns(src.columns), row(src.row)
{
    data = new double[columns];
    memcpy(data, src.data, columns * sizeof(data[0]));
    cur_column = new size_t(*src.cur_column);
}

DenseVector::~DenseVector()
{
    if (data != nullptr) delete []data;
    delete cur_column;
}

/******DenseVector operators******/

double
DenseVector::operator[](size_t column) const
{
    if (column >= columns) {
        throw std::out_of_range{"DenseVector::operator[]"};
        return 0;
    }
    return data[column];
}

double&
DenseVector::operator[](size_t column)
{
    if (column >= columns) {
        throw std::out_of_range{"DenseVector::operator[]"};
    }
    return data[column];
}

DenseVector&
DenseVector::operator=(const DenseVector &src)
{
    if (data == nullptr) {
        data = new double[src.columns];
    } else if (columns != src.columns) {
        delete []data;
        data = new double[src.columns];
    }
    row = src.row;
    columns = src.columns;
    memcpy(data, src.data, columns * sizeof(data[0]));
    return *this;
}

double
DenseVector::operator*() const
{
    if (*cur_column >= columns) {
        throw std::out_of_range{"DenseVector::operator*"};
    }
    size_t clm = *cur_column;
    *cur_column = 0;
    return data[clm];
}

double&
DenseVector::operator*()
{
    if (*cur_column >= columns) {
        throw std::out_of_range{"DenseVector::operator*"};
    }
    size_t clm = *cur_column;
    *cur_column = 0;
    return data[clm];
}

const DenseVector&
DenseVector::operator+(const size_t shft) const
{
    if (*cur_column + shft >= columns) {
        if (columns > 0) throw std::out_of_range{"DenseVector::operator+"};
    }
    *cur_column += shft;
    return *this;
}

DenseVector&
DenseVector::operator+(const size_t shft)
{
    if (*cur_column + shft >= columns) {
        if (columns > 0) throw std::out_of_range{"DenseVector::operator+"};
    }
    *cur_column += shft;
    return *this;
}