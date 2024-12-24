#ifndef SORT_UTILS_H
#define SORT_UTILS_H

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <thread>
#include <chrono>
#include <iomanip>

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

//================/ Методы для типа int \================

//-----------------/ Генерация значений \----------------
/// Генерация случайных данных int
std::vector<int> generate_data(size_t size);

//--------------------/ Quick Sort \--------------------
/// Функция для разделения массива на две части
int partition(std::vector<int>& arr, int low, int high);

/// Быстроя сортировка (Quick Sort)
void quickSort(std::vector<int>& arr, int low, int high);

//------------------/ StableSort Sort \------------------
/// Стаблильная сортировка (StableSort Sort)
void stableSort(std::vector<int>& arr, int left, int right);

//---------------------/ Heap Sort \------------------
/// Функция для перестройки кучи
void heapify(std::vector<int>& arr, int n, int i);

/// Сортировка кучей (Heap Sort)
void heapSort(std::vector<int>& arr);

/// Функция слияния двух частей массива
void merge(std::vector<int>& arr, int left, int mid, int right);

//--------------------/ Radix Sort \------------------
/// Функция для получения разряда числа
int getDigit(int number, int place);

/// Поразрядная сортировка (Radix Sort)
void radixSort(std::vector<int>& arr);

//================/ Методы для типа string \================

//-----------------/ Генерация значений \----------------
/// Функция для генерации случайной строки
std::string generateRandomString(size_t length);

/// Функция для генерации массива строк случайной длины
std::vector<std::string> generateRandomStrings(size_t count);

//--------------------/ Quick Sort \--------------------
/// Функция для разделения массива на две части
int partition(std::vector<std::string>& arr, int low, int high);

/// Быстрая сортировка (Quick Sort)
void quickSort(std::vector<std::string>& arr, int low, int high);

//------------------/ StableSort Sort \------------------
/// Функция слияния двух частей массива
void merge(std::vector<std::string>& arr, int left, int mid, int right);

/// Стабильная сортировка (StableSort Sort)
void mergeSort(std::vector<std::string>& arr, int left, int right);

//---------------------/ Heap Sort \------------------
/// Вспомогательная часть алгоритма пирамидальной сортировки (Рекурсия)
void heapify(std::vector<std::string>& arr, int n, int i);

/// Пирамидальная сортировка (Heap Sort)
void heapSort(std::vector<std::string>& arr);

//--------------------/ Radix Sort \------------------
/// Поразрядная сортировка для строк в векторе arr, используя символы строк на позиции index
void countingSort(std::vector<std::string>& arr, int index);

/// Поразрядная сортировка (Radix Sort)
void radixSort(std::vector<std::string>& arr);

//------------------/ Вывод результатов \------------------
struct SortResult {
   std::string name; /// Имя сортировки
   double time; /// Время выполнения
};
/// Функция для вывода результатов
void printSortResults(const std::string& type, const std::vector<SortResult>& results);


#endif // SORT_UTILS_H
