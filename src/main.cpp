#include <drogon/drogon.h>

#include <iostream>

#include "ChatWebSocket.hpp"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <port>\n";
        return 1;
    }

    int port = std::stoi(argv[1]);
    std::string otherServerUrl = "ws://localhost:" + std::to_string(port) + "/chat";

    // Start the Drogon application
    auto &app = drogon::app();

    // auto chatWs = std::make_shared<ChatWebSocket>();
    // chatWs->connectToAnotherServer(otherServerUrl);

    // set config.json
    app.loadConfigFile("../ui/config.json");

    // Listen on a different port (port + 1) for this instance
    app.addListener("0.0.0.0", port + 1);
    app.run();

    return 0;
}
