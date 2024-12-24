#include "sort_utils.h"

/// Генерация случайных данных int
std::vector<int> generate_data(size_t size) {
   std::vector<int> data(size);
   std::random_device rd;
   std::mt19937 gen(rd());
   std::uniform_int_distribution<> dis(1, 1000000);
   for (auto &x : data) {
      x = dis(gen);
   }
   return data;
}

/// Функция для разделения массива на две части
int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high]; // Выбираем последний элемент как опорный
    int i = low - 1;       // Индекс меньшего элемента

    for (int j = low; j < high; j++) {
        // Если текущий элемент меньше или равен опорному
        if (arr[j] <= pivot) {
            i++; // Увеличиваем индекс меньшего элемента
            if (i >= 0 && i < arr.size() && j >= 0 && j < arr.size()) {
                std::swap(arr[i], arr[j]);
            } else {
                std::cerr << "Ошибка: выход за пределы массива (i = " << i << ", j = " << j << ")" << std::endl;
                exit(1); // Завершаем программу
            }
        }
    }
    // Помещаем опорный элемент на правильную позицию
    if (i + 1 >= 0 && i + 1 < arr.size() && high >= 0 && high < arr.size()) {
        std::swap(arr[i + 1], arr[high]);
    } else {
        std::cerr << "Ошибка: выход за пределы массива при установке опорного элемента" << std::endl;
        exit(1);
    }
    return i + 1; // Возвращаем индекс опорного элемента
}

/// Быстроя сортировка (Quick Sort)
void quickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high); // Индекс опорного элемента

        // Рекурсивно сортируем элементы до и после разделения
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

/// Стаблильная сортировка (StableSort Sort)
void stableSort(std::vector<int> &arr, int left, int right)
{
   if (left < right) {
       int mid = left + (right - left) / 2;
       stableSort(arr, left, mid);
       stableSort(arr, mid + 1, right);
       merge(arr, left, mid, right);
   }
}

/// Функция для перестройки кучи
void heapify(std::vector<int> &arr, int n, int i) {
   int largest = i;          // Изначально считаем текущий узел наибольшим
   int left = 2 * i + 1;     // Левый дочерний элемент
   int right = 2 * i + 2;    // Правый дочерний элемент

   // Если левый дочерний элемент больше текущего узла
   if (left < n && arr[left] > arr[largest])
      largest = left;

   // Если правый дочерний элемент больше текущего узла
   if (right < n && arr[right] > arr[largest])
      largest = right;

   // Если наибольший элемент не текущий узел, меняем их местами
   if (largest != i) {
      std::swap(arr[i], arr[largest]);
      // Рекурсивно перестраиваем затронутую поддерево
      heapify(arr, n, largest);
   }
}

/// Сортировка кучей (Heap Sort)
void heapSort(std::vector<int> &arr) {
   int n = arr.size();

   // Построение кучи (перестройка массива)
   for (int i = n / 2 - 1; i >= 0; i--)
      heapify(arr, n, i);

   // Извлечение элементов из кучи один за другим
   for (int i = n - 1; i > 0; i--) {
      // Перемещаем текущий корень в конец
      std::swap(arr[0], arr[i]);
      // Вызываем heapify на уменьшенной куче
      heapify(arr, i, 0);
   }
}

/// Функция слияния двух частей массива
void merge(std::vector<int> &arr, int left, int mid, int right)
{
   std::vector<int> temp; // Временный массив для слияния
   int i = left;
   int j = mid + 1;

   // Слияние двух половин
   while (i <= mid && j <= right) {
      if (arr[i] <= arr[j]) {
         temp.push_back(arr[i++]);
      } else {
         temp.push_back(arr[j++]);
      }
   }

   // Добавляем оставшиеся элементы
   while (i <= mid) {
      temp.push_back(arr[i++]);
   }
   while (j <= right) {
      temp.push_back(arr[j++]);
   }

   // Копируем результат обратно в оригинальный массив
   for (int k = 0; k < temp.size(); ++k) {
      arr[left + k] = temp[k];
   }

}

/// Функция для получения разряда числа
int getDigit(int number, int place) {
   return (number / place) % 10;
}

/// Поразрядная сортировка (Radix Sort)
void radixSort(std::vector<int> &arr) {
   // Найти максимальное число, чтобы определить количество разрядов
   int maxNumber = *std::max_element(arr.begin(), arr.end());
   int place = 1; // Текущий разряд (единицы, десятки, сотни и т.д.)

   // Пока есть разряды для сортировки
   while (maxNumber / place > 0) {
      // Временный массив для подсчёта
      std::vector<int> output(arr.size());
      int count[10] = {0}; // Массив подсчёта для цифр от 0 до 9

      // Подсчитываем количество чисел для каждого разряда
      for (int num : arr) {
         count[getDigit(num, place)]++;
      }

      // Преобразуем count в индекс
      for (int i = 1; i < 10; i++) {
         count[i] += count[i - 1];
      }

      // Заполняем временный массив в обратном порядке для устойчивости
      for (int i = arr.size() - 1; i >= 0; i--) {
         int digit = getDigit(arr[i], place);
         output[count[digit] - 1] = arr[i];
         count[digit]--;
      }

      // Копируем отсортированные данные обратно в исходный массив
      arr = output;

      // Переход к следующему разряду
      place *= 10;
   }
}

/// Функция для генерации случайной строки
std::string generateRandomString(size_t length) {
   const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
   static std::random_device rd;
   static std::mt19937 generator(rd());
   static std::uniform_int_distribution<> charDist(0, characters.size() - 1);

   std::string randomString;
   for (size_t i = 0; i < length; ++i) {
      randomString += characters[charDist(generator)];
   }
   return randomString;
}

/// Функция для генерации массива строк случайной длины
std::vector<std::string> generateRandomStrings(size_t count) {
   std::vector<std::string> randomStrings;
   randomStrings.reserve(count);

   static std::random_device rd;
   static std::mt19937 generator(rd());
   static std::uniform_int_distribution<> lengthDist(1, 10); // Длина строки от 1 до 10

   for (size_t i = 0; i < count; ++i) {
      size_t randomLength = lengthDist(generator);
      randomStrings.push_back(generateRandomString(randomLength));
   }
   return randomStrings;
}

/// Функция для разделения массива на две части
int partition(std::vector<std::string> &arr, int low, int high) {
   std::string pivot = arr[high];
   int i = low - 1;

   for (int j = low; j < high; j++) {
      if (arr[j] <= pivot) {
         i++;
         std::swap(arr[i], arr[j]);
      }
   }
   std::swap(arr[i + 1], arr[high]);
   return i + 1;
}

/// Быстрая сортировка (Quick Sort)
void quickSort(std::vector<std::string> &arr, int low, int high) {
   if (low < high) {
      int pivot = partition(arr, low, high);
      quickSort(arr, low, pivot - 1);
      quickSort(arr, pivot + 1, high);
   }
}

/// Функция слияния двух частей массива
void merge(std::vector<std::string> &arr, int left, int mid, int right) {
   std::vector<std::string> leftArr(arr.begin() + left, arr.begin() + mid + 1);
   std::vector<std::string> rightArr(arr.begin() + mid + 1, arr.begin() + right + 1);

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

/// Стабильная сортировка (StableSort Sort)
void mergeSort(std::vector<std::string> &arr, int left, int right) {
   if (left < right) {
      int mid = left + (right - left) / 2;
      mergeSort(arr, left, mid);
      mergeSort(arr, mid + 1, right);
      merge(arr, left, mid, right);
   }
}

/// Вспомогательная часть алгоритма пирамидальной сортировки (Рекурсия)
void heapify(std::vector<std::string> &arr, int n, int i) {
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

/// Пирамидальная сортировка (Heap Sort)
void heapSort(std::vector<std::string> &arr) {
   int n = arr.size();

   for (int i = n / 2 - 1; i >= 0; i--) {
      heapify(arr, n, i);
   }

   for (int i = n - 1; i > 0; i--) {
      std::swap(arr[0], arr[i]);
      heapify(arr, i, 0);
   }
}

/// Поразрядная сортировка для строк в векторе arr, используя символы строк на позиции index
void countingSort(std::vector<std::string> &arr, int index) {
   int n = arr.size();
   std::vector<std::string> output(n);
   int count[256] = {0};

   for (const auto& str : arr) {
      char key = index < str.size() ? str[index] : 0;
      count[key]++;
   }

   for (int i = 1; i < 256; i++) {
      count[i] += count[i - 1];
   }

   for (int i = n - 1; i >= 0; i--) {
      char key = index < arr[i].size() ? arr[i][index] : 0;
      output[count[key] - 1] = arr[i];
      count[key]--;
   }

   arr = output;
}

/// Поразрядная сортировка (Radix Sort)
void radixSort(std::vector<std::string> &arr) {
   int maxLen = 0;
   for (const auto& str : arr) {
      maxLen = std::max(maxLen, static_cast<int>(str.size()));
   }

   for (int i = maxLen - 1; i >= 0; i--) {
      countingSort(arr, i);
   }
}
