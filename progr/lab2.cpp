#include <iostream>
#include <cmath>
#include <iomanip>  // ��� �������������� ������
#include <locale>   // ��� setlocale
#include <string>

struct Answer {
    double root;       // ��������� ������
    int iterations;    // ���������� ��������
    double precision;  // �������� ����������
};

// ������ �������
double func(double x) {
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

// ����� �������
Answer secant(double L, double R, double eps) {
    Answer answer;
    answer.iterations = 0;
    double M;

    while (true) {
        M = R - func(R) * (R - L) / (func(R) - func(L));  // ������� �������
        answer.iterations++;

        if (fabs(func(M)) < eps) {  // �������� ��������
            break;
        }

        if (func(M) < 0) {
            L = M;
        }
        else {
            R = M;
        }
    }

    answer.root = M;
    answer.precision = fabs(func(M));
    return answer;
}

int main() {
    setlocale(LC_ALL, "Russian");  // ������������� �����������

    double eps[] = { 0.1, 0.01, 0.0001 };  // ������ �������� ��������
    double L_init = -10;  // ����� �������
    double R_init = 10;   // ������ �������

    std::cout << std::fixed << std::setprecision(6);  // ��������� �������� ������
    const int col_width = 28;  // ������ ������ �������

    // ������� �����������
    std::cout << "|" << std::setw(col_width) << " "
        << "|" << std::setw(col_width) << "E=0.1"
        << "|" << std::setw(col_width) << "E=0.01"
        << "|" << std::setw(col_width) << "E=0.0001" << "|\n";

    std::cout << "|" << std::setw(col_width) << " "
        << "|" << std::setw(col_width) << "---------------"
        << "|" << std::setw(col_width) << "---------------"
        << "|" << std::setw(col_width) << "---------------" << "|\n";

    // ����� ����������� �������
    std::cout << "|" << std::setw(col_width) << "����� ���. ���." << "|";
    for (double e : eps) {
        Answer result_bis = bisection(L_init, R_init, e);
        std::cout << std::setw(col_width) << "M1=" + std::to_string(result_bis.root)
            + ", k1=" + std::to_string(result_bis.iterations)
            + ", Ep1=" + std::to_string(result_bis.precision) << "|";
    }
    std::cout << "\n";

    // ����� �������
    std::cout << "|" << std::setw(col_width) << "����� ���." << "|";
    for (double e : eps) {
        Answer result_sec = secant(L_init, R_init, e);
        std::cout << std::setw(col_width) << "M2=" + std::to_string(result_sec.root)
            + ", k2=" + std::to_string(result_sec.iterations)
            + ", Ep2=" + std::to_string(result_sec.precision) << "|";
    }
    std::cout << "\n";

    // ��������� k1/k2
    std::cout << "|" << std::setw(col_width) << " " << "|";
    for (double e : eps) {
        Answer result_bis = bisection(L_init, R_init, e);
        Answer result_sec = secant(L_init, R_init, e);
        double k_ratio = (double)result_bis.iterations / result_sec.iterations;
        std::cout << std::setw(col_width) << "k1/k2=" + std::to_string(k_ratio) << "|";
    }
    std::cout << "\n";

    return 0;
}
