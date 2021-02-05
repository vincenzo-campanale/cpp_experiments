#include "curried.h"
#include <functional>
#include <iostream>

class SumTest {
  public:
    template <typename T1, typename T2, typename T3>
    auto operator() (T1 x, T2 y, T3 z) const
    {
        return x + y + z;
    }

    template <typename T1, typename T2>
    auto operator() (T1 x, T2 y) const
    {
        return x + y;
    }
};

int main()
{
    auto less_curried = Curried(std::less<>());
    std::cout << less_curried(42, 1) << std::endl;

    auto greater_than_42 = less_curried(42);
    std::cout << greater_than_42(1.0) << std::endl;
    std::cout << greater_than_42(100.0) << std::endl;

    SumTest sum_test;

    auto sum_test_curried = Curried(sum_test);
    std::cout << sum_test_curried(1)(2,3) << std::endl;

    auto sum_test_curried_one = Curried(sum_test_curried, 1);
    std::cout << sum_test_curried_one(2,3) << std::endl;

    return 0;
}
