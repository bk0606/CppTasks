#include <assert.h>
#include <bits/stl_algobase.h>
#include "../utils/Console.h"
#include "Matrix.h"

using namespace utils;

namespace mathf {

    Matrix Matrix::transpose(const Matrix &matrix) {
        unsigned colsCnt = matrix.colsCnt, rowsCnt = matrix.rowsCnt,
                 elemsCnt = matrix.elemsCnt;
        int* tmpData = new int[elemsCnt];
        for (unsigned row = 0; row < rowsCnt; ++row) {
            for (unsigned col = 0; col < colsCnt; ++col) {
                tmpData[col * rowsCnt + row] = matrix(row, col);
            }
        }
        return Matrix(tmpData, colsCnt, rowsCnt);
    }

    void Matrix::assignData(int *dataArray, unsigned rowsCount, unsigned colsCount) {
        colsCnt = colsCount;
        rowsCnt = rowsCount;
        elemsCnt = colsCnt * rowsCnt;
        _data = new int[elemsCnt];
        std::copy(&dataArray[0], &dataArray[elemsCnt], _data);
    }

    int* Matrix::begin() const {
        return &_data[0];
    }

    int* Matrix::end() const {
        return &_data[elemsCnt];
    }

    void Matrix::printToConsole(char const* title) {
        puts(title);
        Console::printMatrix(_data, rowsCnt, colsCnt);
        puts("\n");
    }

    Matrix& Matrix::operator= (const Matrix &rightMtx) {
        if (this == &rightMtx) {
            return *this;
        }
        if (elemsCnt > 0) {
            delete[] _data;
        }
        colsCnt = rightMtx.colsCnt;
        rowsCnt = rightMtx.rowsCnt;
        elemsCnt = rightMtx.elemsCnt;
        int* newData = new int[elemsCnt];
        std::copy(rightMtx.begin(), rightMtx.end(), newData);
        _data = newData;
        return *this;
    }

    Matrix Matrix::operator+ (const Matrix &withMtx) const {
        assert(withMtx.rowsCnt == rowsCnt && withMtx.colsCnt == colsCnt);
        int* resultArray = new int[rowsCnt * colsCnt];
        // TODO: May be parallel
        for (unsigned row = 0; row < rowsCnt; ++row) {
            for (unsigned col = 0; col < colsCnt; ++col) {
                resultArray[row * colsCnt + col] = _data[row * colsCnt + col] + withMtx(row, col);
            }
        }
        return Matrix(resultArray, rowsCnt, colsCnt); // TODO: test it
    }

    Matrix Matrix::operator* (const Matrix &toMtx) const {
        assert(colsCnt == toMtx.rowsCnt);
        int* resultArray = new int[rowsCnt * toMtx.colsCnt];
        // TODO: May be parallel
        for (unsigned aRow = 0; aRow < rowsCnt; ++aRow) {
            for (unsigned bCol = 0; bCol < toMtx.colsCnt; ++bCol) {
                for (unsigned bRow = 0; bRow < toMtx.rowsCnt; ++bRow) {
                    resultArray[aRow * toMtx.colsCnt + bCol] += _data[aRow * colsCnt + bRow] * toMtx(bRow, bCol);
                }
            }
        }
        return Matrix(resultArray, rowsCnt, toMtx.colsCnt); // TODO: test it
    }

    int& Matrix::operator() (unsigned rowIndex, unsigned colIndex) {
        assert(rowIndex < rowsCnt && colIndex < colsCnt);
        return _data[colsCnt * rowIndex + colIndex];
    }

    int Matrix::operator() (unsigned rowIndex, unsigned colIndex) const {
        assert(rowIndex < rowsCnt && colIndex < colsCnt);
        return _data[colsCnt * rowIndex + colIndex];
    }

    Matrix::Matrix(int *dataArray, unsigned rowsCnt, unsigned colsCnt) {
        assignData(dataArray, rowsCnt, colsCnt);
    }

    Matrix::Matrix(unsigned rowsCnt, unsigned colsCnt)
            : rowsCnt(rowsCnt), colsCnt(colsCnt) {
        elemsCnt = rowsCnt * colsCnt;
        _data = new int[elemsCnt];
    }

    Matrix::Matrix(const Matrix &copyingMtx) {
        rowsCnt = copyingMtx.rowsCnt;
        colsCnt = copyingMtx.colsCnt;
        elemsCnt = copyingMtx.elemsCnt;
        _data = new int[elemsCnt];
        std::copy(copyingMtx.begin(), copyingMtx.end(), _data);
    }

    Matrix::~Matrix() {
        if (elemsCnt > 0) {
            delete[] _data;
        }
    }
}