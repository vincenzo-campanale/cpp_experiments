#include <vector>

//
// cache[0] = Fibonacci(0) = 0
// cache[1] = Fibonacci(1) = 1
// ...
// cache[n] = Fibonacci(n)
//
static std::vector<unsigned int> cache = {0, 1};

unsigned int CachedFibonacci(unsigned int num)
{
    int result{0};

    if (num < cache.size())
    {
        result = cache[num];
    }
    else
    {
        result = CachedFibonacci(num - 1) + CachedFibonacci(num - 2);
        cache.push_back(result);
    }

    return result;
}
