#include <iostream>
#include <vector>

using namespace std;

namespace parallel_tasks {

    class Matrix {
    public:
        void Sum(Matrix &matrixB) {
            if (rowsCount_ != matrixB.getRowsCount() || colsCount_ != matrixB.getColsCount()) {
                // TODO: Throw exception
            };
            for (int i = 0; i < rowsCount_; ++i) {
                for (int j = 0; j < colsCount_; ++j) {
                    this->setItem(i, j, this->getItem(i, j) + matrixB.getItem(i, j));
                }
            }
        };

        void Multiply(Matrix &matrix) {
            // TODO: Realize
        };

        int getItem(int row, int col) {
            return matrix_[row][col];
        }
        void setItem(int row, int col, int value) {
            matrix_[row][col] = value;
        }

        int getRowsCount() const {
            return rowsCount_;
        }

        int getColsCount() const {
            return colsCount_;
        }

        vector<int> getRow(int number) {
            return matrix_[number];
        }

        Matrix(unsigned int rowsCount, unsigned int colsCount) {
            rowsCount_ = rowsCount;
            colsCount_ = colsCount;
            matrix_ = vector< vector<int> > (rowsCount, vector<int>(colsCount, 0));
        }

        ~Matrix() {
            matrix_.clear();
        }
    private:
        vector< vector<int> > matrix_;
        int rowsCount_, colsCount_;
    };

}