#include "sort_utils.h"

/// Генерация случайных данных int
std::vector<int> generate_data(size_t size) {
   std::vector<int> data(size);
   unsigned int seed = static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count());
   std::mt19937 gen(seed);
   std::uniform_int_distribution<> dis(1, 1000000);
   for (auto &x : data) {
      x = dis(gen);
   }
   return data;
}

/// Функция для генерации случайной строки
std::string generateRandomString(size_t length) {
   const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
   unsigned int seed = static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count());
   std::mt19937 generator(seed);
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

   unsigned int seed = static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count());
   std::mt19937 generator(seed);
   static std::uniform_int_distribution<> lengthDist(1, 10); // Длина строки от 1 до 10

   for (size_t i = 0; i < count; ++i) {
      size_t randomLength = lengthDist(generator);
      randomStrings.push_back(generateRandomString(randomLength));
   }
   return randomStrings;
}

/// Поразрядная сортировка для строк (Radix Sort многопоточность)
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
