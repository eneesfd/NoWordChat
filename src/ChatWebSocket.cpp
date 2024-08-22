#include "ChatWebSocket.hpp"

#include <drogon/HttpTypes.h>
#include <drogon/WebSocketClient.h>
#include <drogon/WebSocketController.h>
#include <drogon/drogon.h>

#include <iostream>
#include <thread>

ChatWebSocket::ChatWebSocket() {
    // Initialize your server and client here if needed
}

void ChatWebSocket::handleNewMessage(const WebSocketConnectionPtr &wsConn, std::string &&message,
                                     const WebSocketMessageType &type) {
    std::cout << "New WebSocket message received: " << message << std::endl;

    // Send the message to the connected client (other instance of the app)
    if (clientConn_) {
        clientConn_->send(message);  // Send the message to the other app instance
    }

    // Echo the message back to the sender
    wsConn->send("Echo: " + message);
}

void ChatWebSocket::handleNewConnection(const HttpRequestPtr &,
                                        const WebSocketConnectionPtr &wsConn) {
    std::cout << "New WebSocket connection established" << std::endl;

    // Send a welcome message to the connected client
    wsConn->send("Welcome to the chat server!");
}

void ChatWebSocket::handleConnectionClosed(const WebSocketConnectionPtr &wsConn) {
    std::cout << "WebSocket connection closed" << std::endl;
}

void ChatWebSocket::connectToAnotherServer(const std::string &url) {
    client_ = WebSocketClient::newWebSocketClient(url);

    client_->setMessageHandler([this](const std::string &message, const WebSocketClientPtr &,
                                      const WebSocketMessageType &type) {
        std::cout << "Message received from another server: " << message << std::endl;
        // Process the message received from the other server
    });

    client_->setConnectionClosedHandler([this](const WebSocketClientPtr &) {
        std::cout << "Connection to another server closed" << std::endl;
        clientConn_.reset();  // Reset the connection pointer
    });

    // Connect to the other server
    auto req = HttpRequest::newHttpRequest();
    req->setMethod(drogon::Get);

    std::cout << "Connecting to the other server..." << std::endl;
    try {
        client_->connectToServer(req, [this, url](ReqResult result, const HttpResponsePtr &response,
                                             const WebSocketClientPtr &wsClient) {
            std::cout << "Connection result: " << static_cast<int>(result) << std::endl;
            if (result == ReqResult::Ok && wsClient) {
                clientConn_ = wsClient->getConnection();
                std::cout << "Successfully connected to the other server" << std::endl;
            } else {
                std::cerr << "Failed to connect to the other server. Error: "
                          << static_cast<int>(result) << std::endl;
                if (response) {
                    std::cerr << "HTTP Response code: " << response->statusCode() << std::endl;
                }
                // Optionally, schedule a reconnection attempt after a delay
                std::this_thread::sleep_for(std::chrono::seconds(5));  // Wait before retrying
                connectToAnotherServer(url);                           // Retry connection
            }
        });
    } catch (const std::exception &e) {
        std::cerr << "Exception occurred while connecting to the other server: " << e.what()
                  << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception occurred while connecting to the other server" << std::endl;
    }
}
