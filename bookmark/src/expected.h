#ifndef EXPECTED_H
#define EXPECTED_H

#include <algorithm>
#include <stdexcept>

template<typename T, typename E>
class Expected
{
  protected:
    union
    {
        T value_;
        E error_;
    };

    bool is_valid_;

    Expected() {}

  public:
    ~Expected();
    Expected(const Expected& other);
    Expected(Expected&& other);
    Expected& operator=(Expected other);

    void Swap(Expected& other);

    template<typename... ConsParams>
    static Expected Success(ConsParams&& ...params);

    template<typename... ConsParams>
    static Expected Error(ConsParams&& ...params);

    operator bool() const;
    bool IsValid() const;

    T& Get();
    const T& Get() const;
    T* operator-> ();
    const T* operator-> () const;

    E& Error();
    const E& Error() const;

    template <typename F>
    void Visit(F f);
};

template<typename T, typename E>
Expected<T, E>::~Expected()
{
    if (is_valid_)
    {
        value_.~T();
    }
    else
    {
        error_.~E();
    }
}

template<typename T, typename E>
Expected<T, E>::Expected(const Expected& other)
    : is_valid_{other.is_valid_}
{
    if (is_valid_)
    {
        new (&value_) T{other.value_};
    }
    else
    {
        new (&error_) E{other.error_};
    }
}

template<typename T, typename E>
Expected<T, E>::Expected(Expected&& other)
    : is_valid_{other.is_valid_}
{
    if (is_valid_)
    {
        new (&value_) T{std::move(other.value_)};
    }
    else
    {
        new (&error_) E{std::move(other.error_)};
    }
}

template<typename T, typename E>
auto Expected<T, E>::operator=(Expected other) -> Expected&
{
    Swap(other);
    return *this;
}

template<typename T, typename E>
void Expected<T, E>::Swap(Expected& other)
{
    if (is_valid_)
    {
        if (other.is_valid_)
        {
            std::swap(value_, other.value_);
        }
        else
        {
            auto temp{std::move(other.error_)};
            other.error_.~E();
            new (&other.value_) T(std::move(value_));
            value_.~T();
            new (&error_) E(std::move(temp));
            std::swap(is_valid_, other.is_valid_);
        }
    }
    else
    {
        if (other.is_valid_)
        {
            other.Swap(*this);
        }
        else
        {
            std::swap(error_, other.error_);
            std::swap(is_valid_, other.is_valid_);
        }
    }
}

template<typename T, typename E>
template<typename... ConsParams>
auto Expected<T, E>::Success(ConsParams&& ...params) -> Expected
{
    Expected result{};
    result.is_valid_ = true;
    new (&result.value_) T{std::forward<ConsParams>(params)...};

    return result;
}

template<typename T, typename E>
template<typename... ConsParams>
auto Expected<T, E>::Error(ConsParams&& ...params) -> Expected
{
    Expected result{};
    result.is_valid_ = false;
    new (&result.error_) E{std::forward<ConsParams>(params)...};

    return result;
}

template<typename T, typename E>
Expected<T, E>::operator bool() const
{
    return is_valid_;
}

template<typename T, typename E>
bool Expected<T, E>::IsValid() const
{
    return is_valid_;
}

template<typename T, typename E>
T& Expected<T, E>::Get()
{
    if (!is_valid_)
    {
        throw std::runtime_error("expected<T, E> contains no value");
    }

    return value_;
}

template<typename T, typename E>
const T& Expected<T, E>::Get() const
{
    if (!is_valid_)
    {
        throw std::runtime_error("expected<T, E> contains no value");
    }

    return value_;
}

template<typename T, typename E>
T* Expected<T, E>::operator-> ()
{
    return &Get();
}

template<typename T, typename E>
const T* Expected<T, E>::operator-> () const
{
    return &Get();
}

template<typename T, typename E>
E& Expected<T, E>::Error()
{
    if (is_valid_)
    {
        throw std::runtime_error("There is no error in this expected<T, E>");
    }

    return error_;
}

template<typename T, typename E>
const E& Expected<T, E>::Error() const
{
    if (is_valid_)
    {
        throw std::runtime_error("There is no error in this expected<T, E>");
    }

    return error_;
}

template<typename T, typename E>
template <typename F>
void Expected<T, E>::Visit(F f)
{
    if (is_valid_)
    {
        f(value_);
    }
    else
    {
        f(error_);
    }
}

template<typename T,
         typename E,
         typename Function,
         typename ResultType = decltype(std::declval<Function>()(std::declval<T>()))>
ResultType MBind(const Expected<T, E>& exp, Function f)
{
    if (exp)
    {
        return std::invoke(f, exp.Get());
    }
    else
    {
        return ResultType::Error(exp.Error());
    }
}

#endif  // EXPECTED_H
