#include <iostream>
#include <exception>
#include <iomanip>
#include <cmath>

#include "matrix.h"

void allocMatrix(int** (&dArray), const int& size);
void freeMatrix(int** (&dArray), const int& size);
void inArray(int** arr, const int &size, std::istream& stream);
void outArray(int** arr, const int &size, std::ostream& stream);

int maxLenSubArray(int** (&dArray), const int& size) {
    int* lenCount = new int [size] {-1};

    for (int i = 0; i < size; ++i) {
        int currentMaxSubLen = 1;
        for (int j = 0; j < size - 1; ++j) {
            if (dArray[i][j] == dArray[i][j + 1]) {
                ++currentMaxSubLen;
            } else {
                lenCount[i] = std::max(lenCount[i], currentMaxSubLen);
                currentMaxSubLen = 1;
            }
        }
        lenCount[i] = std::max(lenCount[i], currentMaxSubLen);
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

    int mSize;
    std::cin >> mSize;

    try {
        allocMatrix(matrix, mSize);
        inArray(matrix, mSize, std::cin);

        // 11) Номер строки, в которой находится самая длинная серия подряд идущих равных элементов.
        std::cout << maxLenSubArray(matrix, mSize) << "\n";

        // outArray(matrix, mSize, std::cout);

    } catch (std::runtime_error &ex) {
        std::cerr << ex.what() << "\n";
    } catch (std::invalid_argument &ex) {
        std::cerr << ex.what() << "\n";
    } catch (std::bad_alloc &ex) {
        std::cerr << "ERROR: Memory is not allocated" << "\n";
    } catch (...) {
        std::cerr << "UNKNOWN ERROR\n";
    }
    
    if (matrix != nullptr) {
        freeMatrix(matrix, mSize);
    }

    return 0;
}