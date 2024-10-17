#include <iostream>
using namespace std;

void generate_factors(int limit, long long current_value = 1, int k = 0, int l = 0, int m = 0) {
    if (current_value > limit)
        return;

    cout << current_value << endl; // ������� ������� ��������

    // ���������� ���������� ��������� ��������
    generate_factors(limit, current_value * 3, k + 1, l, m);  // �������� �� 3
    generate_factors(limit, current_value * 5, k, l + 1, m);  // �������� �� 5
    generate_factors(limit, current_value * 7, k, l, m + 1);  // �������� �� 7
}

void solve(int x) {
    generate_factors(x);
}

int main() {
    setlocale(LC_ALL, "Russian");
    int x;
    cout << "������� ����� x: ";
    cin >> x;
    solve(x);
    return 0;
}
