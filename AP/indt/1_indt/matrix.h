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

void inArray(int** arr, const int &size, std::istream& stream) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            stream >> arr[i][j];

            if ((stream.eof())) {
                throw std::runtime_error("ERROR: Not enough matrix elements");
            }

            if (!(stream)) {
                throw std::runtime_error("ERROR: Invalid matrix element");
            }
        }
    }
}

void outArray(int** arr, const int &size, std::ostream& stream) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            stream << std::setw(3) << arr[i][j];
        }
        stream << "\n";
    }
}

#endif /* MATRIX_H */