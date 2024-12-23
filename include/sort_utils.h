#ifndef SORT_UTILS_H
#define SORT_UTILS_H

#include <iostream>
#include <vector>
#include <random>
//#include <algorithm>
#include <thread>
#include <chrono>
//#include <string>

// Генерация случайных данных
std::vector<int> generate_data(size_t size);

// Быстрая сортировка
//void quickSort(std::vector<int>& data, int left, int right);

// Функция для разделения массива на две части
int partition(std::vector<int>& arr, int low, int high);

// Рекурсивная функция быстрой сортировки
void quickSort(std::vector<int>& arr, int low, int high);

// Стаблильная сортировка (Слияние)
void stableSort(std::vector<int>& data, int left, int right);

// Функция слияния двух частей массива
void merge(std::vector<int>& vec, int left, int mid, int right);

// Функция для замеров времени
template <typename Func>
void measure_time(const std::string& name, Func func, std::vector<int>& data) {
    auto start = std::chrono::high_resolution_clock::now();
    func(data); // Вызов переданной функции
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << name << " completed in " << duration.count() << " seconds.\n";
}

#endif // SORT_UTILS_H
