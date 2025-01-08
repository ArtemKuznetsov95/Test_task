#ifndef SORT_UTILS_H
#define SORT_UTILS_H

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <thread>
#include <chrono>
#include <iomanip>
#include <future>
#include <queue>

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

/// Рекурсия слияния
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

/// Стаблильная сортировка (StableSort Sort многопоточность)
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

// Слияние двух отсортированных частей
template <typename T>
std::vector<T> mergeHeadSort(const std::vector<T>& left, const std::vector<T>& right) {
    std::vector<T> result;
    size_t i = 0, j = 0;

    while (i < left.size() && j < right.size()) {
        if (left[i] <= right[j]) {
            result.push_back(left[i]);
            i++;
        } else {
            result.push_back(right[j]);
            j++;
        }
    }

    // Добавляем оставшиеся элементы
    while (i < left.size()) {
        result.push_back(left[i]);
        i++;
    }

    while (j < right.size()) {
        result.push_back(right[j]);
        j++;
    }

    return result;
}

/// Сортировка кучей (Heap Sort многопоточность)
template <typename T>
void parallelHeapSort(std::vector<T>& arr) {
   int numThreads = std::thread::hardware_concurrency();
   int partSize = arr.size() / numThreads;
   std::vector<std::future<std::vector<T>>> futures;

   // Разделяем массив на части и сортируем их в потоках
   for (int i = 0; i < numThreads; i++) {
      int start = i * partSize;
      int end = (i == numThreads - 1) ? arr.size() : start + partSize;

      futures.push_back(std::async(std::launch::async, [start, end, &arr]() {
         std::vector<T> part(arr.begin() + start, arr.begin() + end);
         heapSort(part);
         return part;
      }));
   }

   // Собираем отсортированные части
   std::vector<std::vector<T>> sortedParts;
   for (auto& future : futures) {
      sortedParts.push_back(future.get());
   }

   // Сливаем отсортированные части
   while (sortedParts.size() > 1) {
      std::vector<T> left = sortedParts.back();
      sortedParts.pop_back();
      std::vector<T> right = sortedParts.back();
      sortedParts.pop_back();

      sortedParts.push_back(mergeHeadSort(left, right));
   }

   // Результат
   arr = sortedParts.front();
}

//------------------/ Вывод результатов \------------------
struct SortResult {
   std::string name; /// Имя сортировки
   double time; /// Время выполнения
};
/// Функция для вывода результатов
void printSortResults(const std::string& type, const std::vector<SortResult>& results);


#endif // SORT_UTILS_H
