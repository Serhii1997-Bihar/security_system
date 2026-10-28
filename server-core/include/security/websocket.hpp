#pragma once

#include "models/security.hpp"
#include <drogon/WebSocketController.h>
#include <mutex>
#include <set>
#include <atomic>
#include <string>

extern SecurityState g_state;
extern std::atomic<LockState> lock_state;
extern std::mutex g_state_mutex;

class SecurityWebSocket : public drogon::WebSocketController<SecurityWebSocket>
{
public:
    void handleNewMessage(const drogon::WebSocketConnectionPtr& wsConnPtr,
                         std::string&& message,
                         const drogon::WebSocketMessageType& type) override;

    void handleNewConnection(const drogon::HttpRequestPtr& req,
                            const drogon::WebSocketConnectionPtr& wsConnPtr) override;

    void handleConnectionClosed(const drogon::WebSocketConnectionPtr& wsConnPtr) override;

    static void broadcast(const std::string& message);

    WS_PATH_LIST_BEGIN
    WS_PATH_ADD("/ws/security");
    WS_PATH_LIST_END

  private:
    static std::set<drogon::WebSocketConnectionPtr> connections_;
    static std::mutex connections_mutex_;
};