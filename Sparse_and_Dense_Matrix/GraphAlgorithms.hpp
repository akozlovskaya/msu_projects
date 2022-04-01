#ifndef GRAPHALG_H_
#define GRAPHALG_H_

#include <set>
#include <iterator>
#include "IterableSquareMatrix.hpp"

std::set <unsigned> max_edges (const IterableSquareMatrix *);
std::set <std::set <unsigned>> max_cliques(const IterableSquareMatrix *);

#endif /* GRAPHALG_H_ */