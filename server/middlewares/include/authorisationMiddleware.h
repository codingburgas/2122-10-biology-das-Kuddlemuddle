#pragma once
#include <crow.h>

struct AuthorisationMiddleware : crow::ILocalMiddleware
{
    struct context
    {
        int isAdmin = NULL;
        int userId = NULL;
    };

    void before_handle(crow::request& req, crow::response& res, context& ctx);

    void after_handle(crow::request& req, crow::response& res, context& ctx);
};