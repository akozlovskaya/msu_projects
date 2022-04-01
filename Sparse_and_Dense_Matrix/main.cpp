#include "IterableDenseMatrix.hpp"
#include "IterableSparseMatrix.hpp"
#include "GraphAlgorithms.hpp"
#include <set>
#include <iterator>
#include <iostream>

using namespace std;

int main() {
    
    IterableSparseMatrix A(600);
    
    A[16][2] = 0.00007;
    A[16][33] = 0.00003;
    A[2][16] = 0.00007;
    A[33][16] = 0.00003;
    
    A[6][16] = 0.00003;
    A[6][33] = 0.00005;
    A[6][2] = 0.00005;
    
    A[0][6] = 0.00002;
    A[6][0] = 0.00002;
    
    IterableSparseMatrix::RowIterator rit = A.iterate_rows(6);
    cout << "6th row:\n";
    while (rit != A.row_end(6)) {
        cout << rit->val << " " << rit->column << " | ";
        ++rit;
    }
    cout << "\n";
    
    cout << "\nSparce (size = 600)\nvertices with the max number of edges: ";
    set <unsigned> myset = max_edges(&A);
    
    copy(myset.begin(), myset.end(), ostream_iterator<int>(cout, " "));
    cout << '\n';
    
    cout << "\nmax cliques:\n";
    set <set <unsigned>> myset_2 = max_cliques(&A);
    
    set <set <unsigned>>::iterator it;
    for (it = myset_2.begin(); it != myset_2.end(); ++it) {
        copy((*it).begin(), (*it).end(), ostream_iterator<int>(cout, " "));
        cout << "\n";
    }
//-------------------------------------------------------------------------//   
    IterableDenseMatrix B(100);
    for (unsigned i = 0; i < B.size(); i += 2) {
        for (unsigned j = 0; j < B.size(); j += 2) {
            if (i != j) B[i][j] = i + j;
        }
    }
    cout << "\n\nDense (size = 100)\nvertices with the max number of edges: ";
    set <unsigned> res_set = max_edges(&B);
    
    copy(res_set.begin(), res_set.end(), ostream_iterator<int>(cout, " "));
    cout << '\n';
    
    cout << "\nmax cliques:\n";
    set <set <unsigned>> res_set_2 = max_cliques(&B);
    
    for (it = res_set_2.begin(); it != res_set_2.end(); ++it) {
        copy((*it).begin(), (*it).end(), ostream_iterator<int>(cout, " "));
        cout << "\n\n";
    }
    
    return 0;
}