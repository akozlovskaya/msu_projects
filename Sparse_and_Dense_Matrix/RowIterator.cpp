#include "IterableSquareMatrix.hpp"
using ISQM = IterableSquareMatrix;
//----------------------------------------------------------------------------

ISQM::RowIterator::RowIterator(const Info& src) : info(src) {}

ISQM::RowIterator::RowIterator(const RowIterator& src) : info(src.info) {}

ISQM::RowIterator::~RowIterator() {}

//----------------------------------------------------------------------------

double
ISQM::RowIterator::operator*() const { return info.val; }

const Info*
ISQM::RowIterator::operator->() const { return &info; }

//----------------------------------------------------------------------------

ISQM::RowIterator&
ISQM::RowIterator::operator++()
{
    info.val = info.mtx->next_elem_row(info.row, info.column);
    return *this;
}

ISQM::RowIterator
ISQM::RowIterator::operator++(int)
{
    ISQM::RowIterator tmp = *this;
    ++(*this);
    return tmp;
}

bool
ISQM::RowIterator::operator==(const ISQM::RowIterator& rit) const
{
    return (this->info == rit.info);
}

bool
ISQM::RowIterator::operator!=(const ISQM::RowIterator& rit) const
{
    return !(this->info == rit.info);
}