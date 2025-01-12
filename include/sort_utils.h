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
void quickSort(std::vector<T>& arr, int left, int right) {
   if (left >= right) return;

   int i = left;
   int j = right;
   T pivot = arr[(left + right) / 2];  // Опорный элемент

   while (i <= j) {
       while (arr[i] < pivot) i++;
       while (arr[j] > pivot) j--;
       if (i <= j) {
           std::swap(arr[i], arr[j]);
           i++;
           j--;
       }
   }

   if (left < j) quickSort(arr, left, j);
   if (i < right) quickSort(arr, i, right);
}

/// Быстроя сортировка (Quick Sort многопоточность)
template <typename T>
void quickSort(std::vector<T>& arr, int left, int right, int numThreads) {
   if (left >= right) return;

   int i = left;
   int j = right;
   T pivot = arr[(left + right) / 2];  // Опорный элемент

   while (i <= j) {
      while (arr[i] < pivot) i++;
      while (arr[j] > pivot) j--;
      if (i <= j) {
         std::swap(arr[i], arr[j]);
         i++;
         j--;
      }
   }
   // Параллельная сортировка левой и правой частей
   std::vector<std::future<void>> futures;

   auto sortPart = [&](int l, int r) {
      if (numThreads > 0) {
         futures.push_back(std::async(std::launch::async, [&, l, r, numThreads]() {
            quickSort(arr, l, r, numThreads - 1);
         }));
      } else {
         quickSort(arr, l, r, 0);
      }
   };

   sortPart(left, j);
   sortPart(i, right);

   // Ожидание завершения всех потоков
   for (auto& future : futures) {
      future.get();
   }
}

/// Вспомогательная функция для вызова сортировки
template <typename T>
void parallelQuickSort(std::vector<T>& arr) {
   int numThreads = std::thread::hardware_concurrency();
   quickSort(arr, 0, arr.size() - 1, numThreads);
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

// Разбиение массива A[] на две части, сортировка обеих частей в B[], слияние в A[]
template <typename T>
void topDownMerge(const std::vector<T>& B, int iBegin, int iMiddle, int iEnd, std::vector<T>& A) {
    int i = iBegin, j = iMiddle;

    // Слияние двух половин
    for (int k = iBegin; k < iEnd; k++) {
        // Если левая половина еще есть и ее элемент меньше или равен правому
        if (i < iMiddle && (j >= iEnd || B[i] <= B[j])) {
            A[k] = B[i];
            i++;
        } else {
            A[k] = B[j];
            j++;
        }
    }
}
// Разбиение массива A[] на две части, сортировка обеих частей в B[], слияние в A[]
template <typename T>
void topDownSplitMerge(std::vector<T>& B, int iBegin, int iEnd, std::vector<T>& A) {
   if (iEnd - iBegin <= 1)             // если размер подмассива == 1
       return;                          // считаем его отсортированным

   // разбиение массива на две части
   int iMiddle = (iEnd + iBegin) / 2;  // находим середину
   // рекурсивно сортируем обе части из массива A[] в B[]
   topDownSplitMerge(A, iBegin,  iMiddle, B);  // сортировка левой части
   topDownSplitMerge(A, iMiddle,    iEnd, B);  // сортировка правой части
   // слияние отсортированных частей из массива B[] в A[]
   topDownMerge(B, iBegin, iMiddle, iEnd, A);
}
// Основная функция рекурсивного разбиения и слияния с поддержкой многопоточности
template <typename T>
void topDownSplitMerge(std::vector<T>& B, int iBegin, int iEnd, std::vector<T>& A, int maxThreads) {
    if (iEnd - iBegin <= 1)
        return;

    int iMiddle = (iEnd + iBegin) / 2;

    // Если можно использовать еще потоки, запускаем рекурсивную сортировку в отдельных потоках
    if (maxThreads > 1) {
       std::thread leftThread([&A, iBegin, iMiddle, &B, maxThreads]() {
           topDownSplitMerge<T>(A, iBegin, iMiddle, B, maxThreads / 2);
       });
        topDownSplitMerge(A, iMiddle, iEnd, B, maxThreads / 2);
        leftThread.join();
    } else {
        // Если потоков больше нет, выполняем сортировку последовательно
        topDownSplitMerge(A, iBegin, iMiddle, B, 1);
        topDownSplitMerge(A, iMiddle, iEnd, B, 1);
    }

    // Слияние отсортированных частей
    topDownMerge(B, iBegin, iMiddle, iEnd, A);
}
/// Стаблильная сортировка (StableSort Sort)
template <typename T>
void stableSort(std::vector<T>& A) {
   std::vector<T> B = A;
   int n = A.size();
   topDownSplitMerge(B, 0, n, A);
}

/// Стаблильная сортировка (StableSort Sort многопоточность)
template <typename T>
void parallelMergeSort(std::vector<T>& A) {
   int numThreads = std::thread::hardware_concurrency();  // Получаем количество доступных потоков
   if (numThreads == 0) numThreads = 2;  // Если информация о потоках недоступна, используем 2 потока

   std::vector<T> B = A;  // Создаем временный массив
   topDownSplitMerge(B, 0, A.size(), A, numThreads);
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
