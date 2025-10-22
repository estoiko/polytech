#include <iostream>
#include <exception>
#include <iomanip>
#include <cmath>
#include <fstream>

#include "matrix.h"

void allocMatrix(int** (&dMatrix), const int& size);
void freeMatrix(int** (&dMatrix), const int& size);
void inMatrix(int** arr, const int &size, std::istream &stream);
void outMatrix(const int* const * arr, const int &size, std::ostream &stream);

int maxLenSubArray(const int* const * dMatrix, const int& size) {
    int* lenCount = new int [size] {-1};

    for (int i = 0; i < size; ++i) {
        int currentMaxSubLen = 1;
        for (int j = 0; j < size - 1; ++j) {
            if (dMatrix[i][j] == dMatrix[i][j + 1]) {
                ++currentMaxSubLen;
            } else {
                lenCount[i] = std::max(lenCount[i], currentMaxSubLen);
                currentMaxSubLen = 1;
            }
        }
        lenCount[i] = std::max(lenCount[i], currentMaxSubLen);

        if (lenCount[i] == size) {
            break;
        }
    }

    int maxLenCount = 0;
    for (int i = 0; i < size; ++i) {
        maxLenCount = std::max(maxLenCount, lenCount[i]);
    }

    int maxLineNum = 0;
    for (int i = 0; i < size; ++i) {
        if (lenCount[i] == maxLenCount) {
            maxLineNum = i + 1;
            break;
        }
    }

    delete[] lenCount;
    return maxLineNum;
}

int main() {
    int** matrix = nullptr;
    int mSize = 0;

    try {
        std::ifstream inFile("in.txt");
        if (!inFile) {
            throw std::runtime_error("ERROR: file cannot be opened");
        }

        inFile >> mSize;
        allocMatrix(matrix, mSize);
        inMatrix(matrix, mSize, inFile);
        inFile.close();

        std::ofstream outFile("out.txt");
        outFile << maxLenSubArray(matrix, mSize) << "\n";
        outFile.close();

    } catch (const std::runtime_error &ex) {
        std::cerr << ex.what() << "\n";
    } catch (const std::invalid_argument &ex) {
        std::cerr << ex.what() << "\n";
    } catch (const std::bad_alloc &ex) {
        std::cerr << "ERROR: Memory is not allocated" << "\n";
    } catch (...) {
        std::cerr << "ERROR: Unknown\n";
    }
    
    if (matrix != nullptr) {
        freeMatrix(matrix, mSize);
        matrix = nullptr;
    }

    return 0;
}