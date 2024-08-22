#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/asio.hpp>
#include <iostream>
#include <string>

class Server {
   public:
    Server(boost::asio::io_context& io_context, short port);

    void send(const std::string& message);
    void receive();

   private:
    void start_accept();

    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::socket socket_;
    std::string received_message_;
};

#endif  // SERVER_HPP
