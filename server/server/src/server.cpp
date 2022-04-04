#include <crow.h>
#include <loggerHandler.h>
/**
 * @brief The entry point for our http server.
*/
int main()
{
    crow::SimpleApp app;

    // Change the default handler to a new one
    // with output to a file and a SEQ
    LoggerHandler loggerHandler;

    loggerHandler.initLogs();

    CROW_ROUTE(app, "/")([]() {
        return "Hello world";
        });

    // We should run app in singlethreaded mode
    // so that the request are handled in FIFO order
    app.port(18080).run();
}