#include "GraphAlgorithms.hpp"

//it is assumed that the graph does not contain loops and multiple edges

std::set <unsigned> 
max_edges (const IterableSquareMatrix *graph)
{
    std::set <unsigned> vertices;
    size_t max_edges_num = 0;
    
    for (unsigned i = 0; i < graph->size(); ++i) {
        
        size_t edges_num = 0;
        
        IterableSquareMatrix::RowIterator itrw = graph->iterate_rows(i);
        while (itrw != graph->row_end(i)) {
            ++edges_num;
            ++itrw;
        }
        
        IterableSquareMatrix::ColumnIterator itclm = graph->iterate_columns(i);
        while (itclm != graph->column_end(i)) {
            ++edges_num;
            ++itclm;
        }
        
        if (edges_num < max_edges_num) continue;
  
        if (edges_num > max_edges_num) {
            max_edges_num = edges_num;
            vertices.clear();
        }
        vertices.insert(i);
    }
    
    return vertices;
}
 
bool
check( std::set <unsigned> cand,
       std::set <unsigned> wrong,
       const IterableSquareMatrix *graph
     )
{
    std::set<unsigned>::iterator itwrg, itcnd;
    
    for (itwrg = wrong.begin(); itwrg != wrong.end(); ++itwrg) {
        
        bool connected = true;
        
        for (itcnd = cand.begin(); itcnd != cand.end(); ++itcnd) {
            
            bool back_connected = false;
            
            IterableSquareMatrix::RowIterator itrow = graph->iterate_rows(*itcnd);
            while (itrow != graph->row_end(*itcnd)) {
                if (itrow->column == *itwrg) {
                    back_connected = true;
                    break;
                }
                ++itrow;
            }
            if (!back_connected) connected = false;
        }
        if (connected) return false;
    }
    return true;
}

void
extend( std::set <unsigned> &compsub,
        std::set <unsigned> &candidates,
        std::set <unsigned> &wrong,
        std::set <std::set <unsigned>> &res,
        const IterableSquareMatrix *graph,
        size_t &max_size
      )
{   
    std::set <unsigned> new_candidates, new_wrong;
    std::set<unsigned>::const_iterator iterset;
    
    while (candidates.size() != 0 && check(candidates, wrong, graph)) {
        
        iterset = candidates.begin();
        unsigned vertex = *iterset;
        compsub.insert(vertex);
        
        new_candidates.clear();
        new_wrong.clear();
        
        IterableSquareMatrix::RowIterator itrow = graph->iterate_rows(vertex);
        
        while (itrow != graph->row_end(vertex)) {
            
            iterset = candidates.find(itrow->column);
            if (iterset != candidates.end()) {
                new_candidates.insert(itrow->column);
            }
            
            iterset = wrong.find(itrow->column);
            if (iterset != wrong.end()) {
                new_wrong.insert(itrow->column);
            }
            
            ++itrow;
        }
        
        if (!new_candidates.size() && !new_wrong.size()) {
            if (compsub.size() == max_size) {
                res.insert(compsub);
            } else if (compsub.size() > max_size) {
                max_size = compsub.size();
                res.clear();
                res.insert(compsub);
            }
        } else {
            extend(compsub, new_candidates, new_wrong, res, graph, max_size);
        }
        
        iterset = candidates.find(vertex);
        if (iterset != candidates.end()) {
            candidates.erase(iterset);
        }
        
        iterset = compsub.find(vertex);
        if (iterset != compsub.end()) {
            compsub.erase(iterset);
        }
        
        wrong.insert(vertex);
    }
}

std::set <std::set <unsigned>>
max_cliques(const IterableSquareMatrix *graph)
{
    std::set <unsigned> compsub, wrong, candidates;
    std::set <std::set <unsigned>> result;
    
    for (unsigned vertex = 0; vertex < graph->size(); ++vertex) {
        candidates.insert(vertex);
    }
    size_t max_size = 0;
    extend(compsub, candidates, wrong, result, graph, max_size);
    
    return result;
}