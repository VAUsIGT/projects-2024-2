#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>

struct Node {
    int value;
    Node* left;
    Node* right;
    Node(int v) : value(v), left(nullptr), right(nullptr) {}
};

// Глобальные переменные для удобства парсинга
std::string input;
size_t pos = 0;

// Прототипы функций
Node* parseTree();
Node* parseNode();
int parseNumber();

// Пропускаем пробелы
void skipSpaces() {
    while (pos < input.size() && std::isspace((unsigned char)input[pos])) {
        pos++;
    }
}

// Считываем число (узел без потомков)
int parseNumber() {
    skipSpaces();
    if (pos >= input.size() || !std::isdigit((unsigned char)input[pos]))
        throw std::runtime_error("Ожидалось число");
    int num = 0;
    while (pos < input.size() && std::isdigit((unsigned char)input[pos])) {
        num = num * 10 + (input[pos] - '0');
        pos++;
    }
    skipSpaces();
    return num;
}

// Парсим узел. Узел - это число. За ним может следовать (поддерево).
// Формат узла может быть:
// ЧИСЛО
// или
// ЧИСЛО(ЛЕВОЕ_ПОДДЕРЕВО,ПРАВОЕ_ПОДДЕРЕВО)
Node* parseNode() {
    skipSpaces();
    int val = parseNumber();
    Node* node = new Node(val);
    skipSpaces();
    // Проверяем, есть ли поддеревья
    if (pos < input.size() && input[pos] == '(') {
        // Пропускаем '('
        pos++;
        skipSpaces();
        // Парсим левое поддерево
        // Может быть пустым, если сразу запятая
        Node* leftSubtree = nullptr;
        if (pos < input.size() && input[pos] != ',' && input[pos] != ')') {
            // Есть левый узел
            leftSubtree = parseNode();
        }

        skipSpaces();
        if (pos >= input.size() || input[pos] != ',')
            throw std::runtime_error("Ожидалась запятая после левого поддерева");
        pos++; // пропускаем ','
        skipSpaces();

        // Парсим правое поддерево
        Node* rightSubtree = nullptr;
        if (pos < input.size() && input[pos] != ')') {
            // Есть правый узел
            rightSubtree = parseNode();
        }

        skipSpaces();
        if (pos >= input.size() || input[pos] != ')')
            throw std::runtime_error("Ожидалась закрывающая скобка ')'");
        pos++; // пропускаем ')'

        node->left = leftSubtree;
        node->right = rightSubtree;
    }
    return node;
}

// Парсим всё дерево (по сути, это обёртка, если корень без дополнительных скобок)
Node* parseTree() {
    skipSpaces();
    if (pos >= input.size())
        return nullptr;
    Node* root = parseNode();
    skipSpaces();
    if (pos != input.size()) {
        // Если остались непарсенные символы, это ошибка
        // Но можно считать допустимым, если формат полностью соответствует одному дереву
    }
    return root;
}

// прямой обход
void preOrderPrint(Node* root) {
    if (!root) return;
    std::cout << root->value << " ";
    preOrderPrint(root->left);
    preOrderPrint(root->right);
}
 // центральный обход
void inOrderPrint(Node* root) {
    if (!root) return;
    inOrderPrint(root->left);
    std::cout << root->value << " ";
    inOrderPrint(root->right);
}
// концевой обход
void postOrderPrint(Node* root) {
    if (!root) return;
    postOrderPrint(root->left);
    postOrderPrint(root->right);
    std::cout << root->value << " ";
}
// деструктор
void deleteTree(Node* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::cout << "Введите дерево в формате, например: 8 (3 (1,6(4,7)),10(,14(13,)))\n";
    std::getline(std::cin, input);

    // Удаляем пробелы для удобства
    {
        std::string tmp;
        for (char c : input) {
            // Можно оставить пробелы или убрать. Если оставим, парсер пропускает их.
            // Здесь можно просто оставить, так как skipSpaces() их обработает.
            tmp.push_back(c);
        }
        input = tmp;
    }

    Node* root = nullptr;
    try {
        root = parseTree();
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при разборе: " << e.what() << "\n";
        return 1;
    }

    std::cout << "Прямой обход:\n";
    preOrderPrint(root);
    std::cout << "\n";

    std::cout << "Центральный обход:\n";
    inOrderPrint(root);
    std::cout << "\n";

    std::cout << "Концевой обход:\n";
    postOrderPrint(root);
    std::cout << "\n";

    deleteTree(root);
    return 0;
}
