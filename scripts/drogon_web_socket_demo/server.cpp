#include <drogon/HttpAppFramework.h>
#include <drogon/PubSubService.h>
#include <drogon/WebSocketController.h>

#include <atomic>
#include <iostream>
#include <thread>

using namespace drogon;

class WebSocketChat : public drogon::WebSocketController<WebSocketChat> {
   public:
    void handleNewMessage(const WebSocketConnectionPtr &, std::string &&,
                          const WebSocketMessageType &) override;
    void handleConnectionClosed(const WebSocketConnectionPtr &) override;
    void handleNewConnection(const HttpRequestPtr &, const WebSocketConnectionPtr &) override;

    WS_PATH_LIST_BEGIN
    WS_PATH_ADD("/chat", Get);
    WS_ADD_PATH_VIA_REGEX("/[^/]*", Get);
    WS_PATH_LIST_END

    void publishMessageToRoom(const std::string &roomName, const std::string &message);

   private:
    PubSubService<std::string> chatRooms_;
};

struct Subscriber {
    std::string chatRoomName_;
    drogon::SubscriberID id_;
};

void WebSocketChat::handleNewMessage(const WebSocketConnectionPtr &wsConnPtr, std::string &&message,
                                     const WebSocketMessageType &type) {
    std::cout << "new websocket message: " << message << std::endl;
    if (type == WebSocketMessageType::Ping) {
        std::cout << "recv a ping" << std::endl;
    } else if (type == WebSocketMessageType::Text) {
        auto &s = wsConnPtr->getContextRef<Subscriber>();
        chatRooms_.publish(s.chatRoomName_, message);
    }
}

void WebSocketChat::handleConnectionClosed(const WebSocketConnectionPtr &conn) {
    std::cout << "websocket closed!" << std::endl;
    auto &s = conn->getContextRef<Subscriber>();
    chatRooms_.unsubscribe(s.chatRoomName_, s.id_);
}

void WebSocketChat::handleNewConnection(const HttpRequestPtr &req,
                                        const WebSocketConnectionPtr &conn) {
    std::cout << "new websocket connection!" << std::endl;

    conn->send("Welcome to the chat room!");

    Subscriber s;
    s.chatRoomName_ = req->getParameter("room_name");
    std::cout << "subscribing to chat room: " << s.chatRoomName_ << std::endl;
    s.id_ = chatRooms_.subscribe(s.chatRoomName_,
                                 [conn](const std::string &topic, const std::string &message) {
                                     // Suppress unused variable warning
                                     (void)topic;
                                     conn->send(message);
                                 });
    conn->setContext(std::make_shared<Subscriber>(std::move(s)));
}

void WebSocketChat::publishMessageToRoom(const std::string &roomName, const std::string &message) {
    chatRooms_.publish(roomName, message);
}

int main() {
    WebSocketChat chatServer;
    std::atomic<bool> running(true);

    std::thread inputThread([&]() {
        while (running) {
            std::string roomName;
            std::string message;
            std::cout << "Enter chat room name: ";
            std::getline(std::cin, roomName);
            std::cout << "Enter message: ";
            std::getline(std::cin, message);

            if (!roomName.empty() && !message.empty()) {
                chatServer.publishMessageToRoom(roomName, message);
            } else {
                std::cout << "Chat room name and message cannot be empty." << std::endl;
            }
        }
    });

    app().addListener("127.0.0.1", 8848).run();

    running = false;
    inputThread.join();

    return 0;
}
