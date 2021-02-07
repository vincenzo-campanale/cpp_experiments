#include "transform.h"
#include "service.h"
#include "sink.h"
#include <algorithm>
#include <iostream>
#include <boost/asio.hpp>

int main()
{
   auto trim = [](const std::string& message){
      auto msg_out{message};

      msg_out.erase(msg_out.begin(), std::find_if(msg_out.begin(), msg_out.end(), [](unsigned char ch) {
         return !std::isspace(ch);
      }));

      msg_out.erase(std::find_if(msg_out.rbegin(), msg_out.rend(), [](unsigned char ch) {
         return !std::isspace(ch);
      }).base(), msg_out.end());

      return msg_out;
   };

   auto to_cerr = [](const std::string& message){
      std::cerr << message << std::endl;
   };

   boost::asio::io_service event_loop{};

   auto pipeline{Service{event_loop} | Transform(trim) | Sink(to_cerr)};

   std::cerr << "Service is running...\n";
   event_loop.run();

   return 0;
}
