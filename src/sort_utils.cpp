#include "sort_utils.h"

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

//void quickSort(std::vector<int> &data, int left, int right) {
//   if (left >= right) return;
//   int mid = data[(left + right) / 2];
//   int i = left, j = right;
//   while (i <= j) {
//      while (data[i] < mid) ++i;
//      while (data[j] > mid) --j;
//      if (i <= j) {
//         std::swap(data[i++], data[j--]);
//      }
//   }
//   std::thread left_thread, right_thread;

//   if (left < j)
//      left_thread = std::thread(quickSort, std::ref(data), left, j);

//   if (i < right)
//      right_thread = std::thread(quickSort, std::ref(data), i, right);

//   if (left_thread.joinable())
//      left_thread.join();

//   if (right_thread.joinable())
//      right_thread.join();
//}

void stableSort(std::vector<int> &data, int left, int right)
{
   if (left < right) {
       int mid = left + (right - left) / 2;
       stableSort(data, left, mid);
       stableSort(data, mid + 1, right);
       merge(data, left, mid, right);
   }
}

void merge(std::vector<int> &vec, int left, int mid, int right)
{
   std::vector<int> temp; // Временный массив для слияния
   int i = left;
   int j = mid + 1;

   // Слияние двух половин
   while (i <= mid && j <= right) {
      if (vec[i] <= vec[j]) {
         temp.push_back(vec[i++]);
      } else {
         temp.push_back(vec[j++]);
      }
   }

   // Добавляем оставшиеся элементы
   while (i <= mid) {
      temp.push_back(vec[i++]);
   }
   while (j <= right) {
      temp.push_back(vec[j++]);
   }

   // Копируем результат обратно в оригинальный массив
   for (int k = 0; k < temp.size(); ++k) {
      vec[left + k] = temp[k];
   }

}

// Функция для разделения массива на две части
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

// Рекурсивная функция быстрой сортировки
void quickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high); // Индекс опорного элемента

        // Рекурсивно сортируем элементы до и после разделения
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
