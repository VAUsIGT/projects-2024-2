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

// ���������� ���������� ��� �������� ��������
std::string input;
size_t pos = 0;

// ��������� �������
Node* parseTree();
Node* parseNode();
int parseNumber();

// ���������� �������
void skipSpaces() {
    while (pos < input.size() && std::isspace((unsigned char)input[pos])) {
        pos++;
    }
}

// ��������� ����� (���� ��� ��������)
int parseNumber() {
    skipSpaces();
    if (pos >= input.size() || !std::isdigit((unsigned char)input[pos]))
        throw std::runtime_error("��������� �����");
    int num = 0;
    while (pos < input.size() && std::isdigit((unsigned char)input[pos])) {
        num = num * 10 + (input[pos] - '0');
        pos++;
    }
    skipSpaces();
    return num;
}

// ������ ����. ���� - ��� �����. �� ��� ����� ��������� (���������).
// ������ ���� ����� ����:
// �����
// ���
// �����(�����_���������,������_���������)
Node* parseNode() {
    skipSpaces();
    int val = parseNumber();
    Node* node = new Node(val);
    skipSpaces();
    // ���������, ���� �� ����������
    if (pos < input.size() && input[pos] == '(') {
        // ���������� '('
        pos++;
        skipSpaces();
        // ������ ����� ���������
        // ����� ���� ������, ���� ����� �������
        Node* leftSubtree = nullptr;
        if (pos < input.size() && input[pos] != ',' && input[pos] != ')') {
            // ���� ����� ����
            leftSubtree = parseNode();
        }

        skipSpaces();
        if (pos >= input.size() || input[pos] != ',')
            throw std::runtime_error("��������� ������� ����� ������ ���������");
        pos++; // ���������� ','
        skipSpaces();

        // ������ ������ ���������
        Node* rightSubtree = nullptr;
        if (pos < input.size() && input[pos] != ')') {
            // ���� ������ ����
            rightSubtree = parseNode();
        }

        skipSpaces();
        if (pos >= input.size() || input[pos] != ')')
            throw std::runtime_error("��������� ����������� ������ ')'");
        pos++; // ���������� ')'

        node->left = leftSubtree;
        node->right = rightSubtree;
    }
    return node;
}

// ������ �� ������ (�� ����, ��� ������, ���� ������ ��� �������������� ������)
Node* parseTree() {
    skipSpaces();
    if (pos >= input.size())
        return nullptr;
    Node* root = parseNode();
    skipSpaces();
    if (pos != input.size()) {
        // ���� �������� ����������� �������, ��� ������
        // �� ����� ������� ����������, ���� ������ ��������� ������������� ������ ������
    }
    return root;
}

// ������ �����
void preOrderPrint(Node* root) {
    if (!root) return;
    std::cout << root->value << " ";
    preOrderPrint(root->left);
    preOrderPrint(root->right);
}
 // ����������� �����
void inOrderPrint(Node* root) {
    if (!root) return;
    inOrderPrint(root->left);
    std::cout << root->value << " ";
    inOrderPrint(root->right);
}
// �������� �����
void postOrderPrint(Node* root) {
    if (!root) return;
    postOrderPrint(root->left);
    postOrderPrint(root->right);
    std::cout << root->value << " ";
}
// ����������
void deleteTree(Node* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::cout << "������� ������ � �������, ��������: 8 (3 (1,6(4,7)),10(,14(13,)))\n";
    std::getline(std::cin, input);

    // ������� ������� ��� ��������
    {
        std::string tmp;
        for (char c : input) {
            // ����� �������� ������� ��� ������. ���� �������, ������ ���������� ��.
            // ����� ����� ������ ��������, ��� ��� skipSpaces() �� ����������.
            tmp.push_back(c);
        }
        input = tmp;
    }

    Node* root = nullptr;
    try {
        root = parseTree();
    }
    catch (const std::exception& e) {
        std::cerr << "������ ��� �������: " << e.what() << "\n";
        return 1;
    }

    std::cout << "������ �����:\n";
    preOrderPrint(root);
    std::cout << "\n";

    std::cout << "����������� �����:\n";
    inOrderPrint(root);
    std::cout << "\n";

    std::cout << "�������� �����:\n";
    postOrderPrint(root);
    std::cout << "\n";

    deleteTree(root);
    return 0;
}
