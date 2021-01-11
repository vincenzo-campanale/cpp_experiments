#include <stdexcept>

//
// The Fibonacci cache class has the same API of std::vector used within the function but stores only the last two
// values.
//
class FibonacciCache
{
  public:
    unsigned int size() const
    {
        return size_;
    }

    void push_back(unsigned int value)
    {
        prev_ = last_;
        last_ = value;
        ++size_;
    }

    unsigned int operator[] (unsigned int value) const
    {
        if (value == (size_ - 1))
        {
            return last_;
        }
        else if (value == (size_ - 2))
        {
            return prev_;
        }
        else
        {
            throw std::runtime_error("Unexpected argument");
        }
    }

  private:
    unsigned int prev_{0};
    unsigned int last_{1};
    unsigned int size_{2};
};

static FibonacciCache cache;

unsigned int EfficientCachedFibonacci(unsigned int num)
{
    int result{0};

    if (num < cache.size())
    {
        result = cache[num];
    }
    else
    {
        result = EfficientCachedFibonacci(num - 1) + EfficientCachedFibonacci(num - 2);
        cache.push_back(result);
    }

    return result;
}
