////2.1.1
//#include <iostream>
//#include <vector>
//#include <algorithm>
//
//using namespace std;
//
//bool areDigitsUnique(int n) {
//    if (n < 10) {
//        return true; // однозначное число всегда имеет уникальные цифры
//    }
//
//    vector<int> digits;
//
//    while (n > 0) {
//        int digit = n % 10;
//        digits.push_back(digit);
//        n /= 10;
//    }
//
//    // Сортируем цифры 
//    sort(digits.begin(), digits.end());
//
//    for (size_t i = 0; i < digits.size() - 1; ++i) {
//        if (digits[i] == digits[i + 1]) {
//            return false;
//        }
//    }
//
//    return true;
//}
//
//int main() {
//    int n;
//    cout << "Enter n: ";
//    cin >> n;
//
//    if (areDigitsUnique(n)) {
//        cout << "are unique." << endl;
//    }
//    else {
//        cout << "are not unique." << endl;
//    }
//
//    return 0;
//}

////2.1.2
//#include <iostream>
//#include <vector>
//#include <cmath>
//#include <sstream>
//#include <algorithm>
//
//using namespace std;
//
//int sumDigitsAfterN(double number, int n, int k) {
//    double fractional = number - floor(number);
//
//    // Преобразуем дробную часть в строку
//    ostringstream oss;
//    oss << fractional;
//    string str = oss.str();
//
//    if (str.size() >= 2 && str[0] == '0' && str[1] == '.') {
//        str = str.substr(2);
//    }
//
//    // Преобразуем строку цифр в вектор чисел
//    vector<int> digits;
//    for (char c : str) {
//        if (isdigit(c)) {
//            digits.push_back(c - '0');
//        }
//    }
//
//    // Проверяем, что n не выходит за границы вектора
//    if (n >= digits.size()) {
//        return 0;
//    }
//
//    // Вычисляем сумму k цифр, начиная с позиции n
//    int sum = 0;
//    for (int i = n; i < n + k && i < digits.size(); ++i) {
//        sum += digits[i];
//    }
//
//    return sum;
//}
//
//int main() {
//    double num;
//    int n, k;
//
//    cout << "Enter: ";
//    cin >> num;
//
//    cout << "enter n: ";
//    cin >> n;
//
//    cout << "enter k: ";
//    cin >> k;
//
//    int result = sumDigitsAfterN(num, n, k);
//    cout << "sum: " << result << endl;
//
//    return 0;
//}
//2.2
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

// Вычисляет 1/(1+x) с помощью ряда Тейлора (1 - x + x^2 - x^3 + ...)
// Возвращает {результат, количество слагаемых}
pair<double, int> taylor_series(double x, double epsilon = 1e-6) {
    if (abs(x) >= 1.0) {
        throw invalid_argument("Ряд Тейлора для 1/(1+x) сходится только при |x| < 1");
    }

    double sum = 0.0;
    double term = 1.0; // Первый член ряда: (-x)^0 = 1
    int n = 0;

    while (abs(term) > epsilon) {
        sum += term;
        n++;
        term = pow(-x, n); // Следующий член: (-x)^n
    }

    return make_pair(sum, n);
}

// Точное значение функции y = 1/(1+x) + (x - 1)
double exact_value(double x) {
    return (1.0 / (1.0 + x)) + (x - 1.0);
}

int main() {
    const double x0 = -0.5;
    const double xn = 0.5;
    const double delta_x = 0.1;
    const double epsilon = 1e-6;

    vector<double> x_values;       // Точки x_i
    vector<double> taylor_results; // Результаты ряда Тейлора
    vector<double> exact_results;  // Точные значения
    vector<int> terms_used;        // Количество слагаемых

    
    for (double x = x0; x <= xn + 1e-9; x += delta_x) {
        try {
            pair<double, int> taylor_data = taylor_series(x, epsilon);
            double taylor_sum = taylor_data.first;
            int num_terms = taylor_data.second;

            double y_taylor = taylor_sum + (x - 1.0);
            double y_exact = exact_value(x);

            x_values.push_back(x);
            taylor_results.push_back(y_taylor);
            exact_results.push_back(y_exact);
            terms_used.push_back(num_terms);
        }
        catch (const invalid_argument& e) {
            cerr << "Ошибка при x = " << x << ": " << e.what() << endl;
        }
    }

    cout << fixed << setprecision(6);
    cout << "   x    |  Taylor (y) | computer (y) | number terms | difference\n";
    cout << "---------|-------------|----------------|-----------|---------\n";

    for (size_t i = 0; i < x_values.size(); ++i) {
        double diff = abs(taylor_results[i] - exact_results[i]);
        cout << setw(7) << x_values[i] << " | "
            << setw(11) << taylor_results[i] << " | "
            << setw(14) << exact_results[i] << " | "
            << setw(9) << terms_used[i] << " | "
            << setw(7) << diff << "\n";
    }

    return 0;
}