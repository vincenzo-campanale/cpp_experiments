#include "bind2nd.h"
#include <algorithm>
#include <iostream>
#include <functional>
#include <vector>

template<typename Collection>
void Print(const Collection& collection)
{
   std::for_each(std::begin(collection), std::end(collection), [](const auto& item){
      std::cout << item << " ";
   });
}

int main(int argc, char *argv[])
{
   std::vector<int> numbers{12, 87, 54, 62, 4, 21, 44};

   std::cout << "Before partitioning: ";
   Print(numbers);
   std::cout << std::endl;

   // Create a new unary function from the binary function std::greater where the second argument is 42.
   auto greater_than_42{Bind2nd(std::greater<int>(), 42)};

   // Use the unary function greater_than_42 with the std::partition algorithm.
   std::partition(std::begin(numbers), std::end(numbers), greater_than_42);

   std::cout << "After partitioning:  ";
   Print(numbers);
   std::cout << std::endl;

   return 0;
}
