#include "security/websocket.hpp"

std::set<drogon::WebSocketConnectionPtr> SecurityWebSocket::connections_{};
std::mutex SecurityWebSocket::connections_mutex_{};

void SecurityWebSocket::handleNewMessage(const drogon::WebSocketConnectionPtr& wsConnPtr,
                                         std::string&& message,
                                         const drogon::WebSocketMessageType& type)
{
}

void SecurityWebSocket::handleNewConnection(const drogon::HttpRequestPtr& req,
                                            const drogon::WebSocketConnectionPtr& wsConnPtr)
{
    std::lock_guard lock(connections_mutex_);
    connections_.insert(wsConnPtr);

    Json::Value statusUpdate;
    {
        std::lock_guard state_lock(g_state_mutex);
        statusUpdate["security_status"] = (g_state == SecurityState::Armed) ? "on" : "off";
    }
    statusUpdate["lock_status"] = (lock_state.load() == LockState::Opened) ? "opened" : "closed";

    wsConnPtr->send(statusUpdate.toStyledString());
}

void SecurityWebSocket::handleConnectionClosed(const drogon::WebSocketConnectionPtr& wsConnPtr)
{
    std::lock_guard lock(connections_mutex_);
    connections_.erase(wsConnPtr);
}

void SecurityWebSocket::broadcast(const std::string& message)
{
    std::lock_guard lock(connections_mutex_);
    for (const auto& conn : connections_)
    {
        if (conn->connected())
        {
            conn->send(message);
        }
    }
}