
//
// Implements the Fibonacci function recursively. This implementation is similar to CachedFibonacci() but it is also
// more memory efficient. In CachedFibonacci(), only the last two values are continuosly retrieved from the cache. This
// implementation emulates the std::vector API used by the cache but stores only the last two entries.
//
unsigned int EfficientCachedFibonacci(unsigned int num);
