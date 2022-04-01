#include "IterableSquareMatrix.hpp"
using ISQM = IterableSquareMatrix;
//----------------------------------------------------------------------------

ISQM::ColumnIterator::ColumnIterator(const Info& src) : info(src) {}
    
ISQM::ColumnIterator::ColumnIterator(const ISQM::ColumnIterator& src) : info(src.info) {}

ISQM::ColumnIterator::~ColumnIterator() {}

//----------------------------------------------------------------------------

double
ISQM::ColumnIterator::operator*() const { return info.val; }

const Info*
ISQM::ColumnIterator::operator->() const { return &info; }

//----------------------------------------------------------------------------

bool
ISQM::ColumnIterator::operator==(const ISQM::ColumnIterator& cit) const
{
    return (this->info == cit.info);
}

bool
ISQM::ColumnIterator::operator!=(const ISQM::ColumnIterator& cit) const
{
    return !(this->info == cit.info);
}

ISQM::ColumnIterator&
ISQM::ColumnIterator::operator++()
{
    info.val = info.mtx->next_elem_column(info.row, info.column);
    return *this;
}

ISQM::ColumnIterator
ISQM::ColumnIterator::operator++(int)
{
    ISQM::ColumnIterator tmp = *this;
    ++(*this);
    return tmp;
}