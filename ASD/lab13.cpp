#include <iostream>
#include <string>
#include <fstream>

//создание хеш-таблицы с открытой адресацией и обработкой коллизий через хеширование кукушкой.
//поддерживает добавление, удаление, поиск и авто увеличение размера.

//Хеш-таблиица — структура данных, реализующая интерфейс ассоциативного массива, а именно, она позволяет хранить пары (ключ, значение)
//и выполнять три операции: операцию добавления новой пары, операцию удаления и операцию поиска пары по ключу 
//коллизия - элементы ссылаются на одну область и имеют общий хэш 
//для решения коллизии ссылаемся на некст элемент, пока свободный не найдём

int hash_by_horner(std::string& str, int t_size, int key);
//данные и состояние(удален или нет)
struct node {
    std::string str; //значение элемента
    bool state; //удалён\активен
    node(std::string& val) : str(val), state(true) {}
};

struct hashtable {
    int h_size_def = 10; //начальный размер массива
    double rehash_koef = 0.75; //коэффициент загрузки, при превышении которого вызывается resize

    node** arr; //массив указателей на узлы (основной массив)
    int size; //количество активных элементов
    int arr_size; //текущий размер массива
    int size_all; //общее количество элементов (включая удалённые)

    hashtable() { //конструктор создаёт массив
        arr_size = h_size_def; //с указанным размером
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
    //Удаляет элементы с пометкой state = false (во время ресайз или добавления элемтов)
    void rehash() {
        size_all = 0;
        size = 0;
        //новый массив, с которым сейчас работаем
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
    //добавление элемента
    bool add(std::string str) {
        //проверка нового места
        if (size + 1 > int(rehash_koef * arr_size)) {
            resize();
        }
        //много элементов удалено - делаем рехеш
        else if (size_all > 2 * size) {
            rehash();
        }
        //двойное хеширование
        int h1 = hash_by_horner(str, arr_size, arr_size + 1); // основное значение хеш-функции
        

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
    //помечает элементы удалёнными, не очищает память
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
    //изменение размера (Увеличивает размер массива вдвое и переносит все элементы в новый массив)
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
    //поиск элемента (Возвращает true, если элемент найден)
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
    //печать таблицы
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
    //вывод в файл
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
    int amount_els = 30; //колво ячеек

    for (int i = 2; i < amount_els; i++) { //добавляем числа от 2 до 30
        int a = i;
        std::cout << a << " ";
        ht.add(std::to_string(a));
    }
    std::cout << std::endl;

    ht.print(); //вывод таблицы
    std::cout << ht.add("frefw") << std::endl;
    ht.print();
    for (int i = 2; i < amount_els; i++) { //удаляем элементы 2-30
        int a = i;
        ht.remove(std::to_string(a));
    }
    ht.print();

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