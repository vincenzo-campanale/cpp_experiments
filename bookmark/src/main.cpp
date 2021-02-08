#include "expected.h"
#include "filter.h"
#include "join.h"
#include "json.hpp"
#include "mtry.h"
#include "transform.h"
#include "service.h"
#include "sink.h"
#include "values.h"
#include <boost/asio.hpp>
#include <algorithm>
#include <exception>
#include <iostream>
#include <string>

using ExpectedJson = Expected<nlohmann::json, std::exception_ptr>;

struct Bookmark
{
    std::string url{};
    std::string text{};
};

using ExpectedBookmark = Expected<Bookmark, std::exception_ptr>;

std::string to_string(const Bookmark& page)
{
    return "[" + page.text + "](" + page.url + ")";
}

ExpectedBookmark BookmarkFromJson(const nlohmann::json& data)
{
   return MTry([&]{
      return Bookmark{data.at("FirstURL"), data.at("Text")};
   });
}

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

   auto to_json = [](const std::string& message) -> ExpectedJson {
      return MTry([&]{
         return nlohmann::json::parse(message);
      });
   };

   boost::asio::io_service event_loop{};

   auto pipeline{Values<unsigned short>{42042, 42043, 42044}
                 | Transform([&](auto port){return Service{event_loop, port};})
                 | Join()
                 | Transform(trim)
                 | Filter([](const auto& message){ return ((message.length() > 0) && (message[0] != '#'));})
                 | Transform(to_json)
                 | Transform([] (const auto& exp) {
                    return MBind(exp, BookmarkFromJson);
                 })
                 | Sink([](const auto& exp_bookmark){
                    if (!exp_bookmark)
                    {
                       std::cerr << "ERROR: Request was not understood\n";
                    }
                    else
                    {
                       std::cerr << "OK: " << to_string(exp_bookmark.Get()) << std::endl;
                    }
                 })};

   std::cerr << "Service is running...\n";
   event_loop.run();

   return 0;
}
