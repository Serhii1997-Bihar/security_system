#include "security/auth_filter.hpp"
#include "helpers/load_env.hpp"

void AuthFilter::doFilter(const drogon::HttpRequestPtr& req,
                          drogon::FilterCallback&& fcb,
                          drogon::FilterChainCallback&& fccb) {
    load_env();
    std::string expected_key = get_env_var("AUTH_PASSWORD", "");

    auto authHeader = req->getHeader("Authorization");

    if (authHeader.empty() || authHeader.find("Bearer ") != 0) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k401Unauthorized);
        fcb(resp);
        return;
    }

    std::string token = authHeader.substr(7);

    if (token != expected_key) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k401Unauthorized);
        fcb(resp);
        return;
    }

    fccb();
}

void DROGON_REGISTER_FILTER(AuthFilter);