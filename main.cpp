#include <drogon/HttpAppFramework.h>
#include <iostream>

int main() {
    // Set HTTP listener address and port
    drogon::app().addListener("127.0.0.1", 80);

    // Load config file
    drogon::app().loadConfigFile("../ui/config.json");

    std::cout << "Web server is running" << std::endl;
    std::cout << "Connect to http://127.0.0.1:80" << std::endl;

    // Run HTTP framework,the method will block in the internal event loop
    drogon::app().run();
    return 0;
}