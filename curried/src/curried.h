#include <type_traits>
#include <tuple>

template<typename Function, typename... CapturedArgs>
class Curried
{
  private:
    using CapturedArgsTuple = std::tuple<std::decay_t<CapturedArgs>...>;

    template<typename... Args>
    static auto CaptureByCopy(Args&&... args)
    {
        return std::tuple<std::decay_t<Args>...>(std::forward<Args>(args)...);
    }

  public:
    Curried(Function function, CapturedArgs... args)
        : function_{function},
          captured_{CaptureByCopy(std::move(args)...)}
    {}

    Curried(Function function, std::tuple<CapturedArgs...> args)
        : function_(function)
        , captured_(args)
    {
    }

    template<typename... NewArgs>
    auto operator()(NewArgs&&... args) const
    {
        auto new_args{CaptureByCopy(std::forward<NewArgs>(args)...)};
        auto all_args{std::tuple_cat(captured_, std::move(new_args))};

        if constexpr (std::is_invocable_v<Function, CapturedArgs..., NewArgs...>)
        {
            return std::apply(function_, all_args);
        }
        else
        {
            return Curried<Function, CapturedArgs..., NewArgs...>(function_, all_args);
        }
    }

  private:
    Function function_;
    std::tuple<CapturedArgs...> captured_;
};
