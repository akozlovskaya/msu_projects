#include "IterableDenseMatrix.hpp"
#include "IterableSparseMatrix.hpp"
#include "GraphAlgorithms.hpp"
#include <set>
#include <iterator>
#include <iostream>

using namespace std;

int main() {
    
    IterableSparseMatrix B(300);
    for (unsigned i = 0; i < B.size(); ++i) {
        for (unsigned j = 0; j < B.size(); ++j) {
            if (i != j) B[i][j] = i + j;
        }
    }
    
    cout << "Using a sparse matrix (size = 300) when a dense matrix is needed\n";
    cout << "vertices with the max number of edges: ";
    
    set <unsigned> res_set = max_edges(&B);
    /*copy(res_set.begin(), res_set.end(), ostream_iterator<int>(cout, " "));
    cout << '\n';
    
    cout << "\nmax cliques:\n";
    */
    set <set <unsigned>> res_set_2 = max_cliques(&B);
    /*set <set <unsigned>>::iterator it;
    for (it = res_set_2.begin(); it != res_set_2.end(); ++it) {
        copy((*it).begin(), (*it).end(), ostream_iterator<int>(cout, " "));
        cout << "\n";
    }*/
    
    return 0;
}