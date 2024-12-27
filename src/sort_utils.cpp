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

void printSortResults(const std::string &type, const std::vector<SortResult> &results) {
   std::cout << "Type: " << type << "\n";

   // Сортируем результаты по времени (по возрастанию)
   std::vector<SortResult> sorted_results = results;
   std::sort(sorted_results.begin(), sorted_results.end(), [](const SortResult& a, const SortResult& b) {
      return a.time < b.time;
   });

   // Выводим топ-список
   for (size_t i = 0; i < sorted_results.size(); ++i) {
      std::cout << i + 1 << ". " << sorted_results[i].name << "\t"
                << std::fixed << std::setprecision(6) << sorted_results[i].time << " sec\n";
   }
   std::cout << "\n";
}


void countingSortForInt(std::vector<int> &arr, int exp,  int depth_limit) {
   int n = arr.size();
   std::vector<int> output(n);
   int count[10] = {0};

   for (int i = 0; i < n; ++i) {
       count[(arr[i] / exp) % 10]++;
   }

   for (int i = 1; i < 10; ++i) {
       count[i] += count[i - 1];
   }

   for (int i = n - 1; i >= 0; --i) {
       output[count[(arr[i] / exp) % 10] - 1] = arr[i];
       count[(arr[i] / exp) % 10]--;
   }

   for (int i = 0; i < n; ++i) {
       arr[i] = output[i];
   }

   if (depth_limit > 0) {
       // Возможность вызвать другую задачу по разряду в потоке
       std::thread([&]() { countingSortForInt(arr, exp * 10, depth_limit - 1); }).join();
   }
}

void countingSortForString(std::vector<std::string> &arr, int pos) {
   int n = arr.size();
   std::vector<std::string> output(n);
   int count[256] = {0}; // Для всех символов ASCII

   // Подсчитываем количество символов на текущей позиции
   for (int i = 0; i < n; ++i) {
      char ch = (pos < arr[i].size()) ? arr[i][pos] : 0; // Если позиции нет, считаем символ 0
      count[(int)ch]++;
   }

   // Накопленная сумма
   for (int i = 1; i < 256; ++i) {
      count[i] += count[i - 1];
   }

   // Строим отсортированный массив
   for (int i = n - 1; i >= 0; --i) {
      char ch = (pos < arr[i].size()) ? arr[i][pos] : 0;
      output[count[(int)ch] - 1] = arr[i];
      count[(int)ch]--;
   }

   // Копируем результат обратно в исходный массив
   for (int i = 0; i < n; ++i) {
      arr[i] = output[i];
   }
}

void radixSortMilti(std::vector<int> &arr) {
   int maxVal = getMax(arr);
   int depth_limit = std::thread::hardware_concurrency();

   for (int exp = 1; maxVal / exp > 0; exp *= 10) {
      countingSortForInt(arr, exp, depth_limit);
   }
}

void radixSortMilti(std::vector<std::string> &arr) {
   int maxLength = 0;
   for (const auto& str : arr) {
      maxLength = std::max(maxLength, (int)str.size());
   }

   // Обрабатываем символы с конца строки
   for (int pos = maxLength - 1; pos >= 0; --pos) {
      countingSortForString(arr, pos);
   }
}

// Функция для получения разряда числа
//int getDigit(int number, int digitPosition) {
//    return (number / static_cast<int>(std::pow(10, digitPosition))) % 10;
//}

// Функция для поразрядной сортировки с использованием одного потока
void countingSortByDigit(std::vector<int>& arr, int digitPosition, int start, int end) {
    const int base = 10; // Система счисления (десятичная)
    std::vector<int> output(end - start, 0);
    std::vector<int> count(base, 0);

    // Подсчитываем частоту появления цифр на текущем разряде
    for (int i = start; i < end; ++i) {
        int digit = getDigit(arr[i], digitPosition);
        count[digit]++;
    }

    // Считаем префиксные суммы для подсчета позиций
    for (int i = 1; i < base; ++i) {
        count[i] += count[i - 1];
    }

    // Создаем отсортированный массив для данного разряда
    for (int i = end - 1; i >= start; --i) {
        int digit = getDigit(arr[i], digitPosition);
        output[count[digit] - 1] = arr[i];
        count[digit]--;
    }

    // Копируем отсортированный массив обратно в исходный
    for (int i = start; i < end; ++i) {
        arr[i] = output[i - start];
    }
}

// Оптимизированная многопоточная поразрядная сортировка
void parallelRadixSort(std::vector<int>& arr) {
    int maxDigit = 32;  // Для 32-битных чисел

    // Параметры для многопоточности
    int numThreads = std::thread::hardware_concurrency();  // Используем количество доступных ядер
    int arraySize = arr.size();

    // Если массив слишком мал для многопоточности, используем однопоточную сортировку
    if (arraySize < 1000) {  // Порог для многопоточности, можно настроить
        numThreads = 1;
    }

    // Сортируем по каждому разряду
    for (int digitPosition = 0; digitPosition < maxDigit; ++digitPosition) {
        // Массив для потоков
        std::vector<std::thread> threads;

        // Разделяем работу между потоками
        int chunkSize = arraySize / numThreads;
        for (int i = 0; i < numThreads; ++i) {
            int start = i * chunkSize;
            int end = (i == numThreads - 1) ? arraySize : (i + 1) * chunkSize;

            // Запуск сортировки для каждого потока
            threads.push_back(std::thread([&, start, end]() {
                countingSortByDigit(arr, digitPosition, start, end);
            }));
        }

        // Ожидаем завершения всех потоков
        for (auto& t : threads) {
            t.join();
        }
    }
}
