#include <iostream>
#include <string>
using namespace std;

struct Node {
    string data;
    Node* left;
    Node* right;

    Node(string value) : data(value), left(nullptr), right(nullptr) {}
    ~Node() {//деструктор
        left = nullptr;
        right = nullptr;
        data = "";
    }
};

class Stack
{
private:
    Node* head; //ссылка на верхний узел 
    // размер стека
public:
    Stack() : head(nullptr) {} // стеку присваивается верхний узел и размер

    ~Stack() { // Деструктор
        while (head) {
            Node* temp = head;
            head = head->right;
            delete temp; // Освобождаем память текущего узла
            if (head) {             
                head->left = nullptr;
            }
        }
        head = nullptr; // Обнуляем указатель для безопасности
    }

    void push(const std::string& data) //  добавление нового элемента   (1)
    {
        Node* newNode = new Node(data); //  новый узел с данными
        newNode->right = head; // Указываем следующий узел на текущий верхний узел
        if (head)
        {
            head->left = newNode; // Если стек не пуст, устанавливаем предшествующий узел для старого верхнего узла
        }
        head = newNode; // Обновляем верхний узел стека
    }

    std::string priz() //  Просмотр и извлечение элемента 
    {
        if (!head)
        {
            throw std::out_of_range("стек пуст");// Исключение, если стек пуст
        }
        std::string data = head->data; // Сохраняет данные верхнего узла
        Node* temp = head; // Временный указатель на верхний узел
        head = head->right; // Обновляет верхний узел на следующий
        if (head)
        {
            head->left = nullptr;// Устанавливает предшествующий узел для нового верхнего узла
        }

        delete temp; // Освобождает память, занятую старым верхним узлом
        return data; // Возвращает данные удаленного узла
    }
    bool proverka(const std::string& data)// Метод проверяющий содержится ли указанный элемент в стеке  (3).
    {

        Node* tamp = head; // Начинает с верхнего узла
        while (tamp)
        {
            if (tamp->data == data)
            {
                return true; //если элемент найден
            }
            else

                tamp = tamp->right;
        }
        return false; //если не найден
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
        Stack newStack;  // Создает новый стек
        Node* tamp = other.head;  // Начинает с верхнего узла другого стека
        while (tamp)
        {
            newStack.push(tamp->data);  // Добавляет элементы из другого стека
            tamp = tamp->right;  // Переходит к следующему узлу
        }
        tamp = head;  // Переходит к верхнему узлу текущего стека
        while (tamp) {
            newStack.push(tamp->data);  // Добавляет элементы из текущего стека
            tamp = tamp->right;  // Переходит к следующему узлу
        }
        while (newStack.depth() > 0) {
            result.push(newStack.priz());
        }
        return result;  // Возвращает новый стек
    }

    Stack operator*(const Stack& other) const  // Оператор перемножения
    {
        Stack result;
        Stack newStack;  // Создает новый стек
        Node* tamp1 = head;  // Начинает с верхнего узла текущего стека
        Node* tamp2 = other.head;  // Начинает с верхнего узла другого стека

        while (tamp1 || tamp2)
        {
            if (tamp1) {
                newStack.push(tamp1->data);  // Добавляет элемент из текущего стека
                tamp1 = tamp1->right;  // Переходит к следующему узлу
            }
            if (tamp2) {
                newStack.push(tamp2->data);  // Добавляет элемент из другого стека
                tamp2 = tamp2->right;  // Переходит к следующему узлу
            }
        }
        while (newStack.depth() > 0) {
            result.push(newStack.priz());
        }
        return result; // Возвращает новый стек
    }

    Stack operator-() const {
        Stack newStack;  // Создает новый стек
        Node* tamp = head;  // Начинает с верхнего узла
        while (tamp) {
            newStack.push(tamp->data);  // Добавляет элементы в новый стек
            tamp = tamp->right;  // Переходит к следующему узлу
        }
        return newStack;  // Возвращает новый стек
    }

    void print() const {
        Node* tamp = head;  // Начинает с верхнего узла
        while (tamp) {
            std::cout << tamp->data;  // Выводит данные узла
            if (tamp->right) {
                std::cout << " - ";  // Если есть следующий узел, добавляет стрелку
            }
            tamp = tamp->right;  // Переходит к следующему узлу
        }
        std::cout << std::endl;  // Печатает новую строку после завершения
    }


    void ClearE(const std::string& data)
    {
        Node* tamp = head;
        while (tamp)
        {
            if (tamp->data == data)
            {  // Если данные узла совпадают с искомыми
                Node* toDelete = tamp;  // Сохраняем указатель на узел для удаления
                if (tamp->left)
                {
                    tamp->left->right = tamp->right;  // Обновляем указатель предыдущего узла
                }
                else
                {
                    head = tamp->right;  // Если это был верхний узел, обновляем head
                    head->left = nullptr;
                }
                if (tamp->right) {
                    tamp->right->left = tamp->left;  // Обновляем указатель следующего узла
                }
                tamp = tamp->right;  // Переходим к следующему узлу
                delete toDelete;  // Освобождаем память
            }
            else {
                tamp = tamp->right;  // Переходим к следующему узлу
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
    //Поиск элемента а (вернуть T/F).
    if (stack1.proverka("s1") == 1)
        std::cout << "Проверка на наличие: T";

    else
        std::cout << "Проверка на наличие: F";
    std::cout << std::endl;
    std::cout << "Просмотр и извлечение элемента: " << stack1.priz() << std::endl;
    /////////////////////////
    //
    std::cout << "Глубина стека: " << stack1.depth() << std::endl;

    std::cout << "вес элемента: " << stack2.ves("p2") << std::endl;

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
    std::cout << "обратный Stack 2: ";
    stack5.print();
    //функция ClearE
    stack1.ClearE("s1");
    std::cout << "функция ClearE:  ";
    stack1.print();
}