#include <iostream>
#include <string>
#include <fstream>

//�������� ���-������� � �������� ���������� � ���������� �������� ����� ����������� ��������.
//������������ ����������, ��������, ����� � ���� ���������� �������.

//���-�������� � ��������� ������, ����������� ��������� �������������� �������, � ������, ��� ��������� ������� ���� (����, ��������)
//� ��������� ��� ��������: �������� ���������� ����� ����, �������� �������� � �������� ������ ���� �� ����� 
//�������� - �������� ��������� �� ���� ������� � ����� ����� ��� 
//��� ������� �������� ��������� �� ����� �������, ���� ��������� �� �����

int hash_by_horner(std::string& str, int t_size, int key);
//������ � ���������(������ ��� ���)
struct node {
    std::string str; //�������� ��������
    bool state; //�����\�������
    node(std::string& val) : str(val), state(true) {}
};

struct hashtable {
    int h_size_def = 10; //��������� ������ �������
    double rehash_koef = 0.75; //����������� ��������, ��� ���������� �������� ���������� resize

    node** arr; //������ ���������� �� ���� (�������� ������)
    int size; //���������� �������� ���������
    int arr_size; //������� ������ �������
    int size_all; //����� ���������� ��������� (������� ��������)

    hashtable() { //����������� ������ ������
        arr_size = h_size_def; //� ��������� ��������
        size = 0;
        size_all = 0;
        arr = new node * [arr_size];
        //fulling arr with nullptr
        for (int i = 0; i < arr_size; i++) arr[i] = nullptr;
    }
    ~hashtable() {
        for (int i = 0; i < arr_size; i++) {
            if (arr[i]) delete arr[i];
        }
        delete[] arr;
    }
    //������� �������� � �������� state = false (�� ����� ������ ��� ���������� �������)
    void rehash() {
        size_all = 0;
        size = 0;
        //����� ������, � ������� ������ ��������
        node** arr2 = new node * [arr_size];
        for (int i = 0; i < arr_size; i++) {
            arr2[i] = nullptr;
        }
        //swapping els with orig arr( arr now with nullptr )
        std::swap(arr, arr2);
        //looking for not deleted el
        for (int i = 0; i < arr_size; i++) {
            //if not deleted so move into orig arr
            if (arr2[i] && arr2[i]->state) {
                add(arr2[i]->str);
            }
            //if deleted el so we continue moving on
        }
        //clearing memory
        for (int i = 0; i < arr_size; i++) {
            if (arr2[i]) delete arr2[i];
        }
        delete[] arr2;
    }
    //���������� ��������
    bool add(std::string str) {
        //�������� ������ �����
        if (size + 1 > int(rehash_koef * arr_size)) {
            resize();
        }
        //����� ��������� ������� - ������ �����
        else if (size_all > 2 * size) {
            rehash();
        }
        //������� �����������
        int h1 = hash_by_horner(str, arr_size, arr_size + 1); // �������� �������� ���-�������
        

        int i = 0; // for cheching how many we have checked
        int first_del = -1; // index of first deleted el
        //loooking for first deleted
        while (arr[h1] != nullptr && i < arr_size) {
            if (arr[h1]->str == str && arr[h1]->state) {
                return false;
            }
            if (!arr[h1]->state && first_del == -1)
                first_del = h1;
            h1 = (h1 + 1) % arr_size;
            i++;
        }
        //creating new node if havent found deleted
        if (first_del == -1) {
            arr[h1] = new node(str);
            size_all++;
        }
        else {
            arr[first_del]->str = str;
            arr[first_del]->state = true;
        }
        size++;
        return true;
    }
    //�������� �������� ���������, �� ������� ������
    bool remove(std::string str) {
        int h1 = hash_by_horner(str, arr_size, arr_size + 1);
        int i = 0;
        //looking for this el in the arr
        while (arr[h1] != nullptr && i < arr_size) {
            //if we find this el -> chage state into true(deleted)
            if (arr[h1]->str == str && arr[h1]->state) {
                arr[h1]->state = false;
                size--;
                return true;
            }
            h1 = (1 + h1) % arr_size;
            i++;
        }
        return false;
    }
    //��������� ������� (����������� ������ ������� ����� � ��������� ��� �������� � ����� ������)
    void resize() {
        int past_arr_size = arr_size;
        arr_size *= 2; //chage arr size
        size = 0;
        size_all = 0;
        node** arr2 = new node * [arr_size];
        for (int i = 0; i < arr_size; i++) {
            arr2[i] = nullptr;
        }
        std::swap(arr, arr2);
        //clearing of deleted
        for (int i = 0; i < past_arr_size; i++) {
            if (arr2[i] && arr2[i]->state) {
                add(arr2[i]->str);
            }
        }
        for (int i = 0; i < past_arr_size; i++) {
            if (arr2[i]) delete arr2[i];
        }
        delete[] arr2;
    }
    //����� �������� (���������� true, ���� ������� ������)
    bool find(std::string str) {
        int h1 = hash_by_horner(str, arr_size, arr_size + 1);
        int i = 0;
        while (arr[h1] != nullptr && i < arr_size) {
            if (arr[h1]->str == str && arr[h1]->state) {
                return true;
            }
            h1 = (1 + h1) % arr_size;
            i++;
        }
        return false;
    }
    //������ �������
    void print() {
        std::cout << arr_size << " -> size of arr\n_ -> empty/deleted\n";
        for (int i = 0; i < arr_size; i++) {
            if (arr[i] && arr[i]->state)
                std::cout << arr[i]->str << " ";
            else {
                std::cout << "_ ";
            }
        }
        std::cout << std::endl;
    }
    //����� � ����
    void print(std::string out) {
        std::ofstream out_f(out);
        out_f << arr_size << " -> size of arr\n_ -> empty/deleted\n";
        for (int i = 0; i < arr_size; i++) {
            if (arr[i] && arr[i]->state)
                out_f << arr[i]->str << " ";
            else {
                out_f << "_ ";
            }
        }
        out_f << std::endl;
        out_f.close();
    }
};
int main() {
    hashtable ht;
    int amount_els = 30; //����� �����

    for (int i = 2; i < amount_els; i++) { //��������� ����� �� 2 �� 30
        int a = i;
        std::cout << a << " ";
        ht.add(std::to_string(a));
    }
    std::cout << std::endl;

    ht.print(); //����� �������
    std::cout << ht.add("frefw") << std::endl;
    ht.print();
    for (int i = 2; i < amount_els; i++) { //������� �������� 2-30
        int a = i;
        ht.remove(std::to_string(a));
    }
    ht.print();

    std::ifstream in("in.txt");
    std::string str;
    //���������� ���� �� �����
    while (getline(in, str)) {
        std::cout << ht.add(str);
    }
    std::cout << std::endl;
    ht.print();
    in.close();
    ht.print("out.txt"); //������ � ��������������

    return 0;
}
//��� ������� �� ������ ������� (��� ������������ ������������� ����� �� �������)
int hash_by_horner(std::string& str, int t_size, int key) {
    int result = 0;
    for (int i = 0; i < str.length(); i++) {
        result = (key * result + str[i]) % t_size;
    }
    result = (result * 2 + 1) % t_size;
    return result;

}