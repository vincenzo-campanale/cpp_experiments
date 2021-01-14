
#include <map>
#include <mutex>
#include <type_traits>
#include <tuple>
#include <utility>

namespace detail
{

class NullParam {};   //< Dummy class used in the constructor to avoid overload collision with copy-constructor.

template <typename Signature, typename F>
class MemoizedHelper;

template<typename Result, typename... Args, typename F>
class MemoizedHelper<Result(Args...), F>
{
  private:
    using FunctionType = F;
    using ArgsTupleType = std::tuple<std::decay_t<Args>...>;   //< std::decay() allows to make the class work with
                                                               //< string literals.

    FunctionType f_;
    mutable std::map<ArgsTupleType, Result> cache_{};   //< Defines the cache, and, because it's mutable synchronizes
    mutable std::recursive_mutex cache_mutex_{};        //< all the changes.

  public:
    template<typename Function>
    MemoizedHelper(Function&& f, NullParam)
      : f_{std::forward<Function>(f)}
    {}

    MemoizedHelper(MemoizedHelper&& other)
      : f_{other.f_}
    {}

    template<typename... InnerArgs>
    Result operator()(InnerArgs&&... args) const
    {
        std::unique_lock<std::recursive_mutex> lock{cache_mutex_};

        const auto args_tuple{std::make_tuple(args...)};
        const auto it{cache_.find(args_tuple)};             //< Searches for the cached value.
        const auto item_in_cache{std::end(cache_) != it};

        if (item_in_cache)
        {
            return it->second;
        }
        else
        {
            auto&& result{f_(*this, std::forward<InnerArgs>(args)...)};   //< If the cached value isn't found, calls f_
            cache_[args_tuple] = result;                                  //< and stores the result. Passes *this as the
            return result;                                                //< first argument: the function to be used
        }                                                                 //< for the recursive call.
    }
};

}  // namespace detail

template<typename Signature, typename F>
detail::MemoizedHelper<Signature, std::decay_t<F>> MakeMemoizedRecursive(F&& f)
{
    return {std::forward<F>(f), detail::NullParam{}};
}
