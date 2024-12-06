#include <iostream>
#include <string>
#include <locale>

class Node {
public:
    std::string data;
    Node* next;

    Node(const std::string& data) : data(data), next(nullptr) {}
};

class Queue {
private:
    Node* front; //��������� �� ������

public:
    Queue() : front(nullptr) {}

    // ����� 1: ���������� �������� � ����� �������
    void enqueue(const std::string& data) {
        Node* newNode = new Node(data);
        if (!front) {
            front = newNode; //����� ���� ���������� �������
        }
        else {//���� �� ����� �������
            Node* current = front;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode; //��������� ����� ���� � �����
        }
    }

    // ����� 2: �������� � ���������� �������� �� ������ �������
    std::string dequeue() {
        if (!front) return "������� �����!";
        Node* temp = front;
        front = front->next;
        std::string data = temp->data;
        delete temp;
        return data;
    }

    // ����� 3: �������� ������� �������� � �������
    bool find(const std::string& data) {
        Node* current = front;
        while (current) {
            if (current->data == data) return true;
            current = current->next;
        }
        return false;
    }

    // ����� 4: ���������� ���� �������� (������� � �������)
    int weight(const std::string& data) {
        Node* current = front;
        int pos = 1;
        while (current) {
            if (current->data == data) return pos;
            current = current->next;
            pos++;
        }
        return -1;
    }

    // ���������� ��������� + ��� ����������������� ���������� ���� ��������
    Queue operator+(const Queue& other) const {
        Queue result;
        Node* current = front;
        while (current) {
            result.enqueue(current->data);
            current = current->next;
        }
        current = other.front;
        while (current) {
            result.enqueue(current->data);
            current = current->next;
        }
        return result;
    }

    // ���������� ��������� * ��� ������������� ���������� ���� ��������
    Queue operator*(const Queue& other) const {
        Queue result;
        Node* current1 = front;
        Node* current2 = other.front;
        bool toggle = true;

        while (current1 || current2) {
            if (toggle && current1) {
                result.enqueue(current1->data);
                current1 = current1->next;
            }
            else if (!toggle && current2) {
                result.enqueue(current2->data);
                current2 = current2->next;
            }
            toggle = !toggle;
        }
        return result;
    }

    // ������� �������� ~ ��� ��������� �������
    Queue operator~() const {
        Queue reversed;
        Node* current = front;
        while (current) {
            Node* newNode = new Node(current->data);
            newNode->next = reversed.front;
            reversed.front = newNode;
            current = current->next;
        }
        return reversed;
    }

    // ����� YaZanimal ��� ������� �������� b ����� �������� a
    void YaZanimal(const std::string& a, const std::string& b) {
        Node* current = front;
        while (current) {
            if (current->data == a) {
                Node* newNode = new Node(b);
                newNode->next = current->next;
                current->next = newNode;
                break;
            }
            current = current->next;
        }
    }

    // ����� ��� ����������� �������
    void display() const {
        Node* current = front;
        while (current) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

    // ���������� ��� ������������ ������
    ~Queue() {
        while (front) {
            Node* temp = front;
            front = front->next;
            delete temp;
        }
    }
};

int main() {
    // ������������� ������ ��� ����������� ����������� �������� �����
    setlocale(LC_ALL, "Russian");

    Queue q1;
    q1.enqueue("a1");
    q1.enqueue("a2");
    q1.enqueue("a3");
    q1.enqueue("a4");

    Queue q2;
    q2.enqueue("b1");
    q2.enqueue("b2");
    q2.enqueue("b3");

    std::cout << "������� 1: ";
    q1.display();
    std::cout << "������� 2: ";
    q2.display();

    // ����: ���������������� ����������� ���� ��������
    Queue q3 = q1 + q2;
    std::cout << "���������������� ����������� (q1 + q2): ";
    q3.display();

    // ����: ������������ ����������� ���� ��������
    Queue q4 = q1 * q2;
    std::cout << "������������ ����������� (q1 * q2): ";
    q4.display();

    // ����: �������� �������
    Queue q5 = ~q1;
    std::cout << "�������� ������� (~q1): ";
    q5.display();

    // ����: ������� YaZanimal (������� b ����� a)
    q1.YaZanimal("a2", "b1");
    std::cout << "������� ����� YaZanimal(a2, b1): ";
    q1.display();

    return 0;
}