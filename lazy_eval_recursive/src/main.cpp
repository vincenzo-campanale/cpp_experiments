#include "make_memoized_recursive.h"
#include <iostream>

int main()
{
   auto MemoizedFibonacci{MakeMemoizedRecursive<unsigned int(unsigned int)>([](auto& fib, unsigned int num){
      std::cout << " > Calculating " << num << std::endl;
      return num == 0 ? 0
         : num == 1 ? 1
         : fib(num -1) + fib(num - 2);
   })};

   std::cout << "Calling MemoizedFibonacci(8)" << std::endl;
   const auto result_of_8{MemoizedFibonacci(8)};
   std::cout << "MemoizedFibonacci(8) = " << result_of_8 << std::endl << std::endl;

   std::cout << "Calling MemoizedFibonacci(12)" << std::endl;
   const auto result_of_12{MemoizedFibonacci(12)};
   std::cout << "MemoizedFibonacci(12) = " << result_of_12 << std::endl << std::endl;

   std::cout << "Calling MemoizedFibonacci(5)" << std::endl;
   const auto result_of_5{MemoizedFibonacci(5)};
   std::cout << "MemoizedFibonacci(5) = " << result_of_5 << std::endl << std::endl;

   return 0;
}
