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

void quicksort(std::vector<int> &data, int left, int right) {
   if (left >= right) return;
   int mid = data[(left + right) / 2];
   int i = left, j = right;
   while (i <= j) {
      while (data[i] < mid) ++i;
      while (data[j] > mid) --j;
      if (i <= j) {
         std::swap(data[i++], data[j--]);
      }
   }
   std::thread left_thread, right_thread;

   if (left < j)
      left_thread = std::thread(quicksort, std::ref(data), left, j);

   if (i < right)
      right_thread = std::thread(quicksort, std::ref(data), i, right);

   if (left_thread.joinable())
      left_thread.join();

   if (right_thread.joinable())
      right_thread.join();
}
