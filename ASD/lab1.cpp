#include <iostream>
#include <stack>

int main() {
    setlocale(LC_ALL, "Russian"); // Для вывода сообщений на русском языке
    std::stack<char> st;
    std::string input;

    std::cout << "Введите строку: ";
    std::cin >> input;

    bool isValid = true;

    // Проверяем правильность скобочного выражения
    for (char c : input) {
        if (c == '(' || c == '{' || c == '[') {
            st.push(c);
        }
        else if (c == ')' || c == '}' || c == ']') {
            if (st.empty()) {
                isValid = false;
                break;
            }
            char top = st.top();
            st.pop();

            // Проверка соответствия типов скобок
            if ((c == ')' && top != '(') ||
                (c == '}' && top != '{') ||
                (c == ']' && top != '[')) {
                isValid = false;
                break;
            }
        }
        else {
            // В строке найден недопустимый символ
            isValid = false;
            break;
        }
    }

    if (isValid && st.empty()) {
        std::cout << "Строка существует." << std::endl;
    }
    else {
        std::cout << "Строка не существует." << std::endl;
    }

    return 0;
}
