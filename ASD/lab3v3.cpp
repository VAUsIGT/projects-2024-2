#include <iostream>
using namespace std;

void generate_factors(int limit, long long current_value = 1, int k = 0, int l = 0, int m = 0) {
    if (current_value > limit)
        return;

    cout << current_value << endl; // Выводим текущее значение

    // Рекурсивно генерируем следующие значения
    generate_factors(limit, current_value * 3, k + 1, l, m);  // Умножаем на 3
    generate_factors(limit, current_value * 5, k, l + 1, m);  // Умножаем на 5
    generate_factors(limit, current_value * 7, k, l, m + 1);  // Умножаем на 7
}

void solve(int x) {
    generate_factors(x);
}

int main() {
    setlocale(LC_ALL, "Russian");
    int x;
    cout << "Введите число x: ";
    cin >> x;
    solve(x);
    return 0;
}
