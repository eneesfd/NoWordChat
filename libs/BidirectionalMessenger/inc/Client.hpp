#ifndef LIBS_BIDIRECTIONALMESSENGER_INC_CLIENT
#define LIBS_BIDIRECTIONALMESSENGER_INC_CLIENT

#include <boost/asio.hpp>
#include <iostream>
#include <string>

class Client {
   public:
    Client(boost::asio::io_context& io_context, const std::string& host, const std::string& port);

    void send(const std::string& message);
    void receive();

   private:
    void connect(const std::string& host, const std::string& port);

    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::ip::tcp::socket socket_;
    std::string received_message_;
};

#endif // LIBS_BIDIRECTIONALMESSENGER_INC_CLIENT
