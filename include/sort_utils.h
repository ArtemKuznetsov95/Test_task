#ifndef SORT_UTILS_H
#define SORT_UTILS_H

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <thread>
#include <chrono>
#include <iomanip>

enum Sort {
   QUICK_THREAD = 10,
   QUICK_MULTITHREAD = 11,
   STABLE_THREAD = 21,
   STABLE_MULTITHREAD = 22,
   HEAP_THREAD = 31,
   HEAP_MULTITHREAD = 32,
   RABIX_THREAD = 41,
   RABIX_MULTITHREAD = 42
};

///  Функция для замеров времени
template <typename Func, typename T>
double measure_time(const std::string& name, Func func, std::vector<T>& data) {
    auto start = std::chrono::high_resolution_clock::now();
    func(data); // Вызов переданной функции
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << name << " completed in " << duration.count() << " sec.\n";

    return duration.count();
}

/// Функция для разбиения массива
template <typename T>
int partition(std::vector<T>& arr, int low, int high) {
    T pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
            std::swap(arr[++i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

//-----------------/ Генерация значений \----------------
/// Генерация случайных данных int
std::vector<int> generate_data(size_t size);

/// Функция для генерации случайной строки
std::string generateRandomString(size_t length);

/// Функция для генерации массива строк случайной длины
std::vector<std::string> generateRandomStrings(size_t count);

//--------------------/ Quick Sort \--------------------

/// Быстроя сортировка (Quick Sort)
template <typename T>
void quickSort(std::vector<T>& arr, int low, int high) {
   if (low < high) {
      int pivot = partition(arr, low, high);
      quickSort(arr, low, pivot - 1);
      quickSort(arr, pivot + 1, high);
   }
}

/// Быстроя сортировка (Quick Sort многопоточность)
// Рекурсивная функция быстрой сортировки
template <typename T>
void quickSort(std::vector<T>& arr, int low, int high, int depth_limit) {
   if (low < high) {
      int pivotIndex = partition(arr, low, high);

      // Если есть запас потоков, запускаем рекурсию в потоках
      if (depth_limit > 0) {
         std::thread leftThread([&arr, low, pivotIndex, depth_limit]() {
            quickSort(arr, low, pivotIndex - 1, depth_limit - 1);
         });
         quickSort(arr, pivotIndex + 1, high, depth_limit - 1);
         leftThread.join();
      } else {
         quickSort(arr, low, pivotIndex - 1, 0);
         quickSort(arr, pivotIndex + 1, high, 0);
      }
   }
}

/// Вспомогательная функция для вызова сортировки
template <typename T>
void parallelQuickSort(std::vector<T>& arr) {
    int depth_limit = std::thread::hardware_concurrency(); // Лимит глубины рекурсии
    quickSort(arr, 0, arr.size() - 1, depth_limit);
}

//------------------/ StableSort Sort \------------------
/// Функция слияния двух частей массива
template <typename T>
void merge(std::vector<T>& arr, int left, int mid, int right) {
   std::vector<T> leftArr(arr.begin() + left, arr.begin() + mid + 1);
   std::vector<T> rightArr(arr.begin() + mid + 1, arr.begin() + right + 1);

   int i = 0, j = 0, k = left;

   while (i < leftArr.size() && j < rightArr.size()) {
      if (leftArr[i] <= rightArr[j]) {
         arr[k++] = leftArr[i++];
      } else {
         arr[k++] = rightArr[j++];
      }
   }

   while (i < leftArr.size()) arr[k++] = leftArr[i++];
   while (j < rightArr.size()) arr[k++] = rightArr[j++];
}

/// Стаблильная сортировка (StableSort Sort)
template <typename T>
void stableSort(std::vector<T>& arr, int left, int right) {
   if (left < right) {
       int mid = left + (right - left) / 2;
       stableSort(arr, left, mid);
       stableSort(arr, mid + 1, right);
       merge(arr, left, mid, right);
   }
}

// Рекурсивная функция сортировки слиянием
template <typename T>
void mergeSort(std::vector<T>& arr, int left, int right, int depth_limit) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        if (depth_limit > 0) {
            // Запускаем сортировку левой половины в отдельном потоке
            std::thread leftThread([&arr, left, mid, depth_limit]() {
                mergeSort(arr, left, mid, depth_limit - 1);
            });

            // Сортируем правую половину в текущем потоке
            mergeSort(arr, mid + 1, right, depth_limit - 1);

            // Ждем завершения потока
            leftThread.join();
        } else {
            // Без многопоточности, если достигнут лимит
            mergeSort(arr, left, mid, 0);
            mergeSort(arr, mid + 1, right, 0);
        }

        // Сливаем отсортированные части
        merge(arr, left, mid, right);
    }
}

// Вспомогательная функция для вызова сортировки
template <typename T>
void parallelMergeSort(std::vector<T>& arr) {
    int depth_limit = std::thread::hardware_concurrency(); // Лимит глубины рекурсии
    mergeSort(arr, 0, arr.size() - 1, depth_limit);
}

//---------------------/ Heap Sort \------------------
/// Функция для перестройки кучи
template <typename T>
void heapify(std::vector<T>& arr, int n, int i) {
   int largest = i;
   int left = 2 * i + 1;
   int right = 2 * i + 2;

   if (left < n && arr[left] > arr[largest]) largest = left;
   if (right < n && arr[right] > arr[largest]) largest = right;

   if (largest != i) {
      std::swap(arr[i], arr[largest]);
      heapify(arr, n, largest);
   }
}

/// Сортировка кучей (Heap Sort)
template <typename T>
void heapSort(std::vector<T>& arr) {
   int n = arr.size();

   for (int i = n / 2 - 1; i >= 0; i--) {
      heapify(arr, n, i);
   }

   for (int i = n - 1; i > 0; i--) {
      std::swap(arr[0], arr[i]);
      heapify(arr, i, 0);
   }
}

// Восстановление кучи
template <typename T>
void heapify(std::vector<T>& arr, int n, int i, int depth_limit) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        if (depth_limit > 0) {
            heapify(arr, n, largest, depth_limit - 1);
        } else {
            heapify(arr, n, largest, 0);  // Без многопоточности
        }
    }
}
/// Построение кучи
template <typename T>
void buildHeap(std::vector<T>& arr, int n, int depth_limit) {
    int startIdx = n / 2 - 1;
    for (int i = startIdx; i >= 0; --i) {
        heapify(arr, n, i, depth_limit);
    }
}

/// Вспомогательная функция для вызова сортировки
template <typename T>
void parallelHeapSort(std::vector<T>& arr) {
   int n = arr.size();
   int depth_limit = std::thread::hardware_concurrency();

   buildHeap(arr, n, depth_limit);

   for (int i = n - 1; i > 0; --i) {
      std::swap(arr[0], arr[i]);
      heapify(arr, i, 0, depth_limit);
   }
}

//================/ Методы для типа int \================

//--------------------/ Radix Sort \------------------
/// Функция для получения разряда числа
int getDigit(int number, int place);

/// Поразрядная сортировка (Radix Sort)
void radixSort(std::vector<int>& arr);

// Получение максимального элемента
template <typename T>
T getMax(const std::vector<T>& arr) {
    return *std::max_element(arr.begin(), arr.end());
}

// Поразрядная сортировка для чисел
void countingSortForInt(std::vector<int>& arr, int exp, int depth_limit);

// Поразрядная сортировка для строк
void countingSortForString(std::vector<std::string>& arr, int pos);

// Поразрядная сортировка для чисел
void radixSortMilti(std::vector<int>& arr);

// Поразрядная сортировка для строк
void radixSortMilti(std::vector<std::string>& arr);
//================/ Методы для типа string \================

//--------------------/ Radix Sort \------------------
/// Поразрядная сортировка для строк в векторе arr, используя символы строк на позиции index
void countingSort(std::vector<std::string>& arr, int index);

/// Поразрядная сортировка (Radix Sort)
void radixSort(std::vector<std::string>& arr);


// Функция для получения разряда числа
int getDigit(int number, int digitPosition);

// Функция для поразрядной сортировки с использованием одного потока
void countingSortByDigit(std::vector<int>& arr, int digitPosition, int start, int end);

// Многопоточная поразрядная сортировка
void parallelRadixSort(std::vector<int>& arr);


//------------------/ Вывод результатов \------------------
struct SortResult {
   std::string name; /// Имя сортировки
   double time; /// Время выполнения
};
/// Функция для вывода результатов
void printSortResults(const std::string& type, const std::vector<SortResult>& results);


#endif // SORT_UTILS_H
