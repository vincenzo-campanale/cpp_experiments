#include "lib/lazy_eval.h"
#include "lib/sample_function.h"
#include <iostream>


int main()
{
   const auto LazySampleFunction{MakeLazy(SampleFunction)};

   std::cout << "First call (computes the value): " << LazySampleFunction() << std::endl;
   std::cout << "Second call (returns the cached value): " << LazySampleFunction() << std::endl;

   return 0;
}
