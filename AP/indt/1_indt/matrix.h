#ifndef MATRIX_H
#define MATRIX_H

void allocMatrix(int** (&dArray), const int& size) {
    if (size <= 0) {
        throw std::invalid_argument("ERROR: Invalid size specified");
    }

    dArray = new int* [size] {nullptr};
    for (int i = 0; i < size; ++i) {
        dArray[i] = new int [size] {0};
    }
}

void freeMatrix(int** (&dArray), const int& size) {
    for (int i = 0; i < size; ++i) {
        delete[] dArray[i];
    }
    delete[] dArray;
    dArray = nullptr;
}

void inArray(int** arr, const int &size, const char* filename) {
    std::ifstream file(filename);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            file >> arr[i][j];

            if ((file.eof())) {
                throw std::runtime_error("ERROR: Not enough matrix elements");
            }

            if (!(file)) {
                throw std::runtime_error("ERROR: Invalid matrix element");
            }
        }
    }

    file.close();
}

void outArray(int** arr, const int &size, const char* filename) {
    std::ofstream file(filename);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            file << std::setw(3) << arr[i][j];
        }
        file << "\n";
    }

    file.close();
}

#endif /* MATRIX_H */