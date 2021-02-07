#include "dsl.h"
#include "person.h"
#include "vehicle.h"
#include <iostream>
#include <string>

int main()
{
   {
      using namespace person;

      Record martha{};

      With(martha)
      (
         name = "Martha",
         surname = "Jones"
      );

      std::cout << martha.name << std::endl;
   }

   {
      using namespace vehicle;

      Record car{};

      With(car)
      (
         model = "Ford T",
         color = "blue"
      );

      std::cout << car.Model() << " " << car.Color() << std::endl;

      With(car)
      (
         model = "Fiat Panda",
         color = ""   //< Empty string makes this update (and so the whole transaction) to fail.
                      //< This leaves all the car fields unchanged (copy-and-swap idiom).
      );

      std::cout << car.Model() << " " << car.Color() << std::endl;
   }

   return 0;
}
