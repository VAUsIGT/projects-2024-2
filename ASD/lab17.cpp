#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <functional>

struct Node {
    int value;
    Node* left;
    Node* right;

    Node(int value) : value(value), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
private:
    Node* root;

    Node* parseTree(std::istringstream& stream) {
        char ch;
        stream >> ch; // читаем '('
        if (ch != '(') return nullptr;

        int value;
        if (!(stream >> value)) {
            stream >> ch; // читаем ')'
            return nullptr;
        }

        Node* node = new Node(value);
        node->left = parseTree(stream);
        node->right = parseTree(stream);

        stream >> ch; // читаем ')'
        return node;
    }

    void toLinearBracket(Node* node, std::ostringstream& output) const {
        if (!node) {
            output << "()";
            return;
        }
        output << "(" << node->value;
        toLinearBracket(node->left, output);
        toLinearBracket(node->right, output);
        output << ")";
    }

    Node* insert(Node* node, int value) {
        if (!node) return new Node(value);
        if (value < node->value)
            node->left = insert(node->left, value);
        else if (value > node->value)
            node->right = insert(node->right, value);
        return node;
    }

    Node* findMin(Node* node) {
        while (node && node->left) node = node->left;
        return node;
    }

    Node* remove(Node* node, int value) {
        if (!node) return nullptr;

        if (value < node->value)
            node->left = remove(node->left, value);
        else if (value > node->value)
            node->right = remove(node->right, value);
        else {
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

            Node* minNode = findMin(node->right);
            node->value = minNode->value;
            node->right = remove(node->right, minNode->value);
        }
        return node;
    }

    bool search(Node* node, int value) const {
        if (!node) return false;
        if (value == node->value) return true;
        if (value < node->value)
            return search(node->left, value);
        else
            return search(node->right, value);
    }

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

    void buildFromLinearBracket(const std::string& input) {
        std::istringstream stream(input);
        root = parseTree(stream);
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
        toLinearBracket(root, output);
        return output.str();
    }
};

int main() {
    BinarySearchTree bst;

    std::cout << "Введите дерево в формате линейно-скобочной записи: ";
    std::string input;
    std::getline(std::cin, input);
    bst.buildFromLinearBracket(input);

    while (true) {
        std::cout << "\nМеню:\n";
        std::cout << "1. Добавить вершину\n";
        std::cout << "2. Удалить вершину\n";
        std::cout << "3. Найти вершину\n";
        std::cout << "4. Показать дерево\n";
        std::cout << "5. Выход\n";
        std::cout << "Введите номер операции: ";

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