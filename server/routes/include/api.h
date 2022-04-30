#pragma once
#include <db.h>
#include <crow.h>
#include <response.h>
#include <validations.h>
#include <crow/middlewares/cors.h>
#include <authorisationMiddleware.h>
#include <bcrypt-lib.h>

crow::Blueprint initApi(crow::App<crow::CORSHandler, AuthorisationMiddleware> &app);