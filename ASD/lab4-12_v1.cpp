#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <fstream>
#include <string>

// ������� ��� ������ �������
void printArray(const std::vector<int>& arr) {
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

// ���������� ������� ������������ (��������) - ������� �� ��������, �� ���������� gap (������), ������� ����������� ������ �� 1
void combSort(std::vector<int>& arr) {
    int gap = arr.size(); // ������ ����� ������������ ����� �������
    const double shrink = 1.3; // ������ ���������� (������� �����������, ������ 200� ������)
    bool sorted = false;

    while (!sorted) { //���� �� ������������
        gap = int(gap / shrink); //������ ��������� � 1.3
        if (gap <= 1) {
            gap = 1; //����� �������� �� ��������� ���������, ������� ��������
            sorted = true;
        }
        for (size_t i = 0; i + gap < arr.size(); i++) {
            if (arr[i] > arr[i + gap]) { // ��������� ���� �����
                std::swap(arr[i], arr[i + gap]);  // ���������
                sorted = false;
            }
        }
    }
}

// ���������� ��������� - ����������� �� �������� � �������� (�� �������, �� ������ �� ����) ��� �� �����
void insertionSort(std::vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); i++) { // �������� � 1 � ������� ���������������
        int key = arr[i]; // ����. �������, ������� ����� �������� � ��������������� �����
        int j = i - 1;

        while (j >= 0 && arr[j] > key) { // ���������� �������� ������ ����� �� 1 ������
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key; // ������ ���� �� ��������� �����
    }
}

// ���������� ������� - ����� ������������ �������� � ��������� � ������ (� �����) �� �����. ����� ������� ������� (����\�� ����)
void selectionSort(std::vector<int>& arr) {
    for (size_t i = 0; i < arr.size() - 1; i++) { // ������� ������� ��� ������ ���. ����. � ������. i ��� �������
        size_t minIndex = i;
        for (size_t j = i + 1; j < arr.size(); j++) { // ����� � ������. ����� ���. ����.
            if (arr[j] < arr[minIndex]) {  // ���� � ������ ������ ����. � ����?
                minIndex = j; 
            }
        }
        std::swap(arr[i], arr[minIndex]); // ���������
    }
}


//// ���������� �����                                                   //�������
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

// ����������� ���������� (Radix Sort)                                                 �������
void radixSort(std::vector<int>& arr) {
    auto getMax = [](const std::vector<int>& arr) -> int { // ������� ����. ������� ��� ����������� �����������
        return *std::max_element(arr.begin(), arr.end());   
        };

    auto countingSortByDigit = [](std::vector<int>& arr, int exp) { //���������� �� ��������
        int n = arr.size();
        std::vector<int> output(n);
        int count[10] = { 0 }; //������� ����� �������

        for (int i = 0; i < n; i++) {
            int digit = (arr[i] / exp) % 10; // ����� ����� �������� �������, exp - 1 10 100...
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

// ������������� ���������� (Heap Sort)
void heapify(std::vector<int>& arr, int n, int i) {
    int largest = i;          // ������
    int left = 2 * i + 1;     // ����� �������
    int right = 2 * i + 2;    // ������

    // ���� ����� ������� ������ �����
    if (left < n && arr[left] > arr[largest])
        largest = left;

    // ������ ������� ������ �����
    if (right < n && arr[right] > arr[largest])
        largest = right;

    // ������ �� ����� �������
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(std::vector<int>& arr) {
    int n = arr.size();

    // ���������� ���� (heapify)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // ���������� ��������� �� ����
    for (int i = n - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]);  // ����������� �������� ����� � �����
        heapify(arr, i, 0);         // �������������� ������� ����
    }
}

// ���������� �������� (Merge Sort)
void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1; // ������ ����� �����
    int n2 = right - mid; // ������

    std::vector<int> L(n1), R(n2); // ��������� �������

    //�������� � ������� (�����, ������) �������� ������
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int i = 0; i < n2; i++)
        R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left; // ��������� ��� L, R, arr


    // ������� ������� ������ � ������ �������� ������� (��� �����)
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
    
    // ������ ���������� � �������� ������
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

void mergeSort(std::vector<int>& arr, int left, int right) { //�������: ������, 0, ����.������;
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

// ������� ���������� (Quick Sort)
void quickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) { // �������� �� >1
        int pivot = arr[high]; //������� ����.
        int i = low - 1;
        
        //����-�� ������ pivot ������������ �����, ��������� ��������
        for (int j = low; j < high; j++) {
            if (arr[j] <= pivot) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[high]); // pivot ������������ �� ��� �����

        int pi = i + 1;
        quickSort(arr, low, pi - 1); //���� �����, �� ��� ����� �����
        quickSort(arr, pi + 1, high); //��� ������
    }
}

// ������� ����������� ���������� (External Multi-Phase Sort)
void externalSort(const std::string& inputFileName, const std::string& outputFileName, size_t memoryLimit) {
    std::ifstream inputFile(inputFileName);
    std::vector<int> buffer;
    std::vector<std::string> tempFiles;

    // ������ �������� ����� � �������� ��������������� ��������� ������
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

    // ������� ��������� ������ � �������������� ����
    using MinHeapNode = std::pair<int, int>; // ���� ��������: (��������, ������ �����)
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

    // �������� ��������� ������
    for (const auto& tempFileName : tempFiles) {
        std::remove(tempFileName.c_str());
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    // ���� ������
    std::vector<int> arr;
    int n, choice;

    std::cout << "������� ���������� ���������: ";
    std::cin >> n;

    std::cout << "������� ��������: ";
    for (int i = 0; i < n; i++) {
        int num;
        std::cin >> num;
        arr.push_back(num);
    }

    std::cout << "�������� ����� ����������:\n";
    std::cout << "4 - ����� ������������\n";
    std::cout << "5 - ���������� ���������\n";
    std::cout << "6 - ���������� �������\n";
    //std::cout << "7 - ���������� �����\n";
    std::cout << "8 - ����������� ����������\n";
    std::cout << "9 - ������������� ����������\n";
    std::cout << "10 - ���������� ��������\n";
    std::cout << "11 - ������� ����������\n";
    std::cout << "12 - ������� ����������� ����������\n";
    std::cout << "��� �����: ";
    std::cin >> choice;

    // ����� ������� ��� ������ ����������� ���� �������
    std::vector<int> sortedArr = arr;

    switch (choice) {
    case 4:
        combSort(sortedArr);
        std::cout << "������������� ������� ������������: ";
        break;
    case 5:
        insertionSort(sortedArr);
        std::cout << "������������� ������� ���������: ";
        break;
    case 6:
        selectionSort(sortedArr);
        std::cout << "������������� ������� ������: ";
        break;
        /*case 7:
            shellSort(sortedArr);
            std::cout << "������������� ������� �����: ";
            break;*/
    case 8:
        radixSort(sortedArr);
        std::cout << "������������� ������� ����������� ����������: ";
        break;
    case 9:
        heapSort(sortedArr);
        std::cout << "������������� ������������� �����������: ";
        break;
    case 10:
        mergeSort(sortedArr, 0, sortedArr.size() - 1);
        std::cout << "������������� ������� �������: ";
        break;
    case 11:
        quickSort(sortedArr, 0, sortedArr.size() - 1);
        std::cout << "������������� ������� �����������: ";
        break;
    case 12: {
        std::string inputFile = "input.txt";
        std::string outputFile = "output.txt";

        std::ofstream input(inputFile);
        for (int num : arr)
            input << num << " ";
    
        input.close();

        externalSort(inputFile, outputFile, 5); // ����������� ������ ��� ������������
    }
        std::cout << "������������� ������� �����������, ��������� � ����� output.txt\n";
        return 0;
    default:
        std::cout << "�������� ����� ������ ����������.\n";
        return 1;
    }

    // ����� ����������
    printArray(sortedArr);

    return 0;
}
