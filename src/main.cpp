#include <iostream>
#include <random>
#include <algorithm>
#include <thread>
#include <chrono>

#include "sort_utils.h"
int main()
{
   std::cout << "Start sort\n";

   size_t data_size = 100000;
   std::vector<int> data = generate_data(data_size);
//   std::vector<int> data = {1,3,2,4,1,5,3,4,8,5,1,3};

   std::vector<int> data_copy1 = data;

   measure_time("QuickSort", [](std::vector<int>& data) {
          quicksort(data, 0, data.size() - 1);
      }, data_copy1);

	return 0;
}
