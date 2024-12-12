// Хеш-таблица используется для эффективного хранения и поиска данных, каждое слово преобразуется с помощью хеш-функции в индекс, 
// который определяет ячейку в массиве. Если два элемента попадают в одну ячейку (коллизия), создается бинарное дерево для 
// разрешения этой коллизии. 

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <functional>

// Узел для хеш-таблицы
struct HashNode {
    std::string key; // поле для хранения строки
    HashNode* left; // указатели на поддеревья (левое)
    HashNode* right; // //указатели на поддеревья (правое)

    HashNode(const std::string& key) : key(key), left(nullptr), right(nullptr) {} //инициализирует узел с ключом и обнуляет указатели.
};

// Хеш-таблица
class HashTable {
private:
    std::vector<HashNode*> table; // вектор указателей на корни бинарных деревьев, реализующий таблицу
    std::hash<std::string> hashFunction;
    size_t size; //размер таблицы (количество слотов)

    void insertNode(HashNode*& root, const std::string& key) {
        if (!root) {
            root = new HashNode(key); // если текущий корень пуст, создается новый узел
        }
        // если ключ меньше текущего узла, слово добавляется в левое поддерево, иначе в правое
        else if (key < root->key) {
            insertNode(root->left, key);
        }
        else {
            insertNode(root->right, key);
        }
    }
    // Прямой обход (обрабатывает сначала -> текущий узел -> левое -> правое поддеревья)
    void preOrder(HashNode* node) const {
        if (node) {
            std::cout << node->key << " ";
            preOrder(node->left);
            preOrder(node->right);
        }
    }
    // Центральный (лево, центр, право)
    void inOrder(HashNode* node) const {
        if (node) {
            inOrder(node->left);
            std::cout << node->key << " ";
            inOrder(node->right);
        }
    }
    // Концевой (лево, право, центр)
    void postOrder(HashNode* node) const {
        if (node) {
            postOrder(node->left);
            postOrder(node->right);
            std::cout << node->key << " ";
        }
    }

public:
    explicit HashTable(size_t size) : size(size) {
        table.resize(size, nullptr);
    }

    // Вставка в хеш - таблицу
    void insert(const std::string& key) {
        size_t index = hashFunction(key) % size; // индекс ячейки для слова
        insertNode(table[index], key); // добавляет слово в дерево, если в этой ячейке уже есть узлы
    }
    
    // Вызов обхода
    void traversePreOrder() const {
        for (const auto& root : table) {
            preOrder(root);
        }
        std::cout << std::endl;
    }
    // Вызов обхода
    void traverseInOrder() const {
        for (const auto& root : table) {
            inOrder(root);
        }
        std::cout << std::endl;
    }
    // Вызов обхода
    void traversePostOrder() const {
        for (const auto& root : table) {
            postOrder(root);
        }
        std::cout << std::endl;
    }
    // деструктор класса
    ~HashTable() {
        for (auto& root : table) {
            deleteTree(root);
        }
    }

private:
    // удаление узлов
    void deleteTree(HashNode* node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    std::ifstream inputFile("text.txt");  // читаем
    if (!inputFile.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл." << std::endl;
        return 1;
    }

    HashTable hashTable(10);
    std::string word;

    while (inputFile >> word) { // читает 1 слово
        hashTable.insert(word); // добавляет в хеш таблицу
    }

    inputFile.close();

    std::cout << "Обход в прямом порядке:" << std::endl;
    hashTable.traversePreOrder();

    std::cout << "Обход в центральном порядке:" << std::endl;
    hashTable.traverseInOrder();

    std::cout << "Обход в концевом порядке:" << std::endl;
    hashTable.traversePostOrder();

    return 0;
}
