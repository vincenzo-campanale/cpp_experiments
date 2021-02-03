#include "type_utils.h"
#include <iostream>
#include <vector>

template<typename C, typename R = typename C::value_type>
R SumCollection(const C& collection)
{
    return std::accumulate(std::begin(collection), std::end(collection), R{});
}

template<typename C, typename R = contained_type_t<C>>
R SumIterable(const C& collection)
{
    return std::accumulate(std::begin(collection), std::end(collection), R{});
}

template<typename C>
auto Sum(const C& c)
{
    if constexpr (has_value_type<C>())
    {
        return SumCollection(c);
    }
    else if constexpr (is_iterable<C>())
    {
        return SumIterable(c);
    }
    else
    {
        static_assert(false_<C>(), "sum can be called only on collections");
    }
}

int main()
{
    // Uncomment this to make the compiler write the exact result of the contained_type meta-function.
    // error<contained_type_t<std::vector<std::string>>>();

    // Asserting that std::vector<std::string> contains values of type std::string.
    static_assert(is_same<contained_type_t<std::vector<std::string>>, std::string>(),
                  "Expected the contained_type to return std::string");

    // Calling the function sum on a vector of ints.
    // The result will be an int
    std::vector<int> xs = { 1, 2, 3, 4 };

    std::cout << SumIterable(xs) << std::endl;
    std::cout << SumCollection(xs) << std::endl;
    std::cout << Sum(xs) << std::endl;

    // Uncomment this to trigger the static_assert in the sum function.
    //Sum(1);

   return 0;
}
