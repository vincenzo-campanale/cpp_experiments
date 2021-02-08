#ifndef JOIN_H
#define JOIN_H

#include <functional>
#include <list>

namespace detail
{

template<typename Sender,
         typename SourceMessageType = typename Sender::value_type,
         typename MessageType = typename SourceMessageType::value_type>
class JoinImpl
{
  public:
    using value_type = MessageType;

    JoinImpl(Sender&& sender);

    template <typename EmitFunction>
    void SetMessageHandler(EmitFunction emit_function);

    void ProcessMessage(SourceMessageType&& source);

  private:
    Sender sender_;
    std::function<void(MessageType&&)> emit_function_{};
    std::list<SourceMessageType> sources_{};
};

template<typename Sender, typename SourceMessageType, typename MessageType>
JoinImpl<Sender, SourceMessageType, MessageType>::JoinImpl(Sender&& sender)
    : sender_{std::move(sender)}
{
}

template<typename Sender, typename SourceMessageType, typename MessageType>
template <typename EmitFunction>
void JoinImpl<Sender, SourceMessageType, MessageType>::SetMessageHandler(EmitFunction emit_function)
{
    emit_function_ = emit_function;

    sender_.SetMessageHandler([this](SourceMessageType&& message){
        ProcessMessage(std::move(message));
    });
}

template<typename Sender, typename SourceMessageType, typename MessageType>
void JoinImpl<Sender, SourceMessageType, MessageType>::ProcessMessage(SourceMessageType&& source)
{
    sources_.emplace_back(std::move(source));
    sources_.back().SetMessageHandler(emit_function_);
}

struct JoinHelper {};

}  // namespace detail

auto Join()
{
    return detail::JoinHelper{};
}

template <typename Sender>
auto operator|(Sender&& sender, detail::JoinHelper)
{
    return detail::JoinImpl<Sender>(std::forward<Sender>(sender));
}

#endif  // JOIN_H
