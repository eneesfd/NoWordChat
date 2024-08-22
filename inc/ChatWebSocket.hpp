#ifndef CHATWEBSOCKET_HPP
#define CHATWEBSOCKET_HPP

#include <drogon/WebSocketClient.h>
#include <drogon/WebSocketController.h>

using namespace drogon;

class ChatWebSocket : public drogon::WebSocketController<ChatWebSocket> {
   public:
    ChatWebSocket();

    void handleNewMessage(const WebSocketConnectionPtr &, std::string &&,
                          const WebSocketMessageType &) override;

    void handleNewConnection(const HttpRequestPtr &, const WebSocketConnectionPtr &) override;

    void handleConnectionClosed(const WebSocketConnectionPtr &) override;

    void connectToAnotherServer(const std::string &url);

    WS_PATH_LIST_BEGIN
    WS_PATH_ADD("/chat");
    WS_PATH_LIST_END

   private:
    WebSocketClientPtr client_;
    WebSocketConnectionPtr clientConn_;
};

#endif  // CHATWEBSOCKET_HPP
