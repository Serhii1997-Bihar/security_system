#include "models/security.hpp"
#include "security/auth_filter.hpp"
#include <drogon/drogon.h>
#include <mutex>

using drogon::AdviceCallback;
using drogon::HttpRequestPtr;
using drogon::HttpResponse;

static SecurityState g_state{SecurityState::Disarmed};
static LockState lock_state{LockState::Closed};
static std::mutex g_state_mutex{};

int main() {
    drogon::app().registerHandler(
        "/api/v1/security/status",
        [](const HttpRequestPtr& request, AdviceCallback&& callback) {
            std::lock_guard lock(g_state_mutex);

            Json::Value response;

            switch (g_state) {
                case SecurityState::Disarmed: response["status"] = "off"; break;
                case SecurityState::Armed:    response["status"] = "on"; break;
            }

            callback(HttpResponse::newHttpJsonResponse(response));
        },

        {drogon::Get}
    );

    drogon::app().registerHandler(
        "/api/v1/security/put_on",
        [](const HttpRequestPtr& request, AdviceCallback&& callback) {
            {
                std::lock_guard lock(g_state_mutex);
                g_state = SecurityState::Armed;
            }

            Json::Value response;
            response["message"] = "Security system armed";

            callback(HttpResponse::newHttpJsonResponse(response));
        },

        {drogon::Post}
    );

    drogon::app().registerHandler(
        "/api/v1/security/put_off",
        [](const HttpRequestPtr& request, AdviceCallback&& callback) {
            {
                std::lock_guard lock(g_state_mutex);
                g_state = SecurityState::Disarmed;
            }

            Json::Value response;
            response["message"] = "Security system disarmed";

            callback(HttpResponse::newHttpJsonResponse(response));
        },

        {drogon::Post}
    );

    drogon::app().registerHandler(
        "/api/v1/security/reload",
        [](const HttpRequestPtr& request, AdviceCallback&& callback) {
            Json::Value response;
            response["message"] = "Security system is reloading configuration";

            callback(HttpResponse::newHttpJsonResponse(response));
        },

        {drogon::Post}
    );

    drogon::app().registerHandler(
        "/api/v1/lock_status",
        [](const HttpRequestPtr& request, AdviceCallback&& callback) {
            Json::Value response;

            switch (lock_state) {
                case LockState::Opened: response["status"] = "opened"; break;
                case LockState::Closed:    response["status"] = "closed"; break;
            }

            callback(HttpResponse::newHttpJsonResponse(response));
        },

        {drogon::Get}
    );

    drogon::app()
        .addListener("0.0.0.0", 8080)
        .setThreadNum(4)
        .run();

    return 0;
}