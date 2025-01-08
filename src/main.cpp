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

   std::vector<int> data_copy_int_multithreading_1 = data_int;
   std::vector<int> data_copy_int_multithreading_2 = data_int;
   std::vector<int> data_copy_int_multithreading_3 = data_int;


   std::vector<std::string> data_copy_string_1 = data_string;
   std::vector<std::string> data_copy_string_2 = data_string;
   std::vector<std::string> data_copy_string_3 = data_string;

   std::vector<std::string> data_copy_string_multithreading_1 = data_string;
   std::vector<std::string> data_copy_string_multithreading_2 = data_string;
   std::vector<std::string> data_copy_string_multithreading_3 = data_string;

   std::cout << "sorting with the [ INT ] type:" << std::endl;

   std::string sort_quick  = "QuickSort ";
   std::string sort_stable = "StableSort ";
   std::string sort_heap   = "HeapSort ";
   std::string milti       = "Milti";

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
        }, data_copy_int_3) }
   };

   std::cout << "\n";
   std::cout << "sorting with the [ INT ] type milti:" << std::endl;

   int_results.push_back({sort_quick + milti,
                      measure_time(sort_quick + milti, [](std::vector<int>& data_copy_int_multithreading_1) {
                         parallelQuickSort( data_copy_int_multithreading_1);
                      }, data_copy_int_multithreading_1)});
   int_results.push_back({sort_stable + milti,
                      measure_time(sort_stable + milti, [](std::vector<int>& data_copy_int_multithreading_2) {
                         parallelMergeSort( data_copy_int_multithreading_2);
                      }, data_copy_int_multithreading_2)});
   int_results.push_back({sort_heap + milti,
                      measure_time(sort_heap + milti, [](std::vector<int>& data_copy_int_multithreading_3) {
                         parallelHeapSort( data_copy_int_multithreading_3);
                      }, data_copy_int_multithreading_3)});


   std::cout << "\n";
   std::cout << "sorting with the [ STRING ] type" << std::endl;

   std::vector<SortResult> string_results = {
      { sort_quick,
        measure_time(sort_quick, [](std::vector<std::string>& data_copy_string_1) {
           quickSort(data_copy_string_1, 0, data_copy_string_1.size() - 1);
        }, data_copy_string_1) },

      { sort_stable,
        measure_time(sort_stable, [](std::vector<std::string>& data_copy_string_2) {
           stableSort(data_copy_string_2, 0, data_copy_string_2.size() - 1);
        }, data_copy_string_2) },

      { sort_heap,
        measure_time(sort_heap, [](std::vector<std::string>& data_copy_string_3) {
           heapSort(data_copy_string_3);
        }, data_copy_string_3)}
   };

   std::cout << "\n";
   std::cout << "sorting with the [ STRING ] type multi:" << std::endl;

   string_results.push_back({sort_quick + milti,
                      measure_time(sort_quick + milti, [](std::vector<std::string>& data_copy_string_multithreading_1) {
                         parallelQuickSort( data_copy_string_multithreading_1);
                      }, data_copy_string_multithreading_1)});
   string_results.push_back({sort_stable + milti,
                      measure_time(sort_stable + milti, [](std::vector<std::string>& data_copy_string_multithreading_2) {
                         parallelMergeSort( data_copy_string_multithreading_2);
                      }, data_copy_string_multithreading_2)});
   string_results.push_back({sort_heap + milti,
                      measure_time(sort_stable + milti, [](std::vector<std::string>& data_copy_string_multithreading_3) {
                         parallelHeapSort( data_copy_string_multithreading_3);
                      }, data_copy_string_multithreading_3)});

   std::cout << "\n";
   // Выводим результаты
   printSortResults("int", int_results);
   printSortResults("string", string_results);

	return 0;
}
