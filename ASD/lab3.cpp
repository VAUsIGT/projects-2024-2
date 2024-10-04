#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

int main() {
    setlocale(LC_ALL, "Russian");
    long long x;

    std::cout << "¬ведите число x: ";
    std::cin >> x;

    std::vector<long long> results;

    for (int k = 0; std::pow(3, k) <= x; ++k) {
        long long pow3 = std::pow(3, k);
        for (int l = 0; pow3 * std::pow(5, l) <= x; ++l) {
            long long pow5 = pow3 * std::pow(5, l);
            for (int m = 0; pow5 * std::pow(7, m) <= x; ++m) {
                long long value = pow5 * std::pow(7, m);
                if (value <= x) {
                    results.push_back(value);
                }
                else {
                    break;
                }
            }
        }
    }

    // —ортируем и удал€ем дубликаты, так как 3^0 * 5^0 * 7^0 = 1 может встречатьс€ несколько раз
    std::sort(results.begin(), results.end());
    results.erase(std::unique(results.begin(), results.end()), results.end());

    std::cout << "„исла, удовлетвор€ющие условию, от 1 до " << x << ": ";
    for (auto value : results) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    return 0;
}
