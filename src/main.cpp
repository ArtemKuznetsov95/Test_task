#include <iostream>
#include <random>
#include <algorithm>
#include <thread>
#include <chrono>

#include "sort_utils.h"

int main()
{
   size_t data_size = 1000000;
   std::vector<int> data_int = generate_data(data_size);
   std::vector<std::string> data_string = generateRandomStrings(data_size);


   std::vector<int> data_copy_int_1 = data_int;
   std::vector<int> data_copy_int_2 = data_int;
   std::vector<int> data_copy_int_3 = data_int;
   std::vector<int> data_copy_int_4 = data_int;


   std::vector<std::string> data_copy_string_1 = data_string;
   std::vector<std::string> data_copy_string_2 = data_string;
   std::vector<std::string> data_copy_string_3 = data_string;
   std::vector<std::string> data_copy_string_4 = data_string;

   std::cout << "sorting with the [ INT ] type:" << std::endl;

   std::string sort_quick = "QuickSort ";
   std::string sort_stable = "StableSort ";
   std::string sort_heap = "HeapSort ";
   std::string sort_radix = "RadixSort ";

   std::vector<SortResult> int_results = {
      { sort_quick,
        measure_time(sort_quick, [](std::vector<int>& data_copy_int_1) {
           quickSort(data_copy_int_1, 0, data_copy_int_1.size() - 1);
        }, data_copy_int_1) },

      { sort_stable,
        measure_time(sort_stable, [](std::vector<int>& data_copy_int_2) {
           stableSort(data_copy_int_2, 0, data_copy_int_2.size() - 1);
        }, data_copy_int_2) },

      { sort_heap,
        measure_time(sort_heap, [](std::vector<int>& data_copy_int_3) {
           heapSort(data_copy_int_3);
        }, data_copy_int_3) },

      { sort_radix,
        measure_time(sort_radix, [](std::vector<int>& data_copy_int_4) {
           radixSort(data_copy_int_4);
        }, data_copy_int_4) }
   };

   std::cout << "\n";
   std::cout << "sorting with the [ STRING ] type" << std::endl;

   std::vector<SortResult> string_results = {
      { sort_quick,
        measure_time(sort_quick, [](std::vector<std::string>& data_copy_string_1) {
           quickSort(data_copy_string_1, 0, data_copy_string_1.size() - 1);
        }, data_copy_string_1) },

      { sort_stable,
        measure_time(sort_stable, [](std::vector<std::string>& data_copy_string_2) {
           mergeSort(data_copy_string_2, 0, data_copy_string_2.size() - 1);
        }, data_copy_string_2) },

      { sort_heap,
        measure_time(sort_heap, [](std::vector<std::string>& data_copy_string_3) {
           heapSort(data_copy_string_3);
        }, data_copy_string_3)},

      { sort_radix,
        measure_time(sort_radix, [](std::vector<std::string>& data_copy_string_4) {
           radixSort(data_copy_string_4);
        }, data_copy_string_4) }
   };

   std::cout << "\n";
   // Выводим результаты
   printSortResults("int", int_results);
   printSortResults("string", string_results);

	return 0;
}
