#include <vector>
#include <iostream>
#include <iomanip>
#include "Console.h"

using namespace std;

namespace utils {

    void Console::printVector(vector<int> const &vect) {
        for (int i = 0; i < vect.size(); ++i) {
            cout << "[" << i+1 << "]" << ": " << vect[i] << "; ";
            if (i > 100) {
                cout << " ... " << vect.size() - i << " other items";
                break;
            }
        }
        cout << endl;
    }

    void Console::printMatrix(vector<vector<int> > const &matrix) {
        for (int i = 0; i < matrix.size(); ++i) {
            if (i == 0) {
                int margin = 10;
                for (int j = 0; j < matrix[0].size(); j++) {
                    if (j > 0) margin = 6;
                    cout << setw(margin) << "[" << j+1 << "]";
                }
                cout << endl;
            }
            cout << "[" << i+1 << "]" << setw(4);
            for (int j = 0; j < matrix[i].size(); ++j) {
                cout << setw(8) << matrix[i][j];
            }
            cout << endl;
        }
    }

}
