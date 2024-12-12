// ���-������� ������������ ��� ������������ �������� � ������ ������, ������ ����� ������������� � ������� ���-������� � ������, 
// ������� ���������� ������ � �������. ���� ��� �������� �������� � ���� ������ (��������), ��������� �������� ������ ��� 
// ���������� ���� ��������. 

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <functional>

// ���� ��� ���-�������
struct HashNode {
    std::string key; // ���� ��� �������� ������
    HashNode* left; // ��������� �� ���������� (�����)
    HashNode* right; // //��������� �� ���������� (������)

    HashNode(const std::string& key) : key(key), left(nullptr), right(nullptr) {} //�������������� ���� � ������ � �������� ���������.
};

// ���-�������
class HashTable {
private:
    std::vector<HashNode*> table; // ������ ���������� �� ����� �������� ��������, ����������� �������
    std::hash<std::string> hashFunction;
    size_t size; //������ ������� (���������� ������)

    void insertNode(HashNode*& root, const std::string& key) {
        if (!root) {
            root = new HashNode(key); // ���� ������� ������ ����, ��������� ����� ����
        }
        // ���� ���� ������ �������� ����, ����� ����������� � ����� ���������, ����� � ������
        else if (key < root->key) {
            insertNode(root->left, key);
        }
        else {
            insertNode(root->right, key);
        }
    }
    // ������ ����� (������������ ������� -> ������� ���� -> ����� -> ������ ����������)
    void preOrder(HashNode* node) const {
        if (node) {
            std::cout << node->key << " ";
            preOrder(node->left);
            preOrder(node->right);
        }
    }
    // ����������� (����, �����, �����)
    void inOrder(HashNode* node) const {
        if (node) {
            inOrder(node->left);
            std::cout << node->key << " ";
            inOrder(node->right);
        }
    }
    // �������� (����, �����, �����)
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

    // ������� � ��� - �������
    void insert(const std::string& key) {
        size_t index = hashFunction(key) % size; // ������ ������ ��� �����
        insertNode(table[index], key); // ��������� ����� � ������, ���� � ���� ������ ��� ���� ����
    }
    
    // ����� ������
    void traversePreOrder() const {
        for (const auto& root : table) {
            preOrder(root);
        }
        std::cout << std::endl;
    }
    // ����� ������
    void traverseInOrder() const {
        for (const auto& root : table) {
            inOrder(root);
        }
        std::cout << std::endl;
    }
    // ����� ������
    void traversePostOrder() const {
        for (const auto& root : table) {
            postOrder(root);
        }
        std::cout << std::endl;
    }
    // ���������� ������
    ~HashTable() {
        for (auto& root : table) {
            deleteTree(root);
        }
    }

private:
    // �������� �����
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
    std::ifstream inputFile("text.txt");  // ������
    if (!inputFile.is_open()) {
        std::cerr << "������: �� ������� ������� ����." << std::endl;
        return 1;
    }

    HashTable hashTable(10);
    std::string word;

    while (inputFile >> word) { // ������ 1 �����
        hashTable.insert(word); // ��������� � ��� �������
    }

    inputFile.close();

    std::cout << "����� � ������ �������:" << std::endl;
    hashTable.traversePreOrder();

    std::cout << "����� � ����������� �������:" << std::endl;
    hashTable.traverseInOrder();

    std::cout << "����� � �������� �������:" << std::endl;
    hashTable.traversePostOrder();

    return 0;
}
