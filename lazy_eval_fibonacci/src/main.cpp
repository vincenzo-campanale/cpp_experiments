#include "lib/plain_fibonacci.h"
#include "lib/cached_fibonacci.h"
#include "lib/efficient_cached_fibonacci.h"
#include <iostream>

static const unsigned int value{20};

int main()
{
   std::cout << "PlainFibonacci(" << value << ") = " << PlainFibonacci(value) << std::endl;
   std::cout << "CachedFibonacci(" << value << ") = " << CachedFibonacci(value) << std::endl;
   std::cout << "EfficientCachedFibonacci(" << value << ") = " << EfficientCachedFibonacci(value) << std::endl;

   return 0;
}
