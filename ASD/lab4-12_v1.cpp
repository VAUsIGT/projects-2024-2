#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <fstream>
#include <string>

// Функция для вывода массива
void printArray(const std::vector<int>& arr) {
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

// Сортировка методом прочесывания (расчёска) - основан на пузырьке, но использует gap (разрыв), который уменьшается вплоть до 1
void combSort(std::vector<int>& arr) {
    int gap = arr.size(); // разрыв между проверяемыми равен массиву
    const double shrink = 1.3; // фактор уменьшения (найдено эмперически, спустя 200к тестов)
    bool sorted = false;

    while (!sorted) { //пока не отсортирован
        gap = int(gap / shrink); //разрыв уменьшаем в 1.3
        if (gap <= 1) {
            gap = 1; //чтобы пройтись по последним элементам, стоящим вплотную
            sorted = true;
        }
        for (size_t i = 0; i + gap < arr.size(); i++) {
            if (arr[i] > arr[i + gap]) { // сравнение двух чисел
                std::swap(arr[i], arr[i + gap]);  // рокировка
                sorted = false;
            }
        }
    }
}

// Сортировка вставками - сортируются от большего к меньшему (по порядку, но справа на лево) шаг за шагом
void insertionSort(std::vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); i++) { // начинаем с 1 и считаем отсортированным
        int key = arr[i]; // ключ. элемент, который нужно вставить в отсортированную часть
        int j = i - 1;

        while (j >= 0 && arr[j] > key) { // перемещаем элементы больше ключа на 1 вправо
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key; // ставим ключ на найденное место
    }
}

// Сортировка выбором - поиск минимального элемента и рокировка с первым (и далее) по счёту. После двигает границы (сорт\не сорт)
void selectionSort(std::vector<int>& arr) {
    for (size_t i = 0; i < arr.size() - 1; i++) { // перебор массива для поиска мин. элем. в несорт. i это граница
        size_t minIndex = i;
        for (size_t j = i + 1; j < arr.size(); j++) { // поиск в несорт. части мин. элем.
            if (arr[j] < arr[minIndex]) {  // элем в несорт меньше макс. в сорт?
                minIndex = j; 
            }
        }
        std::swap(arr[i], arr[minIndex]); // рокировка
    }
}


//// Сортировка Шелла                                                   //зачтена
//void shellSort(std::vector<int>& arr) {
//    int n = arr.size();
//
//    for (int gap = n / 2; gap > 0; gap /= 2) {
//        for (int i = gap; i < n; i++) {
//            int temp = arr[i];
//            int j;
//
//            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
//                arr[j] = arr[j - gap];
//            }
//
//            arr[j] = temp;
//        }
//    }
//}

// Поразрядная сортировка (Radix Sort)                                                 зачтена
void radixSort(std::vector<int>& arr) {
    auto getMax = [](const std::vector<int>& arr) -> int { // находим макс. элемент для определения разрядности
        return *std::max_element(arr.begin(), arr.end());   
        };

    auto countingSortByDigit = [](std::vector<int>& arr, int exp) { //сортировка по разрядам
        int n = arr.size();
        std::vector<int> output(n);
        int count[10] = { 0 }; //счётчик числа разряда

        for (int i = 0; i < n; i++) {
            int digit = (arr[i] / exp) % 10; // поиск цифры текущего разряда, exp - 1 10 100...
            count[digit]++;
        }

        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }

        for (int i = n - 1; i >= 0; i--) {
            int digit = (arr[i] / exp) % 10;
            output[count[digit] - 1] = arr[i];
            count[digit]--;
        }

        for (int i = 0; i < n; i++) {
            arr[i] = output[i];
        }
        };

    int maxVal = getMax(arr);

    for (int exp = 1; maxVal / exp > 0; exp *= 10) {
        countingSortByDigit(arr, exp);
    }
}

// Пирамидальная сортировка (Heap Sort)
void heapify(std::vector<int>& arr, int n, int i) {
    int largest = i;          // корень
    int left = 2 * i + 1;     // левый потомок
    int right = 2 * i + 2;    // правый

    // если левый потомок больше корня
    if (left < n && arr[left] > arr[largest])
        largest = left;

    // правый потомок больше корня
    if (right < n && arr[right] > arr[largest])
        largest = right;

    // корень не самый большой
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(std::vector<int>& arr) {
    int n = arr.size();

    // Построение кучи (heapify)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // Извлечение элементов из кучи
    for (int i = n - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]);  // Перемещение текущего корня в конец
        heapify(arr, i, 0);         // Восстановление свойств кучи
    }
}

// Сортировка слиянием (Merge Sort)
void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1; // размер левой части
    int n2 = right - mid; // правой

    std::vector<int> L(n1), R(n2); // временные массивы

    //копируем в массивы (левый, правый) основной массив
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int i = 0; i < n2; i++)
        R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left; // указатели для L, R, arr


    // меньший элемент ставим в начало основого массива (его части)
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    // ставим оставшийся в основной массив
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(std::vector<int>& arr, int left, int right) { //подаётся: массив, 0, макс.индекс;
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

// Быстрая сортировка (Quick Sort)
void quickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) { // содержит ли >1
        int pivot = arr[high]; //опорный элем.
        int i = low - 1;
        
        //элем-ты меньше pivot перемещаются влево, остальные остаются
        for (int j = low; j < high; j++) {
            if (arr[j] <= pivot) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[high]); // pivot перемещается на своё место

        int pi = i + 1;
        quickSort(arr, low, pi - 1); //тоже самое, но для левой части
        quickSort(arr, pi + 1, high); //для правой
    }
}

// Внешняя многофазная сортировка (External Multi-Phase Sort)
void externalSort(const std::string& inputFileName, const std::string& outputFileName, size_t memoryLimit) {
    std::ifstream inputFile(inputFileName);
    std::vector<int> buffer;
    std::vector<std::string> tempFiles;

    // Чтение входного файла и создание отсортированных временных файлов
    while (!inputFile.eof()) {
        int num;
        while (buffer.size() < memoryLimit && inputFile >> num) {
            buffer.push_back(num);
        }
        std::sort(buffer.begin(), buffer.end());

        std::string tempFileName = "temp_" + std::to_string(tempFiles.size()) + ".txt";
        std::ofstream tempFile(tempFileName);
        for (int n : buffer) {
            tempFile << n << " ";
        }
        tempFile.close();
        tempFiles.push_back(tempFileName);

        buffer.clear();
    }
    inputFile.close();

    // Слияние временных файлов в результирующий файл
    using MinHeapNode = std::pair<int, int>; // Пара значений: (значение, индекс файла)
    std::priority_queue<MinHeapNode, std::vector<MinHeapNode>, std::greater<>> minHeap;
    std::vector<std::ifstream> tempStreams(tempFiles.size());

    for (size_t i = 0; i < tempFiles.size(); i++) {
        tempStreams[i].open(tempFiles[i]);
        int num;
        if (tempStreams[i] >> num) {
            minHeap.emplace(num, i);
        }
    }
    std::ofstream outputFile(outputFileName);
    while (!minHeap.empty()) {
        MinHeapNode node = minHeap.top();
        minHeap.pop();

        int val = node.first;
        int index = node.second;

        outputFile << val << " ";

        int num;
        if (tempStreams[index] >> num) {
            minHeap.emplace(num, index);
        }
    }
    outputFile.close();

    for (auto& stream : tempStreams) {
        stream.close();
    }

    // Удаление временных файлов
    for (const auto& tempFileName : tempFiles) {
        std::remove(tempFileName.c_str());
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    // Ввод данных
    std::vector<int> arr;
    int n, choice;

    std::cout << "Введите количество элементов: ";
    std::cin >> n;

    std::cout << "Введите элементы: ";
    for (int i = 0; i < n; i++) {
        int num;
        std::cin >> num;
        arr.push_back(num);
    }

    std::cout << "Выберите метод сортировки:\n";
    std::cout << "4 - Метод прочесывания\n";
    std::cout << "5 - Сортировка вставками\n";
    std::cout << "6 - Сортировка выбором\n";
    //std::cout << "7 - Сортировка Шелла\n";
    std::cout << "8 - Поразрядная сортировка\n";
    std::cout << "9 - Пирамидальная сортировка\n";
    std::cout << "10 - Сортировка слиянием\n";
    std::cout << "11 - Быстрая сортировка\n";
    std::cout << "12 - Внешняя многофазная сортировка\n";
    std::cout << "Ваш выбор: ";
    std::cin >> choice;

    // Копия массива для вывода результатов всех методов
    std::vector<int> sortedArr = arr;

    switch (choice) {
    case 4:
        combSort(sortedArr);
        std::cout << "Отсортировано методом прочесывания: ";
        break;
    case 5:
        insertionSort(sortedArr);
        std::cout << "Отсортировано методом вставками: ";
        break;
    case 6:
        selectionSort(sortedArr);
        std::cout << "Отсортировано методом выбора: ";
        break;
        /*case 7:
            shellSort(sortedArr);
            std::cout << "Отсортировано методом Шелла: ";
            break;*/
    case 8:
        radixSort(sortedArr);
        std::cout << "Отсортировано методом поразрядной сортировки: ";
        break;
    case 9:
        heapSort(sortedArr);
        std::cout << "Отсортировано пирамидальной сортировкой: ";
        break;
    case 10:
        mergeSort(sortedArr, 0, sortedArr.size() - 1);
        std::cout << "Отсортировано методом слияния: ";
        break;
    case 11:
        quickSort(sortedArr, 0, sortedArr.size() - 1);
        std::cout << "Отсортировано быстрой сортировкой: ";
        break;
    case 12: {
        std::string inputFile = "input.txt";
        std::string outputFile = "output.txt";

        std::ofstream input(inputFile);
        for (int num : arr)
            input << num << " ";
    
        input.close();

        externalSort(inputFile, outputFile, 5); // Ограничение памяти для демонстрации
    }
        std::cout << "Отсортировано внешней сортировкой, результат в файле output.txt\n";
        return 0;
    default:
        std::cout << "Неверный выбор метода сортировки.\n";
        return 1;
    }

    // Вывод результата
    printArray(sortedArr);

    return 0;
}
