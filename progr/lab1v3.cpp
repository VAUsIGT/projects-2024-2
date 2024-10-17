#include <iostream>
#include <cmath>
#define M_PI

struct Answer {
    double root;       // ��������� ������
    int iterations;    // ���������� ��������
    double precision;  // �������� ����������
};

// ������ �������. ��������� �� ���� ������� �� �������.
double func(double x) {
    //return cos(2 * x + 3.1415 / 2) + x + 8;
    return 0.7 * (sin(x / 2) + cos(2 * x)) - x;
}

// ����� ����������� ������� (��������)
Answer bisection(double L, double R, double eps) {
    Answer answer;
    answer.iterations = 0;

    double M;
    while ((R - L) / 2 > eps) {
        M = (L + R) / 2;
        answer.iterations++;

        if (func(M) == 0.0) {
            break;  // M � ��� ������
        }
        else if (func(L) * func(M) < 0) {
            R = M;
        }
        else {
            L = M;
        }
    }

    answer.root = M;
    answer.precision = fabs(func(M));

    return answer;
}

int main() {
    double L = -10;  // ����� �������
    double R = 10;   // ������ �������
    double eps = 0.001;  // ��������

    Answer result = bisection(L, R, eps);

    // ����� ������ � ������� {double, int, double}
    std::cout << "{" << result.root << ", " << result.iterations << ", " << result.precision << "}" << std::endl;

    return 0;
}
