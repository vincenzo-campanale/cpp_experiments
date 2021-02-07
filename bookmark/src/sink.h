#ifndef SINK_H
#define SINK_H

#include <functional>
#include <utility>

namespace detail
{

template<typename Sender, typename Function, typename MessageType = typename Sender::value_type>
class SinkImpl
{
  public:
    SinkImpl(Sender&& sender, Function function);
    void Process(MessageType&& message) const;

  private:
    Sender sender_;
    Function function_;
};

template<typename Sender, typename Function, typename MessageType>
SinkImpl<Sender, Function, MessageType>::SinkImpl(Sender&& sender, Function function)
    : sender_(std::move(sender)),
      function_(function)
{
    sender_.SetMessageHandler([this](MessageType&& message){
        Process(std::move(message));
    });
}

template<typename Sender, typename Function, typename MessageType>
void SinkImpl<Sender, Function, MessageType>::Process(MessageType&& message) const
{
    std::invoke(function_, std::move(message));
}

template<typename Function>
struct SinkHelper
{
    Function function;
};

}  // namespace detail

template<typename Function>
auto Sink(Function&& function)
{
    return detail::SinkHelper<Function>{std::forward<Function>(function)};
}

template<typename Sender, typename Function>
auto operator|(Sender&& sender, detail::SinkHelper<Function> sink)
{
    return detail::SinkImpl<Sender, Function>(std::forward<Sender>(sender), sink.function);
}

#endif  // SINK_H
