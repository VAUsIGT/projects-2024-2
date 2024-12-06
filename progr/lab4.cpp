#include <iostream>
#include <string>
using namespace std;

struct Node {
    string data;
    Node* left;
    Node* right;

    Node(string value) : data(value), left(nullptr), right(nullptr) {}
    ~Node() {//����������
        left = nullptr;
        right = nullptr;
        data = "";
    }
};

class Stack
{
private:
    Node* head; //������ �� ������� ���� 
    // ������ �����
public:
    Stack() : head(nullptr) {} // ����� ������������� ������� ���� � ������

    ~Stack() { // ����������
        while (head) {
            Node* temp = head;
            head = head->right;
            delete temp; // ����������� ������ �������� ����
            if (head) {             
                head->left = nullptr;
            }
        }
        head = nullptr; // �������� ��������� ��� ������������
    }

    void push(const std::string& data) //  ���������� ������ ��������   (1)
    {
        Node* newNode = new Node(data); //  ����� ���� � �������
        newNode->right = head; // ��������� ��������� ���� �� ������� ������� ����
        if (head)
        {
            head->left = newNode; // ���� ���� �� ����, ������������� �������������� ���� ��� ������� �������� ����
        }
        head = newNode; // ��������� ������� ���� �����
    }

    std::string priz() //  �������� � ���������� �������� 
    {
        if (!head)
        {
            throw std::out_of_range("���� ����");// ����������, ���� ���� ����
        }
        std::string data = head->data; // ��������� ������ �������� ����
        Node* temp = head; // ��������� ��������� �� ������� ����
        head = head->right; // ��������� ������� ���� �� ���������
        if (head)
        {
            head->left = nullptr;// ������������� �������������� ���� ��� ������ �������� ����
        }

        delete temp; // ����������� ������, ������� ������ ������� �����
        return data; // ���������� ������ ���������� ����
    }
    bool proverka(const std::string& data)// ����� ����������� ���������� �� ��������� ������� � �����  (3).
    {

        Node* tamp = head; // �������� � �������� ����
        while (tamp)
        {
            if (tamp->data == data)
            {
                return true; //���� ������� ������
            }
            else

                tamp = tamp->right;
        }
        return false; //���� �� ������
    }

    int ves(const std::string& data)
    {
        Node* tamp = head;
        int index = 1;
        while (tamp)
        {
            if (tamp->data == data)
            {
                return index;
            }
            tamp = tamp->right;
            index++;
        }
        return -1;
    }

    int depth() const
    {
        int count = 0;
        Node* current = head;
        while (current) {
            count++;
            current = current->right;
        }
        return count;
    }

    Stack operator+(const Stack& other) const
    {
        Stack result;
        Stack newStack;  // ������� ����� ����
        Node* tamp = other.head;  // �������� � �������� ���� ������� �����
        while (tamp)
        {
            newStack.push(tamp->data);  // ��������� �������� �� ������� �����
            tamp = tamp->right;  // ��������� � ���������� ����
        }
        tamp = head;  // ��������� � �������� ���� �������� �����
        while (tamp) {
            newStack.push(tamp->data);  // ��������� �������� �� �������� �����
            tamp = tamp->right;  // ��������� � ���������� ����
        }
        while (newStack.depth() > 0) {
            result.push(newStack.priz());
        }
        return result;  // ���������� ����� ����
    }

    Stack operator*(const Stack& other) const  // �������� ������������
    {
        Stack result;
        Stack newStack;  // ������� ����� ����
        Node* tamp1 = head;  // �������� � �������� ���� �������� �����
        Node* tamp2 = other.head;  // �������� � �������� ���� ������� �����

        while (tamp1 || tamp2)
        {
            if (tamp1) {
                newStack.push(tamp1->data);  // ��������� ������� �� �������� �����
                tamp1 = tamp1->right;  // ��������� � ���������� ����
            }
            if (tamp2) {
                newStack.push(tamp2->data);  // ��������� ������� �� ������� �����
                tamp2 = tamp2->right;  // ��������� � ���������� ����
            }
        }
        while (newStack.depth() > 0) {
            result.push(newStack.priz());
        }
        return result; // ���������� ����� ����
    }

    Stack operator-() const {
        Stack newStack;  // ������� ����� ����
        Node* tamp = head;  // �������� � �������� ����
        while (tamp) {
            newStack.push(tamp->data);  // ��������� �������� � ����� ����
            tamp = tamp->right;  // ��������� � ���������� ����
        }
        return newStack;  // ���������� ����� ����
    }

    void print() const {
        Node* tamp = head;  // �������� � �������� ����
        while (tamp) {
            std::cout << tamp->data;  // ������� ������ ����
            if (tamp->right) {
                std::cout << " - ";  // ���� ���� ��������� ����, ��������� �������
            }
            tamp = tamp->right;  // ��������� � ���������� ����
        }
        std::cout << std::endl;  // �������� ����� ������ ����� ����������
    }


    void ClearE(const std::string& data)
    {
        Node* tamp = head;
        while (tamp)
        {
            if (tamp->data == data)
            {  // ���� ������ ���� ��������� � ��������
                Node* toDelete = tamp;  // ��������� ��������� �� ���� ��� ��������
                if (tamp->left)
                {
                    tamp->left->right = tamp->right;  // ��������� ��������� ����������� ����
                }
                else
                {
                    head = tamp->right;  // ���� ��� ��� ������� ����, ��������� head
                    head->left = nullptr;
                }
                if (tamp->right) {
                    tamp->right->left = tamp->left;  // ��������� ��������� ���������� ����
                }
                tamp = tamp->right;  // ��������� � ���������� ����
                delete toDelete;  // ����������� ������
            }
            else {
                tamp = tamp->right;  // ��������� � ���������� ����
            }
        }
    }
};

int main()
{
    setlocale(LC_ALL, "Russian");
    ///////////////////////---
    Stack stack1;
    stack1.push("s1");
    stack1.push("s2");
    stack1.push("s3");

    Stack stack2;
    stack2.push("p1");
    stack2.push("p2");
    stack2.push("p3");
    ////////////////////////---
    std::cout << "Stack 1: ";
    stack1.print();
    std::cout << "Stack 2: ";
    stack2.print();

    /////////////////////////
    //����� �������� � (������� T/F).
    if (stack1.proverka("s1") == 1)
        std::cout << "�������� �� �������: T";

    else
        std::cout << "�������� �� �������: F";
    std::cout << std::endl;
    std::cout << "�������� � ���������� ��������: " << stack1.priz() << std::endl;
    /////////////////////////
    //
    std::cout << "������� �����: " << stack1.depth() << std::endl;

    std::cout << "��� ��������: " << stack2.ves("p2") << std::endl;

    // A+B
    Stack stack3 = stack1 + stack2;
    std::cout << "Stack 3 (A + B): ";
    stack3.print();

    // A*B
    Stack stack4 = stack1 * stack2;
    std::cout << "Stack 4 (A * B): ";
    stack4.print();

    // -A
    Stack stack5 = -stack2;
    std::cout << "�������� Stack 2: ";
    stack5.print();
    //������� ClearE
    stack1.ClearE("s1");
    std::cout << "������� ClearE:  ";
    stack1.print();
}