#include "client.hpp"


Client::Client(boost::asio::io_context& io_context, const std::string& host,
               const std::string& port)
    : resolver_(io_context), socket_(io_context) {
    connect(host, port);
}

void Client::send(const std::string& message) {
    boost::asio::async_write(socket_, boost::asio::buffer(message),
                             [message](boost::system::error_code ec, std::size_t /*length*/) {
                                 if (!ec) {
                                     std::cout << "Message sent from client: " << message
                                               << std::endl;
                                 }
                             });
}

void Client::receive() {
    boost::asio::async_read_until(
        socket_, boost::asio::dynamic_buffer(received_message_), '\n',
        [this](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec) {
                std::cout << "Message received at client: " << received_message_ << std::endl;
                received_message_.clear();
                receive();
            }
        });
}

void Client::connect(const std::string& host, const std::string& port) {
    auto endpoints = resolver_.resolve(host, port);
    boost::asio::async_connect(
        socket_, endpoints, [this](boost::system::error_code ec, boost::asio::ip::tcp::endpoint) {
            if (!ec) {
                receive();  // Start receiving messages
            }
        });
}
