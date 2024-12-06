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
    Node* front; //указатель на начало

public:
    Queue() : front(nullptr) {}

    // Метод 1: Добавление элемента в конец очереди
    void enqueue(const std::string& data) {
        Node* newNode = new Node(data);
        if (!front) {
            front = newNode; //новый узел становится началом
        }
        else {//если не пуста очередь
            Node* current = front;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode; //добавляем новый узел в конец
        }
    }

    // Метод 2: Просмотр и извлечение элемента из начала очереди
    std::string dequeue() {
        if (!front) return "Очередь пуста!";
        Node* temp = front;
        front = front->next;
        std::string data = temp->data;
        delete temp;
        return data;
    }

    // Метод 3: Проверка наличия элемента в очереди
    bool find(const std::string& data) {
        Node* current = front;
        while (current) {
            if (current->data == data) return true;
            current = current->next;
        }
        return false;
    }

    // Метод 4: Вычисление веса элемента (позиция в очереди)
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

    // Перегрузка оператора + для последовательного соединения двух очередей
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

    // Перегрузка оператора * для перекрестного соединения двух очередей
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

    // Унарный оператор ~ для разворота очереди
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

    // Метод YaZanimal для вставки элемента b после элемента a
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

    // Метод для отображения очереди
    void display() const {
        Node* current = front;
        while (current) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

    // Деструктор для освобождения памяти
    ~Queue() {
        while (front) {
            Node* temp = front;
            front = front->next;
            delete temp;
        }
    }
};

int main() {
    // Устанавливаем локаль для корректного отображения русского языка
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

    std::cout << "Очередь 1: ";
    q1.display();
    std::cout << "Очередь 2: ";
    q2.display();

    // Тест: последовательное объединение двух очередей
    Queue q3 = q1 + q2;
    std::cout << "Последовательное объединение (q1 + q2): ";
    q3.display();

    // Тест: перекрестное объединение двух очередей
    Queue q4 = q1 * q2;
    std::cout << "Перекрестное объединение (q1 * q2): ";
    q4.display();

    // Тест: разворот очереди
    Queue q5 = ~q1;
    std::cout << "Разворот очереди (~q1): ";
    q5.display();

    // Тест: функция YaZanimal (вставка b после a)
    q1.YaZanimal("a2", "b1");
    std::cout << "Очередь после YaZanimal(a2, b1): ";
    q1.display();

    return 0;
}