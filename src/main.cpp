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

   measure_time("QuickSort", [](std::vector<int>& data_copy_int_1) {
          quickSort(data_copy_int_1, 0, data_copy_int_1.size() - 1);
      }, data_copy_int_1);

   measure_time("StableSort", [](std::vector<int>& data_copy_int_2) {
          stableSort(data_copy_int_2, 0, data_copy_int_2.size() - 1);
      }, data_copy_int_2);

   measure_time("HeapSort", [](std::vector<int>& data_copy_int_3) {
          heapSort(data_copy_int_3);
      }, data_copy_int_3);

   measure_time("RadixSort", [](std::vector<int>& data_copy_int_4) {
          radixSort(data_copy_int_4);
      }, data_copy_int_4);

   std::cout << "\n";
   std::cout << "sorting with the [ STRING ] type" << std::endl;

   measure_time("QuickSort", [](std::vector<std::string>& data_copy_string_1) {
          quickSort(data_copy_string_1, 0, data_copy_string_1.size() - 1);
      }, data_copy_string_1);

   measure_time("StableSort", [](std::vector<std::string>& data_copy_string_2) {
          mergeSort(data_copy_string_2, 0, data_copy_string_2.size() - 1);
      }, data_copy_string_2);

   measure_time("HeapSort", [](std::vector<std::string>& data_copy_string_3) {
          heapSort(data_copy_string_3);
      }, data_copy_string_3);

   measure_time("RadixSort", [](std::vector<std::string>& data_copy_string_4) {
          radixSort(data_copy_string_4);
      }, data_copy_string_4);


	return 0;
}
