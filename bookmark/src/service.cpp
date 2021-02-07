#include "service.h"

#include "session.h"
#include <boost/asio.hpp>
#include <iostream>

Service::Service(boost::asio::io_service& service, unsigned short port)
    : acceptor_{service,  boost::asio::ip::tcp::endpoint{boost::asio::ip::tcp::v4(), port}},
      socket_{service}
{}

void Service::DoAccept()
{
    acceptor_.async_accept(socket_, [this](const boost::system::error_code& error){
        if (!error)
        {
            MakeSharedSession(std::move(socket_), emit_function_)->Start();
        }
        else
        {
            std::cerr << error.message() << std::endl;
        }

        DoAccept();   //< Listen to another client.
    });
}
