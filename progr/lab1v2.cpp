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
    double M;    // Найденное значение корня
    int k;       // Кол-во операций
    double Ep;   // Значение функции в найденной точке (эпсилон)
};

Result calcul();

int main() {
    determ();
    zone();
    Result result = calcul();
    std::cout << "M = " << result.M << "; k = " << result.k << "; Ep = " << result.Ep << std::endl;
    return 0;
}

int determ() { //пересчёт R до прот. знака с шагом 0.5
    if (R < 0) {
        while (R <= 0) { R += 0.5; }
    }
    else {
        while (R >= 0) { R -= 0.5; }
    }
    return R;
}

void zone() { //проверка на наличие корня функции в заданных границах
    if (funct(L) > funct(R)) { //если функция убывает
        lowhigh = 1;
        while (funct(L) < 0 || funct(R) > 0) { //пока корень вне границ
            if (funct(L) < 0) { //если корень не в левой границе
                L -= 0.5;
            }
            if (funct(R) > 0) { //если корень не в правой границе
                R += 0.5;
            }
        }
    }
    else { //функция возрастает
        while (funct(L) > 0 || funct(R) < 0) { //пока корень вне границ
            if (funct(L) > 0) { //если корень не в левой границе
                L -= 0.5;
            }
            if (funct(R) < 0) { //если корень не в правой границе
                R += 0.5;
            }
        }
    }
}

Result calcul() { //рассчёт M
    Result result;
    M = (L + R) / 2;
    while (abs(funct(M)) > E) { //пока больше погрешности 
        M = (L + R) / 2;
        if (funct(M) < 0) { //если не дошло до нуля по левой границе
            if (lowhigh) {
                R = M; //двигаем границу, для убывающей
            }
            else {
                L = M; //для возрастающей
            }
        }
        else { //если не дошло по правой границе
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
    result.k = iter;  // Кол-во операций
    result.Ep = funct(M);  // Значение функции в найденной точке (погрешность)

    return result;
}

double funct(double x) {
    return cos(2 * x + 3.1415 / 2) + x + 8; //вариант 1, корень в -7.9 (возрастает)
    //return 0.7 * (sin(x / 2) + cos(2 * x)) - x; //вариант 4, корень в 0.5 (убывает)
}