#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <functional>
#include <utility>

namespace detail
{

template<typename Sender,
         typename Transformation,
         typename SourceMessageType = typename Sender::value_type,
         typename MessageType = decltype(std::declval<Transformation>()(std::declval<SourceMessageType>()))>
class TransformImpl
{
  public:
    using value_type = MessageType;

    TransformImpl(Sender&& sender, Transformation transformation);

    template<typename EmitFunction>
    void SetMessageHandler(EmitFunction emit_function);

    void Process(SourceMessageType&& message) const;

  private:
    Sender sender_;
    Transformation transformation_;
    std::function<void(MessageType&&)> emit_function_;
};

template<typename Sender, typename Transformation, typename SourceMessageType, typename MessageType>
TransformImpl<Sender, Transformation, SourceMessageType, MessageType>::TransformImpl(Sender&& sender,
                                                                                     Transformation transformation)
    : sender_{std::move(sender)},
      transformation_{transformation}
{}

template<typename Sender, typename Transformation, typename SourceMessageType, typename MessageType>
template<typename EmitFunction>
void TransformImpl<Sender, Transformation, SourceMessageType, MessageType>::SetMessageHandler(EmitFunction emit_function)
{
    emit_function_ = emit_function;

    sender_.SetMessageHandler([this](SourceMessageType&& message){
        Process(std::move(message));
    });
}

template<typename Sender, typename Transformation, typename SourceMessageType, typename MessageType>
void TransformImpl<Sender, Transformation, SourceMessageType, MessageType>::Process(SourceMessageType&& message) const
{
    emit_function_(std::invoke(transformation_, std::move(message)));
}

template<typename Transformation>
struct TransformHelper
{
    Transformation function;
};

}  // namespace detail

template<typename Transformation>
auto Transform(Transformation&& transformation)
{
    return detail::TransformHelper<Transformation>{std::forward<Transformation>(transformation)};
}

template<typename Sender, typename Transformation>
auto operator|(Sender&& sender, detail::TransformHelper<Transformation> transform)
{
    return detail::TransformImpl<Sender, Transformation>(std::forward<Sender>(sender), transform.function);
}

#endif  // TRANSFORM_H
