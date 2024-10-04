#include <iostream>
#include <stack>
#include <sstream>
#include <cctype>
#include <stdexcept>

bool checkParentheses(const std::string& expression) {
    std::stack<char> st;
    for (char c : expression) {
        if (c == '(') {
            st.push(c);
        }
        else if (c == ')') {
            if (st.empty()) {
                return false;
            }
            st.pop();
        }
    }
    return st.empty();
}

int getPrecedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

double applyOperation(double a, double b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/':
        if (b == 0) {
            throw std::runtime_error("Ошибка: Деление на ноль.");
        }
        return a / b;
    default: throw std::runtime_error("Ошибка: Неверная операция.");
    }
}

std::string toPostfix(const std::string& expression) {
    std::stack<char> operators;
    std::stringstream postfix;
    for (size_t i = 0; i < expression.size(); ++i) {
        char c = expression[i];

        if (isdigit(c) || c == '.') {
            // Число: добавляем в постфиксную строку
            postfix << c;
        }
        else if (c == '(') {
            operators.push(c);
        }
        else if (c == ')') {
            while (!operators.empty() && operators.top() != '(') {
                postfix << ' ' << operators.top();
                operators.pop();
            }
            if (!operators.empty()) operators.pop(); // Удаляем '('
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            postfix << ' ';
            while (!operators.empty() && getPrecedence(operators.top()) >= getPrecedence(c)) {
                postfix << operators.top() << ' ';
                operators.pop();
            }
            operators.push(c);
        }
        else if (c == '=') {
            break;
        }
    }

    // Добавляем оставшиеся операторы
    while (!operators.empty()) {
        postfix << ' ' << operators.top();
        operators.pop();
    }

    return postfix.str();
}

double evaluatePostfix(const std::string& postfix) {
    std::stack<double> values;
    std::stringstream ss(postfix);
    std::string token;

    while (ss >> token) {
        if (isdigit(token[0]) || (token[0] == '-' && token.size() > 1)) {
            // Если токен - число
            values.push(std::stod(token));
        }
        else if (token.size() == 1 && std::string("+-*/").find(token[0]) != std::string::npos) {
            // Если токен - оператор
            if (values.size() < 2) {
                throw std::runtime_error("Ошибка: Недостаточно операндов.");
            }
            double b = values.top(); values.pop();
            double a = values.top(); values.pop();
            values.push(applyOperation(a, b, token[0]));
        }
    }

    if (values.size() != 1) {
        throw std::runtime_error("Ошибка: Некорректное выражение.");
    }

    return values.top();
}

int main() {
    setlocale(LC_ALL, "Russian"); // Для вывода сообщений на русском языке
    std::string input;

    std::cout << "Введите математическое выражение, заканчивающееся знаком '=': ";
    std::getline(std::cin, input);

    // Проверка на наличие '='
    if (input.back() != '=') {
        std::cout << "Ошибка: Выражение должно заканчиваться знаком '='." << std::endl;
        return 1;
    }

    // Удаление знака '='
    input.pop_back();

    // Проверка корректности скобок
    if (!checkParentheses(input)) {
        std::cout << "Ошибка: Некорректное расположение скобок." << std::endl;
        return 1;
    }

    try {
        // Преобразование в постфиксную запись
        std::string postfix = toPostfix(input);

        // Вычисление результата постфиксного выражения
        double result = evaluatePostfix(postfix);

        std::cout << "Результат выражения: " << result << std::endl;
    }
    catch (const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
