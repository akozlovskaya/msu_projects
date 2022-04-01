#include "SparseVector.hpp"


double SparseVector::eps = 0.000001;

/******SparseVector constructors and destructor******/

SparseVector::SparseVector() : max(0), row(0), size(0), cap(0), data(nullptr)
{ 
    cur_column = new size_t(0);
}

SparseVector::SparseVector(size_t row_, size_t max_)
    : max(max_), row(row_), size(0), cap(0), data(nullptr)
{
    cur_column = new size_t(0);
}

SparseVector::SparseVector(const SparseVector &src)
    : max(src.max), row(src.row), size(src.size), cap(src.cap)
{
    data = new Item[cap];
    memcpy(data, src.data, size * sizeof(data[0]));
    cur_column = new size_t(*src.cur_column);
}

SparseVector::~SparseVector()
{
    if (data != nullptr) delete []data;
    delete cur_column;
}

/******SparseVector methods*******/
double
SparseVector::get(size_t column_) const
{
    if (column_ + 1 > max) {
        if (max > 0) throw std::out_of_range{"SparseVector::get"};
    }
    for (size_t j = 0; j < size; ++j) {
        if (data[j].column == column_) {
            return data[j].value;
        }
    }
    return 0.0;
}

void
SparseVector::set(size_t column_, double val_)
{
    if (column_ + 1 > max) {
        if (max > 0) throw std::out_of_range{"SparseVector::set"};
    }
    for (size_t j = 0; j < size; ++j) {
        if (data[j].column == column_) {
            if (abs(val_) > eps) { data[j].value = val_; }
            else {
                for (size_t m = j; m + 1 < size; ++m) {
                    data[m] = data[m+1];
                }
                --size;
            }
            return;
        }
    }
    if (abs(val_) > eps) {
        Item tmp = {.column = column_, .value = val_};
        push_back(tmp);
    }
}

void
SparseVector::push_back(Item& item)
{
    while (size + 1 > cap) {
        cap = cap == 0 ? 2 : cap * 2;
        Item *tmp;
        tmp = new Item[cap];
        if (data != nullptr) {
            memcpy(tmp, data, size * sizeof(data[0]));
            delete []data;
        }
        data = tmp;
    }
    data[size++] = item;
}

/******SparseVector operators******/

double
SparseVector::operator[](size_t column_) const
{
    if (column_ + 1 > max) {
        if (max > 0) throw std::out_of_range{"SparseVector::operator[]"};
        return 0.0;
    }
    for (size_t i = 0; i < size; ++i) {
        if (data[i].column == column_) return data[i].value;
    }
    return 0.0;
}

double&
SparseVector::operator[](size_t column_)
{
    if (column_ + 1 > max) {
        if (max > 0) throw std::out_of_range{"SparseVector::operator[]"};
    }
    for (size_t i = 0; i < size; ++i) {
        if (data[i].column == column_) return data[i].value;
    }
    
    Item tmp = { .column = column_, .value = 0.0 };
    push_back(tmp);
    return data[size - 1].value;
}

SparseVector&
SparseVector::operator=(const SparseVector &src)
{
    if (cap) delete []data;
    cap = src.cap;
    size = src.size;
    row = src.row;
    max = src.max;
    if (cap) {
        data = new Item[cap];
        memcpy(data, src.data, size * sizeof(data[0]));
    } else { data = nullptr; }
    return *this;
}

SparseVector
SparseVector::operator+(const SparseVector &vtr) const
{
    if (max != vtr.max || row != vtr.row) {
        throw std::domain_error{"SparseVector::operator+"};
    }
    SparseVector sum = vtr;
    for (size_t i = 0; i < size; ++i) {
        bool f = false;
        if (abs(data[i].value) > eps) {
            for (size_t j = 0; j < vtr.size; ++j) {
                if (vtr.data[j].column == data[i].column) {
                    sum.data[j].value = vtr.data[j].value + data[i].value;
                    f = true;
                    break;
                }
            }
            if (!f) { sum.push_back(data[i]); }
        }
    }
    return sum;
}

double
SparseVector::operator*() const
{
    if (*cur_column + 1 > max) {
        if (max > 0) throw std::out_of_range{"SparseVector::operator*"};
    }
    for (size_t i = 0; i < size; ++i) {
        if (data[i].column == *cur_column) {
            *cur_column = 0;
            return data[i].value;
        }
    }
    *cur_column = 0;
    return 0.0;
}

double&
SparseVector::operator*()
{
    if (*cur_column  + 1 > max) {
        if (max > 0) throw std::out_of_range{"SparseVector::operator*"};
    }
    for (size_t i = 0; i < size; ++i) {
        if (data[i].column == *cur_column) {
            *cur_column = 0;
            return data[i].value;
        }
    }
    
    Item tmp = { .column = *cur_column, .value = 0.0 };
    push_back(tmp);
    
    *cur_column = 0;
    
    return data[size-1].value;
}

const SparseVector&
SparseVector::operator+(const size_t shft) const
{
    if (*cur_column + shft + 1 > max) {
        if (max > 0) throw std::out_of_range{"SparseVector::operator+"};
    }
    *cur_column += shft;
    return *this;
}

SparseVector&
SparseVector::operator+(const size_t shft)
{
    if (*cur_column + shft + 1 > max) {
        if (max > 0) throw std::out_of_range{"SparseVector::operator+"};
    }
    *cur_column += shft;
    return *this;
}