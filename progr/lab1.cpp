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
std::string calcul();

int main() {
	//�������� �� ������� ����� ������� � �������� �������� (� ���������� �� 0.5 + ����� ����� ������ ������� ���������)
	//�������� L R, ���� ������� ����������
	determ();
	zone();
	std::cout << calcul() << std::endl;
	//std::cout << "{M=" << calcul() << "; k=" << iter << "; Ep=" << funct(M) << "}" << std::endl;
	
}
int determ() {//�������� R �� ����. ����� � ����� 0.5
	if (R < 0) {
		while (R <= 0) { R += 0.5; }
	}
	else { 
		while (R >= 0) { R -= 0.5; } 
	}
	//std::cout << "L: " << L << "  R: " << R << std::endl;
	return R;
}
void zone() {//�������� �� ������� ����� ������� � �������� ��������
	if (funct(L) > funct(R)) { //���� ������� �������
		lowhigh = 1;
		//std::cout << "go low" << std::endl;
		while (funct(L) < 0 or funct(R) > 0) {//���� ������ ��� ������
			if (funct(L) < 0) { //���� ������ �� � ����� �������
				//std::cout << "no target" << std::endl;
				L -= 0.5;
			}
			if (funct(R) > 0) { //���� ������ �� � ������ �������
				//std::cout << "no target" << std::endl;
				R += 0.5;
			}
		}
	}
	else{//������� ����������
		//std::cout << "go high" << std::endl;
		while (funct(L) > 0 or funct(R) < 0) {//���� ������ ��� ������
			if (funct(L) > 0) { //���� ������ �� � ����� �������
				//std::cout << "no target" << std::endl;
				L -= 0.5;
			}
			if (funct(R) < 0) { //���� ������ �� � ������ �������
				//std::cout << "no target" << std::endl;
				R += 0.5;
			}
		}
	}
	//std::cout << "True L: " << L << "  True R: " << R << std::endl;
}
std::string calcul(){ //������� M
	M = (L + R) / 2;
	while (abs(funct(M)) > E) { //���� ������ ����������� 
		M = (L + R) / 2;
		//std::cout << "M: " << M << "  funct M: " << funct(M) << std::endl;
		if (funct(M) < 0) { //���� �� ����� �� ���� �� ����� �������
			if(lowhigh){
				R = M; //������� �������, ��� ���������
			}
			else { L = M; }//��� ������������
			//std::cout << "New L: " << L << std::endl;
		}
		else { //���� �� ����� �� ������ �������
			if (lowhigh) { L = M; }
			else{ R = M; }
			//std::cout << "New R: " << R << std::endl;
		}
		iter += 1;
	}
	//std::cout << "Target M: " << M << "  his funct: " << funct(M) << std::endl;
	//std::cout << "Iterations count: " << iter << std::endl;
	return "{M=" + std::to_string(M) + "; k=" + std::to_string(iter) + "; Ep=" + std::to_string(funct(M)) + "}";
};
double funct(double x) {
	//return cos(2*x + 3.1415 / 2) + x + 8;//������� 1, ������ � -7.9 (����������)
	return 0.7 * (sin(x / 2) + cos(2 * x)) - x; //������� 4, ������ � 0.5 (�������)
};