#include "make_memoized.h"
#include "lib/func.h"
#include <iostream>

int main()
{
   auto FuncMemoized{MakeMemoized(Func)};

   std::cout << "FuncMemoized(25, 8.0) = " << FuncMemoized(25, 8.0) << " (first call)" << std::endl;
   std::cout << "FuncMemoized(12, 4.0) = " << FuncMemoized(12, 4.0) << " (first call)" << std::endl;
   std::cout << "FuncMemoized(30, 10.0) = " << FuncMemoized(30, 10.0) << " (first call)" << std::endl;

   // Uses cached value.
   std::cout << "FuncMemoized(12, 4.0) = " << FuncMemoized(12, 4.0) << " (second call)" << std::endl;

   return 0;
}
