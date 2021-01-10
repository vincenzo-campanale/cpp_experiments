#ifndef BIND2ND_H
#define BIND2ND_H

#include <type_traits>
#include <utility>

namespace detail
{

template<typename F, typename T>
class Binder2nd
{
  public:
    Binder2nd(F f, T y);

    template<typename P>
    auto operator()(P&& x) const -> std::result_of_t<F(P, T)>;

  private:
    F f_;
    T y_;   //< The 2nd argument of f() is stored within this wrapper and used within the call operator().
};

template<typename F, typename T>
Binder2nd<F, T>::Binder2nd(F f, T y)
    : f_{f},
      y_{y}
{
}

template<typename F, typename T>
template<typename P>
auto Binder2nd<F, T>::operator()(P&& x) const -> std::result_of_t<F(P, T)>
{
    return f_(std::forward<P>(x), y_);
}

}  // namespace detail

///
///@brief Binds a given argument y to a second parameter of the given binary function object f. That is, stores y within
///       the resulting wrapper, which, if called, passes y as the second parameter of f.
///
template<typename F, typename T>
auto Bind2nd(F&& f, T&& y) -> detail::Binder2nd<F, T>
{
    return detail::Binder2nd<F, T>(std::forward<F>(f), std::forward<T>(y));
}

#endif  // BIND2ND_H
