#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <stdexcept>

// структура, представляющая узел дерева
struct Node {
    int value; // целочисленное
    Node* left; // левое поддерево
    Node* right; // правое поддерево

    Node(int value) : value(value), left(nullptr), right(nullptr) {}
};

// инкапсулируем корневой указатель на дерево root и методы для работы с ним
class BinarySearchTree {
private:
    Node* root;
    std::string input;
    size_t pos = 0;

    // пропуск пробелов
    void skipSpaces() {
        while (pos < input.size() && std::isspace((unsigned char)input[pos])) {
            pos++;
        }
    }

    // парсим число
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

    // парсим поддерево, которое может быть пустым
    Node* parseSubtree() {
        skipSpaces();
        // если сразу запятая или закрывающая скобка - поддерево пустое
        if (pos >= input.size() || input[pos] == ',' || input[pos] == ')')
            return nullptr;

        return parseNode();
    }

    // парсим узел в формате: ЧИСЛО [ (ЛЕВОЕ,ПРАВОЕ) ]
    // пример: 8 (3 (1,6(4,7)), 10 (,14(13,)))
    Node* parseNode() {
        int val = parseNumber();
        Node* node = new Node(val);

        skipSpaces();
        if (pos < input.size() && input[pos] == '(') {
            pos++; // пропускаем '('
            skipSpaces();
            // парсим левое поддерево
            node->left = parseSubtree();

            skipSpaces();
            if (pos >= input.size() || input[pos] != ',')
                throw std::runtime_error("Ожидалась запятая после левого поддерева");
            pos++; // пропускаем ','
            skipSpaces();

            // парсим правое поддерево
            node->right = parseSubtree();

            skipSpaces();
            if (pos >= input.size() || input[pos] != ')')
                throw std::runtime_error("Ожидалась закрывающая скобка ')'");
            pos++; // пропускаем ')'
        }

        return node;
    }

    // запись линейно-скобочная. () - пустое дерево, (10(5()())()) - дерево с корнем 10, у которого левый сын 5, а правый — пуст
    // Данный комментарий сохранён, хотя формат теперь другой. Логика изменения ниже.
    // В текущей реализации формат другой, но комментарий не меняем.

    // создает строку с описанием текущего дерева в подобной линейно-скобочной форме
    // Если у узла есть поддеревья, выводим: value(left_subtree,right_subtree)
    // Пустое поддерево: пропускаем значение, оставляем запятую.
    void toCustomFormat(Node* node, std::ostringstream& output) const {
        if (!node) return;
        output << node->value;
        if (node->left || node->right) {
            output << "(";
            if (node->left) {
                toCustomFormat(node->left, output);
            }
            // если левого нет, просто сразу запятая
            output << ",";
            if (node->right) {
                toCustomFormat(node->right, output);
            }
            // если правого нет, ничего не добавляем
            output << ")";
        }
    }

    // вставка
    Node* insert(Node* node, int value) {
        if (!node) return new Node(value); // если текущий узел пуст, создаем новый
        if (value < node->value)
            node->left = insert(node->left, value); // если значение меньше значения текущего узла — идём влево
        else if (value > node->value)
            node->right = insert(node->right, value); // больше - вправо
        return node;
    }

    Node* findMin(Node* node) {
        while (node && node->left) node = node->left;
        return node;
    }

    // удаление
    Node* remove(Node* node, int value) {
        if (!node) return nullptr;

        if (value < node->value)
            node->left = remove(node->left, value);
        else if (value > node->value)
            node->right = remove(node->right, value);
        else {
            // нашли узел для удаления (если ребенок левый, то заменяем на него, иначе на правого
            if (!node->left) {
                Node* rightChild = node->right;
                delete node;
                return rightChild;
            }
            else if (!node->right) {
                Node* leftChild = node->left;
                delete node;
                return leftChild;
            }

            // у узла 2 ребёнка 
            Node* minNode = findMin(node->right); //ищем минимальный в правом поддереве
            node->value = minNode->value;
            node->right = remove(node->right, minNode->value);
        }
        return node;
    }

    // поиск
    bool search(Node* node, int value) const {
        if (!node) return false;
        if (value == node->value) return true;
        if (value < node->value)
            return search(node->left, value);
        else
            return search(node->right, value);
    }

    // удаление всего дерева (рекурсивно)
    void deleteTree(Node* node) {
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

public:
    BinarySearchTree() : root(nullptr) {}

    ~BinarySearchTree() {
        deleteTree(root);
    }
    // проверка корректости начального ввода
    bool isValidBracketNotation(const std::string& input) {
        int balance = 0;
        for (char ch : input) {
            if (ch == '(') balance++;
            else if (ch == ')') balance--;
            if (balance < 0) return false;
        }
        return balance == 0;
    }

    // теперь формат принимаемых данных: например 8 (3 (1, 6 (4,7)), 10 (, 14(13,)))
    // здесь мы создаём дерево, вызывая parseNode.
    void buildFromLinearBracket(const std::string& str) {
        input = str;
        pos = 0;
        skipSpaces();
        if (pos < input.size()) {
            root = parseNode();
        }
        skipSpaces();
    }

    void insert(int value) {
        root = insert(root, value);
    }

    void remove(int value) {
        root = remove(root, value);
    }

    bool search(int value) const {
        return search(root, value);
    }

    std::string toLinearBracket() const {
        std::ostringstream output;
        if (root) {
            toCustomFormat(root, output);
        }
        return output.str();
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    BinarySearchTree bst;

    std::cout << "Введите дерево в формате линейно-скобочной записи: ";
    std::string input;
    std::getline(std::cin, input);
    // построение
    bst.buildFromLinearBracket(input);
    if (!bst.isValidBracketNotation(input)) {
        std::cerr << "Некорректный формат линейно-скобочной записи!" << std::endl;
        std::cerr << "Принято дерево: " << bst.toLinearBracket() << std::endl;
    }

    while (true) {
        std::cout << "\nМеню:\n";
        std::cout << "1. Добавить вершину\n";
        std::cout << "2. Удалить вершину\n";
        std::cout << "3. Найти вершину\n";
        std::cout << "4. Показать дерево\n";
        std::cout << "5. Выход\n";
        std::cout << "Введите номер операции: ";

        // выбор пользователя
        int choice;
        std::cin >> choice;

        if (choice == 5) {
            std::cout << "Дерево перед выходом: " << bst.toLinearBracket() << "\n";
            break;
        }

        if (choice == 1) {
            std::cout << "Введите значение для добавления: ";
            int value;
            std::cin >> value;
            bst.insert(value);
        }
        else if (choice == 2) {
            std::cout << "Введите значение для удаления: ";
            int value;
            std::cin >> value;
            bst.remove(value);
        }
        else if (choice == 3) {
            std::cout << "Введите значение для поиска: ";
            int value;
            std::cin >> value;
            if (bst.search(value)) {
                std::cout << "Вершина найдена.\n";
            }
            else {
                std::cout << "Вершина не найдена.\n";
            }
        }
        else if (choice == 4) {
            std::cout << "Текущее дерево: " << bst.toLinearBracket() << "\n";
        }
        else {
            std::cout << "Некорректный выбор. Попробуйте снова.\n";
        }
    }

    return 0;
}
