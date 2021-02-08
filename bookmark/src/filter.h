#ifndef FILTER_H
#define FILTER_H

#include <functional>

namespace detail
{

template<typename Sender, typename Predictate, typename MessageType = typename Sender::value_type>
class FilterImpl
{
  public:
    using value_type = MessageType;

    FilterImpl(Sender&& sender, Predictate predictate);

    template<typename EmitFunction>
    void SetMessageHandler(EmitFunction emit_function);

    void Process(MessageType&& message);

  private:
    Sender sender_;
    Predictate predictate_;
    std::function<void(MessageType&&)> emit_function_;
};

template<typename Sender, typename Predictate, typename MessageType>
FilterImpl<Sender, Predictate, MessageType>::FilterImpl(Sender&& sender, Predictate predictate)
    : sender_{std::move(sender)},
      predictate_{predictate}
{}

template<typename Sender, typename Predictate, typename MessageType>
template<typename EmitFunction>
void FilterImpl<Sender, Predictate, MessageType>::SetMessageHandler(EmitFunction emit_function)
{
    emit_function_ = emit_function;

    sender_.SetMessageHandler([this](MessageType&& message){
        Process(std::move(message));
    });
}

template<typename Sender, typename Predictate, typename MessageType>
void FilterImpl<Sender, Predictate, MessageType>::Process(MessageType&& message)
{
    if (std::invoke(predictate_, message))
    {
        emit_function_(std::move(message));
    }
}

template<typename Predictate>
struct FilterHelper
{
    Predictate predictate;
};

}  // namespace detail

template<typename Predictate>
auto Filter(Predictate&& predictate)
{
    return detail::FilterHelper<Predictate>{std::forward<Predictate>(predictate)};
}

template<typename Sender, typename Predicate>
auto operator|(Sender&& sender, detail::FilterHelper<Predicate> filter)
{
    return detail::FilterImpl{std::forward<Sender>(sender), filter.predictate};
}

#endif  // FILTER_H
