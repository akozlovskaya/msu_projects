#include "IterableDenseMatrix.hpp"
#include "IterableSparseMatrix.hpp"
#include "GraphAlgorithms.hpp"
#include <set>
#include <iterator>
#include <iostream>

using namespace std;

int main() {
    
    IterableSparseMatrix A(600);
    
    A[16][2] = 7;
    A[16][33] = 3;
    A[2][16] = 7;
    A[33][16] = 3;
    
    A[6][16] = 3;
    A[6][33] = 5;
    A[6][2] = 5;
    
    A[0][6] = 2;
    A[6][0] = 2;
    
    cout << "Using a sparse matrix (size = 600) as intended \n";
    cout << "vertices with the max number of edges: ";
    
    set <unsigned> myset = max_edges(&A);
    /*copy(myset.begin(), myset.end(), ostream_iterator<int>(cout, " "));
    cout << '\n';
    
    
    cout << "\nmax cliques:\n";*/
    set <set <unsigned>> myset_2 = max_cliques(&A);
    /*set <set <unsigned>>::iterator it;
    for (it = myset_2.begin(); it != myset_2.end(); ++it) {
        copy((*it).begin(), (*it).end(), ostream_iterator<int>(cout, " "));
        cout << "\n";
    }*/
    return 0;
}