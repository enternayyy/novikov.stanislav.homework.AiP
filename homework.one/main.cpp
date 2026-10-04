#include <iostream>

int main() {
    int rows = 0;
    int cols = 0;

    if (!(std::cin >> rows >> cols)) {
        return 1;
    }
    if (rows <= 0 || cols <= 0) {
        return 1;
    }

    int** matrix = new int*[rows];
    for (int i = 0; i < rows; ++i) {
        matrix[i] = new int[cols];
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!(std::cin >> matrix[i][j])) {
                for (int k = 0; k < rows; ++k) {
                    delete[] matrix[k];
                }
                delete[] matrix;
                return 1;
            }
        }
    }

    int** transposed = new int*[cols];
    for (int i = 0; i < cols; ++i) {
        transposed[i] = new int[rows];
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            transposed[j][i] = matrix[i][j];
        }
    }

    for (int i = 0; i < cols; ++i) {
        for (int j = 0; j < rows; ++j) {
            std::cout << transposed[i][j];
            if (j < rows - 1) {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }

    for (int i = 0; i < rows; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;

    for (int i = 0; i < cols; ++i) {
        delete[] transposed[i];
    }
    delete[] transposed;

    return 0;
}
