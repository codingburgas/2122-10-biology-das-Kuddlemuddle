/*! @file authorisationMiddleware.h
*   @brief A header file for the authorisation middleware.
*/

#pragma once
#include <crow.h>

/**
 * @brief Struct for the authorisation middleware
*/
struct AuthorisationMiddleware : crow::ILocalMiddleware
{
    /**
     * @brief Contex of the middleware
    */
    struct context
    {
        int isAdmin = NULL; /*!< Is the user admin */
        int userId = NULL; /*!< Id of the user */
    };

    /**
     * @brief Handler for the authorisation middleware before request
     * @param req Request
     * @param res Response
     * @param ctx Context of the middleware
    */
    void before_handle(crow::request& req, crow::response& res, context& ctx);

    /**
     * @brief Handler for the authorisation middleware after request
     * @param req Request
     * @param res Response
     * @param ctx Context of the middleware
    */
    void after_handle(crow::request& req, crow::response& res, context& ctx);
};