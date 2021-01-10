#include "bind2nd.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

template<typename Collection>
void Print(const std::string& prefix, const Collection& collection)
{
   std::cout << prefix;

   std::for_each(std::begin(collection), std::end(collection), [](const auto& item){
      std::cout << item << " ";
   });

   std::cout << std::endl;
}

int main(int argc, char *argv[])
{
   std::vector<int> numbers{12, 87, 54, 62, 4, 21, 44};

   Print("Before partitioning: ", numbers);

   // Create an unary function from the binary function std::greater with the second argument binded to 42 and use it
   // within the std::partition algorithm.
   auto greater_than_42{Bind2nd(std::greater<int>(), 42)};
   std::partition(std::begin(numbers), std::end(numbers), greater_than_42);

   Print("After partitioning:  ", numbers);

   return 0;
}
