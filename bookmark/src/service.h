#ifndef SERVICE_H
#define SERVICE_H

#include <boost/asio.hpp>
#include <functional>
#include <string>

///
/// @brief Web service that can connect with multiple clients. For each client a session is generated that handles
///        string messages.
///
class Service
{
  public:
    using value_type = std::string;

    explicit Service(boost::asio::io_service& service, unsigned short port = 42042);

    Service(const Service& other) = delete;
    Service(Service&& other) = default;

    template<typename EmitFunction>
    void SetMessageHandler(EmitFunction emit_function);

  private:
    void DoAccept();

    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::socket socket_;
    std::function<void(std::string&&)> emit_function_{};
};

template<typename EmitFunction>
void Service::SetMessageHandler(EmitFunction emit_function)
{
    emit_function_ = emit_function;
    DoAccept();
}

#endif  // SERVICE_H
