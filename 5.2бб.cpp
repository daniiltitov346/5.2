//2.1
//#include <iostream>
//#include <vector>
//#include <unordered_map>
//#include <algorithm>
//#include <cstdlib>
//#include <ctime>
//
//using namespace std;
//
//vector<int> find_repeating_numbers(const vector<int>& sequence) {
//    unordered_map<int, int> count_map;
//    vector<int> result;
//
//    // Первый проход: подсчет количества вхождений
//    for (int num : sequence) {
//        count_map[num]++;
//    }
//
//    // Второй проход: сбор чисел, встречающихся более одного раза
//    for (const auto& pair : count_map) {
//        if (pair.second > 1) {
//            result.push_back(pair.first);
//        }
//    }
//
//    // Сортировка результата
//    sort(result.begin(), result.end());
//
//    return result;
//}
//
//int main() {
//    int n;
//    cout << "enter size (n <= 10^12): ";
//    cin >> n;
//
//    vector<int> sequence(n);
//
//    char choice;
//    cout << "random? (y/n): ";
//    cin >> choice;
//
//    if (choice == 'y' || choice == 'Y') {
//        srand(time(0));
//        for (int i = 0; i < n; ++i) {
//            sequence[i] = rand() % 10000000 + 1; // Числа ≤ 10^7
//        }
//    }
//    else {
//        cout << "enter " << n << " numbers:\n";
//        for (int i = 0; i < n; ++i) {
//            cin >> sequence[i];
//        }
//    }
//
//    // Обработка последовательности
//    vector<int> result = find_repeating_numbers(sequence);
//
//    // Вывод результата
//    cout << "sorted:\n";
//    for (int num : result) {
//        cout << num << " ";
//    }
//    cout << endl;
//
//    return 0;
//}
//2.2
#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <memory>

using namespace std;

// Способ 1: Линейное представление матрицы (одномерный массив)
class MatrixLinear {
private:
    vector<double> data;
    size_t rows, cols;

public:
    MatrixLinear(size_t r, size_t c) : rows(r), cols(c), data(r* c) {}

    double& operator()(size_t i, size_t j) {
        if (i >= rows || j >= cols) throw out_of_range("Matrix indices out of range");
        return data[i * cols + j];
    }

    const double& operator()(size_t i, size_t j) const {
        if (i >= rows || j >= cols) throw out_of_range("Matrix indices out of range");
        return data[i * cols + j];
    }

    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }

    MatrixLinear power(unsigned n) const;
    MatrixLinear operator*(const MatrixLinear& other) const;

    static MatrixLinear identity(size_t size) {
        MatrixLinear m(size, size);
        for (size_t i = 0; i < size; ++i) m(i, i) = 1;
        return m;
    }

    void print() const {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                cout << setw(8) << fixed << setprecision(2) << (*this)(i, j);
            }
            cout << endl;
        }
    }
};

// Способ 2: Массив указателей (двумерный массив)
class MatrixPointer {
private:
    vector<vector<double>> data;

public:
    MatrixPointer(size_t r, size_t c) : data(r, vector<double>(c)) {}

    double& operator()(size_t i, size_t j) {
        if (i >= data.size() || j >= data[0].size()) throw out_of_range("Matrix indices out of range");
        return data[i][j];
    }

    const double& operator()(size_t i, size_t j) const {
        if (i >= data.size() || j >= data[0].size()) throw out_of_range("Matrix indices out of range");
        return data[i][j];
    }

    size_t getRows() const { return data.size(); }
    size_t getCols() const { return data.empty() ? 0 : data[0].size(); }

    MatrixPointer power(unsigned n) const;
    MatrixPointer operator*(const MatrixPointer& other) const;

    static MatrixPointer identity(size_t size) {
        MatrixPointer m(size, size);
        for (size_t i = 0; i < size; ++i) m(i, i) = 1;
        return m;
    }

    void print() const {
        for (const auto& row : data) {
            for (double val : row) {
                cout << setw(8) << fixed << setprecision(2) << val;
            }
            cout << endl;
        }
    }
};

// Реализация методов для MatrixLinear
MatrixLinear MatrixLinear::power(unsigned n) const {
    if (rows != cols) throw invalid_argument("Matrix must be square");
    if (n == 0) return identity(rows);

    MatrixLinear result = *this;
    for (unsigned i = 1; i < n; ++i) {
        result = result * *this;
    }
    return result;
}

MatrixLinear MatrixLinear::operator*(const MatrixLinear& other) const {
    if (cols != other.rows) throw invalid_argument("Incompatible matrix sizes");

    MatrixLinear result(rows, other.cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < other.cols; ++j) {
            double sum = 0;
            for (size_t k = 0; k < cols; ++k) {
                sum += (*this)(i, k) * other(k, j);
            }
            result(i, j) = sum;
        }
    }
    return result;
}

// Реализация методов для MatrixPointer
MatrixPointer MatrixPointer::power(unsigned n) const {
    if (getRows() != getCols()) throw invalid_argument("Matrix must be square");
    if (n == 0) return identity(getRows());

    MatrixPointer result = *this;
    for (unsigned i = 1; i < n; ++i) {
        result = result * *this;
    }
    return result;
}

MatrixPointer MatrixPointer::operator*(const MatrixPointer& other) const {
    if (getCols() != other.getRows()) throw invalid_argument("Incompatible matrix sizes");

    MatrixPointer result(getRows(), other.getCols());
    for (size_t i = 0; i < getRows(); ++i) {
        for (size_t j = 0; j < other.getCols(); ++j) {
            double sum = 0;
            for (size_t k = 0; k < getCols(); ++k) {
                sum += (*this)(i, k) * other(k, j);
            }
            result(i, j) = sum;
        }
    }
    return result;
}

// Демонстрация работы обоих классов
int main() {
    try {
        cout << "=== Linear representation of a matrix ===" << endl;
        MatrixLinear ml(2, 2);
        ml(0, 0) = 1; ml(0, 1) = 2;
        ml(1, 0) = 3; ml(1, 1) = 4;

        cout << "old:" << endl;
        ml.print();

        auto ml_pow = ml.power(3);
        cout << "\nnew (^3):" << endl;
        ml_pow.print();

        cout << "\n=== array of pointers ===" << endl;
        MatrixPointer mp(2, 2);
        mp(0, 0) = 1; mp(0, 1) = 1;
        mp(1, 0) = 1; mp(1, 1) = 0;

        cout << "old:" << endl;
        mp.print();

        auto mp_pow = mp.power(6);
        cout << "\nnew (^6):" << endl;
        mp_pow.print();

    }
    catch (const exception& e) {
        cerr << "error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
