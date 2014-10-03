#ifndef HW1_H
#define HW1_H

#include <vector>

using namespace std;

namespace parallel_tasks {

    class HW1 {
    public:
        vector<int> SimpleVectorsProduct(vector<int> &a, vector<int> &b);

        int VectorsProductsSum(vector<int> &a, vector<int> &b);

        vector<vector<int> > MatrixSum(vector<vector<int> > &A, vector<vector<int> > &B);

        vector<vector<int> > MatrixProduct(vector<vector<int> > &A, vector<vector<int> > &B);
    };

}

#endif