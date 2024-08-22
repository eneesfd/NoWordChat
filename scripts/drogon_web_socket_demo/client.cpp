#include <drogon/HttpAppFramework.h>
#include <drogon/WebSocketClient.h>

#include <iostream>

using namespace drogon;
using namespace std::chrono_literals;

int main(int argc, char *argv[]) {
    std::string server;
    std::string path;
    std::optional<uint16_t> port;
    // Connect to a public echo server
    if (argc > 1 && std::string(argv[1]) == "-p") {
        server = "wss://echo.websocket.events/.ws";
        path = "/";
    } else {
        server = "ws://127.0.0.1";
        port = 8848;
        path = "/chat";
    }

    std::string serverString;
    if (port.value_or(0) != 0)
        serverString = server + ":" + std::to_string(port.value());
    else
        serverString = server;
    auto wsPtr = WebSocketClient::newWebSocketClient(serverString);
    auto req = HttpRequest::newHttpRequest();
    req->setPath(path);

    wsPtr->setMessageHandler([](const std::string &message, const WebSocketClientPtr &,
                                const WebSocketMessageType &type) {
        std::string messageType = "Unknown";
        if (type == WebSocketMessageType::Text)
            messageType = "text";
        else if (type == WebSocketMessageType::Pong)
            messageType = "pong";
        else if (type == WebSocketMessageType::Ping)
            messageType = "ping";
        else if (type == WebSocketMessageType::Binary)
            messageType = "binary";
        else if (type == WebSocketMessageType::Close)
            messageType = "Close";

        std::cout << "new message (" << messageType << "): " << message << std::endl;
    });

    wsPtr->setConnectionClosedHandler([](const WebSocketClientPtr &) {
        std::cout << "WebSocket connection closed!" << std::endl;
    });

    std::cout << "Connecting to WebSocket at " << server << std::endl;
    wsPtr->connectToServer(
        req, [](ReqResult r, const HttpResponsePtr &, const WebSocketClientPtr &wsPtr) {
            if (r != ReqResult::Ok) {
                std::cerr << "Failed to establish WebSocket connection!" << std::endl;
                wsPtr->stop();
                return;
            }
            std::cout << "WebSocket connected!" << std::endl;
            wsPtr->getConnection()->setPingMessage("", 2s);
            wsPtr->getConnection()->send("hello!");
        });

    // // Quit the application after 15 seconds
    // app().getLoop()->runAfter(15, []() { app().quit(); });

    // app().setLogLevel(trantor::Logger::kDebug);
    app().run();
    std::cout << "bye!" << std::endl;
    return 0;
}