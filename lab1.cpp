#include <iostream>
#include <cmath>


#define M_PI

double L = -10;
double R = L;
double M;
double E = 0.01;
int iter = 0;

int determ();
void zone();
double funct(double x);
double calcul();

int main() {
	//проверка на наличие корня функции в заданных границах (и расширение на 0.5 + можно сразу вторую границу додвинуть)
	//инверсия L R, если функция возрастает
	determ();
	zone();
	std::cout << "{M=" << calcul() << "; k=" << iter << "; Ep=" << funct(M) << "}" << std::endl;
	
}
int determ() {//пересчёт R до прот. знака с шагом 0.5
	if (R < 0) {
		while (R <= 0) { R += 0.5; }
	}
	else { 
		while (R >= 0) { R -= 0.5; } 
	}
	std::cout << "L: " << L << "  R: " << R << std::endl;
	return R;
}
void zone() {//проверка на наличие корня функции в заданных границах
	if (funct(L) > funct(R)) { //если функция убывает
		std::cout << "go low" << std::endl;
		while (funct(L) < 0 or funct(R) > 0) {//пока корень вне границ
			if (funct(L) < 0) { //если корень не в левой границе
				L -= 0.5;
			}
			if (funct(R) > 0) { //если корень не в правой границе
				R += 0.5;
			}
		}
	}
	else{//функция возрастает
		std::cout << "go high" << std::endl;
		while (funct(L) > 0 or funct(R) < 0) {//пока корень вне границ
			if (funct(L) > 0) { //если корень не в левой границе
				L -= 0.5;
			}
			if (funct(R) < 0) { //если корень не в правой границе
				R += 0.5;
			}
		}
	}
	std::cout << "True L: " << L << "  True R: " << R << std::endl;
}
double calcul(){ //рассчёт M
	M = (L + R) / 2;
	while (abs(funct(M)) > E) { //пока больше погрешности 
		M = (L + R) / 2;
		std::cout << "M: " << M << "  funct M: " << funct(M) << std::endl;
		if (funct(M) < 0) { //если не дошло до нуля по левой границе
			L = M; //двигаем границу
			std::cout << "New L: " << L << std::endl;
		}
		else { //если не дошло по правой границе
			R = M; //двигаем границу
			std::cout << "New R: " << R << std::endl;
		}
		iter += 1;
	}
	std::cout << "Target M: " << M << "  his funct: " << funct(M) << std::endl;
	std::cout << "Iterations count: " << iter << std::endl;
	return M;
};
double funct(double x) {
	//return cos(2*x + 3.1415 / 2) + x + 8;//вариант 1, корень в -7.9 (возрастает)
	return 0.7 * (sin(x / 2) + cos(2 * x)) - x; //вариант 4, корень в 0.5 (убывает)
};