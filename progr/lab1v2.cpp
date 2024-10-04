#include <iostream>
#include <cmath>
#include <string>

#define M_PI

double L = -10;
double R = L;
double M;
double E = 0.01;
int iter = 0;
int lowhigh = 0;

int determ();
void zone();
double funct(double x);

struct Result {
    double M;    // ��������� �������� �����
    int k;       // ���-�� ��������
    double Ep;   // �������� ������� � ��������� ����� (�������)
};

Result calcul();

int main() {
    determ();
    zone();
    Result result = calcul();
    std::cout << "M = " << result.M << "; k = " << result.k << "; Ep = " << result.Ep << std::endl;
    return 0;
}

int determ() { //�������� R �� ����. ����� � ����� 0.5
    if (R < 0) {
        while (R <= 0) { R += 0.5; }
    }
    else {
        while (R >= 0) { R -= 0.5; }
    }
    return R;
}

void zone() { //�������� �� ������� ����� ������� � �������� ��������
    if (funct(L) > funct(R)) { //���� ������� �������
        lowhigh = 1;
        while (funct(L) < 0 || funct(R) > 0) { //���� ������ ��� ������
            if (funct(L) < 0) { //���� ������ �� � ����� �������
                L -= 0.5;
            }
            if (funct(R) > 0) { //���� ������ �� � ������ �������
                R += 0.5;
            }
        }
    }
    else { //������� ����������
        while (funct(L) > 0 || funct(R) < 0) { //���� ������ ��� ������
            if (funct(L) > 0) { //���� ������ �� � ����� �������
                L -= 0.5;
            }
            if (funct(R) < 0) { //���� ������ �� � ������ �������
                R += 0.5;
            }
        }
    }
}

Result calcul() { //������� M
    Result result;
    M = (L + R) / 2;
    while (abs(funct(M)) > E) { //���� ������ ����������� 
        M = (L + R) / 2;
        if (funct(M) < 0) { //���� �� ����� �� ���� �� ����� �������
            if (lowhigh) {
                R = M; //������� �������, ��� ���������
            }
            else {
                L = M; //��� ������������
            }
        }
        else { //���� �� ����� �� ������ �������
            if (lowhigh) {
                L = M;
            }
            else {
                R = M;
            }
        }
        iter += 1;
    }

    result.M = M;
    result.k = iter;  // ���-�� ��������
    result.Ep = funct(M);  // �������� ������� � ��������� ����� (�����������)

    return result;
}

double funct(double x) {
    return cos(2 * x + 3.1415 / 2) + x + 8; //������� 1, ������ � -7.9 (����������)
    //return 0.7 * (sin(x / 2) + cos(2 * x)) - x; //������� 4, ������ � 0.5 (�������)
}