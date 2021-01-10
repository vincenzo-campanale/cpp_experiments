#ifndef BIND2ND_H
#define BIND2ND_H

#include <utility>

namespace detail
{

template<typename F, typename T>
class Binder2nd
{
  private:
    F f_;   //< The binary function is stored within this wrapper
    T y_;   //< together with its 2nd argument binded.

  public:
    Binder2nd(F f, T y);

    template<typename P>
    auto operator()(P&& x) const -> decltype(f_(std::forward<P>(x), y_)); //< Unary call operator.
};

template<typename F, typename T>
Binder2nd<F, T>::Binder2nd(F f, T y)
    : f_{f},
      y_{y}
{
}

template<typename F, typename T>
template<typename P>
auto Binder2nd<F, T>::operator()(P&& x) const -> decltype(f_(std::forward<P>(x), y_))
{
    return f_(std::forward<P>(x), y_);
}

}  // namespace detail

// Binds a given argument y to a second parameter of the given binary function object f. That is, stores y within the
// resulting wrapper, which, if called, passes y as the second parameter of f.
//
// Note: this function template, whose only job is to make an instance of Binder2nd, allows the compiler to deduce the
//       types automatically (template argument deduction is currently supported for functions but not for classes).
// Clarification: the requirement of class template instantiation for the template arguments to be explicit defined has
//                been removed in C++17. However this standard hasn't been yet widely accepted, this example doesn't
//                rely on this feature.
template<typename F, typename T>
auto Bind2nd(F&& f, T&& y) -> detail::Binder2nd<F, T>
{
    return detail::Binder2nd<F, T>(std::forward<F>(f), std::forward<T>(y));
}

#endif  // BIND2ND_H
