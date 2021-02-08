#ifndef MTRY_H
#define MTRY_H

#include "expected.h"
#include <exception>
#include <type_traits>

template <typename F,
          typename Ret = typename std::result_of_t<F()>,
          typename Exp = Expected<Ret, std::exception_ptr>>
Exp MTry(F f)
{
    try
    {
        return Exp::Success(f());
    }
    catch (...)
    {
        return Exp::Error(std::current_exception());
    }
}

#endif  // MTRY_H
