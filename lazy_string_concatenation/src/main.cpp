#include "lazy_string_concatenation.h"
#include <iostream>

LazyStringConcatHelper<> lazy_concat{};   //< It's not possible to overload the operator+ on std::string, so we use
                                          //< a small trick to force the use of the concatenation structure by appending
                                          //< to an empty instance of it.
int main()
{
   const std::string name{"Ugo"};
   const std::string surname{"Fantozzi"};

   const std::string fullname{lazy_concat + surname + ", " + name};

   std::cout << fullname << std::endl;

   return 0;
}
