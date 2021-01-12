#include <map>
#include <tuple>

// Memoized wrapper for a function pointer.
//
// Note: the function returns a functor in form of a lambda. The lambda has 2 member variables, namely the function
//       pointer and the cache. Note that the cache is passed by value to the lambda, i.e. the lambda owns a cache.
//
template<typename Result, typename... Args>
auto MakeMemoized(Result (*f)(Args...))
{
    std::map<std::tuple<Args...>, Result> cache{};

    return [f, cache](Args... args) mutable -> Result {

        const auto args_tuple{std::make_tuple(args...)};
        const auto it{cache.find(args_tuple)};

        if (std::end(cache) != it)
        {
            return it->second;
        }
        else
        {
            const auto result{f(args...)};
            cache[args_tuple] = result;
            return result;
        }
    };
}
