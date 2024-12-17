#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <stdexcept>

// ���������, �������������� ���� ������
struct Node {
    int value; // �������������
    Node* left; // ����� ���������
    Node* right; // ������ ���������

    Node(int value) : value(value), left(nullptr), right(nullptr) {}
};

// ������������� �������� ��������� �� ������ root � ������ ��� ������ � ���
class BinarySearchTree {
private:
    Node* root;
    std::string input;
    size_t pos = 0;

    // ������� ��������
    void skipSpaces() {
        while (pos < input.size() && std::isspace((unsigned char)input[pos])) {
            pos++;
        }
    }

    // ������ �����
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

    // ������ ���������, ������� ����� ���� ������
    Node* parseSubtree() {
        skipSpaces();
        // ���� ����� ������� ��� ����������� ������ - ��������� ������
        if (pos >= input.size() || input[pos] == ',' || input[pos] == ')')
            return nullptr;

        return parseNode();
    }

    // ������ ���� � �������: ����� [ (�����,������) ]
    // ������: 8 (3 (1,6(4,7)), 10 (,14(13,)))
    Node* parseNode() {
        int val = parseNumber();
        Node* node = new Node(val);

        skipSpaces();
        if (pos < input.size() && input[pos] == '(') {
            pos++; // ���������� '('
            skipSpaces();
            // ������ ����� ���������
            node->left = parseSubtree();

            skipSpaces();
            if (pos >= input.size() || input[pos] != ',')
                throw std::runtime_error("��������� ������� ����� ������ ���������");
            pos++; // ���������� ','
            skipSpaces();

            // ������ ������ ���������
            node->right = parseSubtree();

            skipSpaces();
            if (pos >= input.size() || input[pos] != ')')
                throw std::runtime_error("��������� ����������� ������ ')'");
            pos++; // ���������� ')'
        }

        return node;
    }

    // ������ �������-���������. () - ������ ������, (10(5()())()) - ������ � ������ 10, � �������� ����� ��� 5, � ������ � ����
    // ������ ����������� �������, ���� ������ ������ ������. ������ ��������� ����.
    // � ������� ���������� ������ ������, �� ����������� �� ������.

    // ������� ������ � ��������� �������� ������ � �������� �������-��������� �����
    // ���� � ���� ���� ����������, �������: value(left_subtree,right_subtree)
    // ������ ���������: ���������� ��������, ��������� �������.
    void toCustomFormat(Node* node, std::ostringstream& output) const {
        if (!node) return;
        output << node->value;
        if (node->left || node->right) {
            output << "(";
            if (node->left) {
                toCustomFormat(node->left, output);
            }
            // ���� ������ ���, ������ ����� �������
            output << ",";
            if (node->right) {
                toCustomFormat(node->right, output);
            }
            // ���� ������� ���, ������ �� ���������
            output << ")";
        }
    }

    // �������
    Node* insert(Node* node, int value) {
        if (!node) return new Node(value); // ���� ������� ���� ����, ������� �����
        if (value < node->value)
            node->left = insert(node->left, value); // ���� �������� ������ �������� �������� ���� � ��� �����
        else if (value > node->value)
            node->right = insert(node->right, value); // ������ - ������
        return node;
    }

    Node* findMin(Node* node) {
        while (node && node->left) node = node->left;
        return node;
    }

    // ��������
    Node* remove(Node* node, int value) {
        if (!node) return nullptr;

        if (value < node->value)
            node->left = remove(node->left, value);
        else if (value > node->value)
            node->right = remove(node->right, value);
        else {
            // ����� ���� ��� �������� (���� ������� �����, �� �������� �� ����, ����� �� �������
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

            // � ���� 2 ������ 
            Node* minNode = findMin(node->right); //���� ����������� � ������ ���������
            node->value = minNode->value;
            node->right = remove(node->right, minNode->value);
        }
        return node;
    }

    // �����
    bool search(Node* node, int value) const {
        if (!node) return false;
        if (value == node->value) return true;
        if (value < node->value)
            return search(node->left, value);
        else
            return search(node->right, value);
    }

    // �������� ����� ������ (����������)
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
    // �������� ����������� ���������� �����
    bool isValidBracketNotation(const std::string& input) {
        int balance = 0;
        for (char ch : input) {
            if (ch == '(') balance++;
            else if (ch == ')') balance--;
            if (balance < 0) return false;
        }
        return balance == 0;
    }

    // ������ ������ ����������� ������: �������� 8 (3 (1, 6 (4,7)), 10 (, 14(13,)))
    // ����� �� ������ ������, ������� parseNode.
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

    std::cout << "������� ������ � ������� �������-��������� ������: ";
    std::string input;
    std::getline(std::cin, input);
    // ����������
    bst.buildFromLinearBracket(input);
    if (!bst.isValidBracketNotation(input)) {
        std::cerr << "������������ ������ �������-��������� ������!" << std::endl;
        std::cerr << "������� ������: " << bst.toLinearBracket() << std::endl;
    }

    while (true) {
        std::cout << "\n����:\n";
        std::cout << "1. �������� �������\n";
        std::cout << "2. ������� �������\n";
        std::cout << "3. ����� �������\n";
        std::cout << "4. �������� ������\n";
        std::cout << "5. �����\n";
        std::cout << "������� ����� ��������: ";

        // ����� ������������
        int choice;
        std::cin >> choice;

        if (choice == 5) {
            std::cout << "������ ����� �������: " << bst.toLinearBracket() << "\n";
            break;
        }

        if (choice == 1) {
            std::cout << "������� �������� ��� ����������: ";
            int value;
            std::cin >> value;
            bst.insert(value);
        }
        else if (choice == 2) {
            std::cout << "������� �������� ��� ��������: ";
            int value;
            std::cin >> value;
            bst.remove(value);
        }
        else if (choice == 3) {
            std::cout << "������� �������� ��� ������: ";
            int value;
            std::cin >> value;
            if (bst.search(value)) {
                std::cout << "������� �������.\n";
            }
            else {
                std::cout << "������� �� �������.\n";
            }
        }
        else if (choice == 4) {
            std::cout << "������� ������: " << bst.toLinearBracket() << "\n";
        }
        else {
            std::cout << "������������ �����. ���������� �����.\n";
        }
    }

    return 0;
}
