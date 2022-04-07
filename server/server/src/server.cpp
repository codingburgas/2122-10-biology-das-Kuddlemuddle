#include <api.h>
#include <crow.h>
#include <loggerHandler.h>
#include <crow/middlewares/cors.h>
#include <authorisationMiddleware.h>

/**
 * @brief The entry point for our http server.
*/
int main()
{
    // Initialise crow app
    // and apply the default CORS handler
    crow::App<crow::CORSHandler, AuthorisationMiddleware> app;

    // Change the default handler to a new one
    // with output to a file and a SEQ
    LoggerHandler loggerHandler;
    loggerHandler.initLogs();

    // Use blueprints for modularity
    crow::Blueprint api = initApi(app);
    app.register_blueprint(api);

    // We should run app in singlethreaded mode
    // so that the request are handled in FIFO order
    app.port(18080).run();
}