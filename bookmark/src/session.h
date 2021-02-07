#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>
#include <iostream>
#include <memory>

template<typename EmitFunction>
class Session : public std::enable_shared_from_this<Session<EmitFunction>>
{
  public:
    Session(boost::asio::ip::tcp::socket&& socket, EmitFunction emit_function);
    void Start();

  private:
    using SharedSession = std::enable_shared_from_this<Session<EmitFunction>>;

    void DoRead();

    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf data_{};
    EmitFunction emit_function_;
};

template<typename EmitFunction>
Session<EmitFunction>::Session(boost::asio::ip::tcp::socket&& socket, EmitFunction emit_function)
    : socket_{std::move(socket)},
      emit_function_{emit_function}
{}

template<typename EmitFunction>
void Session<EmitFunction>::Start()
{
    DoRead();
}

// Note: the session keeps its own lifetime. As soon as an error occurs and the client disconnects, the session should
// destroy itself. The session object inherits from std::enable_shared_from_this. This allows a session instance that's
// managed by std::shared_ptr to safely create additional shared pointers to itself. The shared pointer to the session
// is captured in the lambdas that process connection events. As long as there is an event the session is waiting for,
// the session object won't be deleted, because the lambda that handles that event will old an instance of the shared
// pointer. When there are no more events to process, the object will be deleted.
template<typename EmitFunction>
void Session<EmitFunction>::DoRead()
{
    auto self{SharedSession::shared_from_this()};   //< Create a pointer that has shared ownership with this session.

    boost::asio::async_read_until(socket_,   //< Schedule a lambda to be executed when the newline character is reached
                                  data_,     //< in the input.
                                  '\n',
                                  [this, self](const boost::system::error_code& error, std::size_t /*unused*/){
        if (!error)
        {
            std::istream is(&data_);
            std::string line{};
            std::getline(is, line);
            emit_function_(std::move(line));
            DoRead();   //< Schedule to read the next message.
        }
    });
}

template <typename Socket, typename EmitFunction>
auto MakeSharedSession(Socket&& socket, EmitFunction&& emit)
{
    return std::make_shared<Session<EmitFunction>>(std::forward<Socket>(socket), std::forward<EmitFunction>(emit));
}

#endif  // SESSION_H
