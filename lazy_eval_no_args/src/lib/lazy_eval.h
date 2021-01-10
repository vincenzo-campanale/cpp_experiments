#include <functional>
#include <mutex>
#include <optional>
#include <utility>

namespace detail
{

//
// Stores the computation of a function (that has no arguments) and caches the result after it is executed
// (memoization). The class is immutable from the outside, i.e. its member functions are all const. However, internally,
// it is needed to change the cached value after the function call, so all cache-related member variables must be
// declared as mutable.
//
template<typename Function>
class LazyEval
{
  private:
    Function function_;
    mutable std::optional<decltype(function_())> cache_value_;
    mutable std::mutex cache_mutex_{};

  public:
    LazyEval(Function function);
    LazyEval(LazyEval&& other);
    auto operator() () const -> decltype(function_());
};

template<typename Function>
LazyEval<Function>::LazyEval(Function function)
    : function_{function}
{}

template<typename Function>
LazyEval<Function>::LazyEval(LazyEval&& other)
    : function_{std::move(other.function_)}
{
}

template<typename Function>
auto LazyEval<Function>::operator() () const -> decltype(function_())
{
   std::lock_guard<std::mutex> lock{cache_mutex_};  //< Forbids concurrent access to the cache.

   if (!cache_value_.has_value())
   {
      cache_value_ = std::invoke(function_);
   }

   return cache_value_.value();
}

}  // namespace detail

//
// Convenience function that creates a new LazyEval instance automatically deducing the type of the computation.
//
template<typename Function>
inline detail::LazyEval<Function> MakeLazy(Function&& function)
{
   return detail::LazyEval<Function>{std::forward<Function>(function)};
}
