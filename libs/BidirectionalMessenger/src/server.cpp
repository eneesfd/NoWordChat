#include "server.hpp"

#include <iostream>
#include <thread>

Server::Server(boost::asio::io_context& io_context, short port)
    : acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
      socket_(io_context) {
    start_accept();
}

void Server::send(const std::string& message) {
    boost::asio::async_write(socket_, boost::asio::buffer(message),
                             [message](boost::system::error_code ec, std::size_t /*length*/) {
                                 if (!ec) {
                                     std::cout << "Message sent from server: " << message
                                               << std::endl;
                                 }
                             });
}

void Server::receive() {
    boost::asio::async_read_until(
        socket_, boost::asio::dynamic_buffer(received_message_), '\n',
        [this](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec) {
                std::cout << "Message received at server: " << received_message_ << std::endl;
                received_message_.clear();
                receive();
            }
        });
}

void Server::start_accept() {
    acceptor_.async_accept(socket_, [this](boost::system::error_code ec) {
        if (!ec) {
            receive();  // Start receiving messages
        }
        start_accept();
    });
}
