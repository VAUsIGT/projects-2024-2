#include <iostream>
#include <string>
#include <fstream>
#include <vector>

int hash_by_horner(std::string& str, int t_size, int key);
//данные и состояние(удален или нет)
struct node
{
    node* left = nullptr;
    node* right = nullptr;
    std::string str = "";
    bool state = true;

    node() {};
    node(std::string str) : str(str) {};
    ~node() {
        left = nullptr;
        delete left;
        right = nullptr;
        delete right;
    }
};
class list {
    node* head = nullptr;

public:
    //конструкторы
    list() {};
    list(list& other) {
        node* tmp = other.head;
        while (tmp) {
            push_back(tmp->str);
            tmp = tmp->right;
        }
    }
    //деструкторы
    ~list() {
        node* tmp = head;
        if (tmp == nullptr) return;
        while (tmp->right != nullptr) tmp = tmp->right;
        while (tmp != head) {
            node* del = tmp;
            tmp = tmp->left;
            tmp->right = nullptr;
            delete del;
        }
        delete tmp;
        head = nullptr;
    }
    //push into the back of the list
    void push_back(std::string str) {
        node* nd = new node(str);
        if (head == nullptr) {
            head = nd;
            return;
        }
        node* tmp = this->head;
        while (tmp->right != nullptr) tmp = tmp->right;
        tmp->right = nd;
        nd->left = tmp;
    }
    //push into the front of the list
    void push_front(std::string str) {
        node* nd = new node(str);
        if (head == nullptr) {
            head = nd;
            return;
        }
        node* tmp = head;
        nd->right = tmp;
        tmp->left = nd;
        head = nd;
        std::cout << "-- " << head->str << std::endl;
    }
    //returning true if el in the list
    bool search(std::string str) {
        node* tmp = head;
        while (tmp) {
            if (tmp->str == str) return 1;
            tmp = tmp->right;
        }
        return 0;
    }
    bool is_first_del() {
        return head->state;
    }
    bool make_delete_node(std::string str) {
        if (!search(str)) return false;
        int id = get_place(str);
        node* tmp = head;
        for (int i = 0; i < id; i++) {
            tmp = tmp->right;
        }
        tmp->state = false;
        return true;
    }
    //getting index of the el
    int get_place(std::string str) {
        if (!search(str)) return -1;
        node* tmp = head;
        int i = 0;
        while (tmp) {
            if (tmp->str == str) break;
            tmp = tmp->right;
            i++;
        }
        return i;
    }
    //getting amount of els in list
    int length() {
        if (!head) return 0;
        node* tmp = head;
        int i = 0;
        while (tmp) {
            tmp = tmp->right;
            i++;
        }
        return i;
    }
    //getting str from 1-st el
    std::string show_first() {
        node* tmp = head;
        return tmp->str;
    }
    //getting and popping last el
    std::string pop_first() {
        node* tmp = head;
        if (!tmp) {
            return "-1";
        }
        if (tmp->right == nullptr) {
            std::string str = tmp->str;
            head = nullptr;
            delete tmp;
            return str;
        }
        node* nd = tmp->right;
        std::string str = tmp->str;
        head = nd;
        delete tmp;
        nd->left = nullptr;
        return str;
    }
    //printing all els
    void print() {
        node* tmp = head;
        std::cout << "(";
        while (tmp != nullptr) {
            if (tmp->state)
                std::cout << tmp->str << " ";
            tmp = tmp->right;
        }
        std::cout << ")";
    }
    void print(std::ofstream& out) {
        node* tmp = head;
        out << "(";
        while (tmp != nullptr) {
            if (tmp->state)
                out << tmp->str << " ";
            tmp = tmp->right;
        }
        out << ")";
    }
    //other`s list el getting after this list
    list operator +(list& other) {
        list l = *this;
        node* tmp = other.head;
        while (tmp) {
            l.push_back(tmp->str);
            tmp = tmp->right;
        }
        return l;
    }
    //other`s list el getting after this list
    list operator += (list& other) {
        node* tmp2 = other.head;
        if (head == nullptr) {
            head = tmp2;
            return *this;
        }
        while (tmp2) {
            push_back(tmp2->str);
            tmp2 = tmp2->right;
        }

        return *this;
    }
    //crossing els from this and other lists
    list operator *(list& other) {
        node* tmp = head;
        node* tmp2 = other.head;
        list l;
        while (tmp || tmp2) {
            if (tmp) {
                l.push_back(tmp->str);
                tmp = tmp->right;
            }
            if (tmp2) {
                l.push_back(tmp2->str);
                tmp2 = tmp2->right;
            }
        }
        return l;
    }
    //inversing el in the list
    list operator -() {
        list l;
        while (head) {
            l.push_front(pop_first());
        }
        return l;
    }
    //equaliton
    list operator =(list& other) {
        node* tmp = other.head;
        while (tmp) {
            push_back(tmp->str);
            tmp = tmp->right;
        }
        return *this;
    }
};
struct hashtable {
    int h_size_def = 10; //default size
    double rehash_koef = 0.75; //when we make resize

    std::vector<list*> arr; // main arr of nodes
    int size; //how many els in the arr without deleted
    int arr_size; //size of arr
    int size_all; //all els with deleted
    int len_arrs = 0;

    hashtable() {
        arr_size = h_size_def;
        size = 0;
        size_all = 0;
        arr.resize(arr_size);
        for (int i = 0; i < arr_size; i++) arr[i] = nullptr;
    }
    hashtable(hashtable& other) {
        arr_size = other.arr_size;
        size = other.size;
        size_all = other.size_all;
        arr = other.arr;
    }
    ~hashtable() {

    }
    //removing deleted els
    void rehash() {
        size_all = 0;
        size = 0;
        node* nd;
        //new arr where we work now
        list** arr2 = new list * [arr_size];
        for (int i = 0; i < arr_size; i++) {
            arr2[i] = arr[i];
        }
        for (int i = 0; i < arr_size; i++) {
            arr[i] = nullptr;
        }
        for (int i = 0; i < arr_size; i++) {
            int len = arr[i]->length();
            for (int j = 0; j < len; j++) {
                node* tmp = new node(arr[i]->pop_first());
                if (tmp->state && tmp) add(tmp->str);
            }
        }

        //cleearing memory
        for (int i = 0; i < arr_size; i++) {
            delete arr2[i];
        }
        delete[] arr2;

    }
    //add new el
    bool add(std::string str) {
        // for(int i = 0; i < arr_size; i++){
        //     if(arr[i]) size++;
        // }
        //checking for a new place
        if (size + 1 > int(rehash_koef * arr_size)) {
            resize();
            //if too many deleted el we making rehash
        }
        else if (size_all > 2 * size) {
            rehash();
        }
        //double hash
        int h1 = hash_by_horner(str, arr_size, arr_size + 1); // -> main hash
        if (arr[h1] && arr[h1]->length() < 3) {
            if (arr[h1]->search(str)) return false;
            arr[h1]->push_back(str);
        }
        else if (arr[h1]) {
            while (arr[h1] != nullptr) {
                if (arr[h1] && arr[h1]->length() < 3) {
                    arr[h1]->push_back(str);
                }
                h1 = (h1 + 1) % arr_size;
            }
        }
        else {
            arr[h1] = new list;
            arr[h1]->push_back(str);
        }
        size++;
        return true;
    }
    //removing el
    bool remove(std::string str) {
        int h1 = hash_by_horner(str, arr_size, arr_size + 1);
        //looking for this el in the arr
        if (arr[h1]) {
            if (arr[h1]->search(str)) {
                if (arr[h1]->make_delete_node(str)) {
                    return true;
                }
            }
        }
        return false;
    }
    //resizing if too less place
    void resize() {
        int past_arr_size = arr_size;
        arr_size *= 2; //chage arr size
        size = 0;
        size_all = 0;
        std::vector<list*> arr2;
        arr2.resize(arr_size);
        for (int i = 0; i < arr_size; i++) {
            arr2[i] = nullptr;
        }

        arr.swap(arr2);

        for (int i = 0; i < past_arr_size; i++) {
            if (arr2[i] && arr2[i]->length() > 0) {
                arr[i] = new list;
                int len = arr2[i]->length();
                for (int j = 0; j < len; j++) {
                    if (arr2[i]->is_first_del()) arr[i]->push_back(arr2[i]->pop_first());
                }

            }
        }
    }
    //find the el
    bool find(std::string str) {
        int h1 = hash_by_horner(str, arr_size, arr_size + 1);
        int i = 0;

        if (arr[h1]) {
            if (arr[h1]->search(str)) return true;
        }
        return false;
    }
    //just printing
    void print() {
        std::cout << arr_size << " -> size of arr\n_ -> empty/deleted\n";
        node* tmp;
        for (int i = 0; i < arr_size; i++) {
            if (arr[i]) arr[i]->print();
            else std::cout << "_ ";
        }
        std::cout << std::endl;
    }
    void print(std::string out) {
        std::ofstream out_f(out);
        out_f << arr_size << " -> size of arr\n_ -> empty/deleted\n";
        node* tmp;
        for (int i = 0; i < arr_size; i++) {
            if (arr[i]) arr[i]->print(out_f);
            else out_f << "_ ";
        }
        out_f << std::endl;
        out_f.close();
    }
};
int main() {
    hashtable ht;
    int amount_els = 20;

    for (int i = 2; i < amount_els; i++) {
        int a = i;
        std::cout << a << " ";
        ht.add(std::to_string(a));
    }
    std::cout << std::endl;

    ht.print();
    std::cout << ht.add("frefw") << std::endl;
    std::cout << ht.add("frefw") << std::endl;
    ht.print();
    for (int i = 2; i < amount_els; i++) {
        int a = i;
        ht.remove(std::to_string(a));
    }
    ht.print();
    for (int i = 2; i < amount_els; i++) {
        int a = i;
        std::cout << a << " ";
        ht.add(std::to_string(a));
    }
    std::ifstream in("in.txt");
    std::string str;
    //добавление слов из файла
    while (getline(in, str)) {
        std::cout << ht.add(str);
    }
    std::cout << std::endl;
    ht.print();
    in.close();
    ht.print("out.txt"); //запись в результирующий

    return 0;
}
//хеш функция по методу Горнера (для равномерного распределения строк по таблице)
int hash_by_horner(std::string& str, int t_size, int key) {
    int result = 0;
    for (int i = 0; i < str.length(); i++) {
        result = (key * result + str[i]) % t_size;
    }
    result = (result * 2 + 1) % t_size;
    return result;

}