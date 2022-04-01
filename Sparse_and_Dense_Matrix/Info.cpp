#include "IterableSquareMatrix.hpp"

Info::Info(size_t row_, size_t column_, size_t size_, 
        const IterableSquareMatrix* mtx_, double val_)
    : row(row_), column(column_), size(size_), mtx(mtx_), val(val_) {};

bool
Info::operator==(const Info& info_) const
{
    if (row == info_.row
            && column == info_.column
            && size == info_.size
            && mtx == info_.mtx
            && abs(val - info_.val) < 0.000001
       ) return true;
    return false;
}