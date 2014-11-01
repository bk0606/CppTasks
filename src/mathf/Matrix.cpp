#include <assert.h>
#include "Matrix.h"

namespace mathf {

    void Matrix::assignData(int *dataArray, unsigned rowsCount, unsigned colsCount) {
        colsCnt = colsCount;
        rowsCnt = rowsCount;
        elemsCnt = colsCnt * rowsCnt;
        _data = dataArray;
    }

    Matrix& Matrix::operator= (Matrix rightMtx) {
        if (this == &rightMtx) {
            return *this;
        }
        colsCnt = rightMtx.colsCnt;
        rowsCnt = rightMtx.rowsCnt;
        elemsCnt = rightMtx.elemsCnt;
        _data = &rightMtx(0, 0);
        return *this;
    }

    Matrix Matrix::operator+ (const Matrix& withMtx) const {
        assert(withMtx.rowsCnt == rowsCnt && withMtx.colsCnt == colsCnt);
        Matrix resultMtx(rowsCnt, colsCnt);
        // TODO: May be parallel
        for (unsigned row = 0; row < rowsCnt; ++row) {
            for (unsigned col = 0; col < colsCnt; ++col) {
                resultMtx(row, col) = _data[colsCnt * row + col] + withMtx(row, col);
            }
        }
        return resultMtx;
    }

    Matrix Matrix::operator* (const Matrix& toMtx) const {
        assert(colsCnt == toMtx.rowsCnt);
        Matrix resultMtx = Matrix(rowsCnt, toMtx.colsCnt);
        // TODO: May be parallel
        for (unsigned aRow = 0; aRow < rowsCnt; ++aRow) {
            for (unsigned bCol = 0; bCol < toMtx.colsCnt; ++bCol) {
                for (unsigned bRow = 0; bRow < toMtx.rowsCnt; ++bRow) {
                    resultMtx(aRow, bCol) += _data[colsCnt * aRow + bRow] * toMtx(bRow, bCol);
                }
            }
        }
        return resultMtx;
    }

    int& Matrix::operator() (unsigned rowIndex, unsigned colIndex) {
        assert(rowIndex < rowsCnt && colIndex < colsCnt);
        return _data[colsCnt * rowIndex + colIndex];
    }

    int Matrix::operator() (unsigned rowIndex, unsigned colIndex) const {
        assert(rowIndex < rowsCnt && colIndex < colsCnt);
        return _data[colsCnt * rowIndex + colIndex];
    }

    Matrix::Matrix(int *dataArray, unsigned rowsCnt, unsigned colsCnt)
            : rowsCnt(rowsCnt), colsCnt(colsCnt) {
        elemsCnt = rowsCnt * colsCnt;
        _data = dataArray;
    }

    Matrix::Matrix(unsigned rowsCnt, unsigned colsCnt)
            : rowsCnt(rowsCnt), colsCnt(colsCnt) {
        elemsCnt = rowsCnt * colsCnt;
        _data = new int[elemsCnt];
    }

    Matrix::Matrix(const Matrix &copiedMtx) {
        rowsCnt = copiedMtx.rowsCnt;
        colsCnt = copiedMtx.colsCnt;
        _data = copiedMtx._data;
    }

    Matrix::Matrix() : rowsCnt(0), colsCnt(0), elemsCnt(0) {}

    Matrix::~Matrix() {}
}