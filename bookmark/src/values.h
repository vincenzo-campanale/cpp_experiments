#ifndef VALUES_H
#define VALUES_H

#include <algorithm>
#include <functional>
#include <initializer_list>
#include <vector>

template<typename T>
class Values
{
  public:
    using value_type = T;

    explicit Values(std::initializer_list<T> values);

    template<typename EmitFunction>
    void SetMessageHandler(EmitFunction emit_function);

  private:
    std::vector<T> values_;
    std::function<void(T&&)> emit_function_{};
};

template<typename T>
Values<T>::Values(std::initializer_list<T> values)
    : values_{values}
{}

template<typename T>
template<typename EmitFunction>
void Values<T>::SetMessageHandler(EmitFunction emit_function)
{
    emit_function_ = emit_function;

    std::for_each(std::begin(values_), std::end(values_), [&emit_function](T value){
        emit_function(std::move(value));
    });
}

#endif  // VALUES_H
