#include <api.h>
#include <crow.h>
#include <loggerHandler.h>
#include <crow/middlewares/cors.h>
#include <authorisationMiddleware.h>

/**
 * @brief The entry point for our http server.
*/
int main(int argc, char* argv[])
{
	// If you pass createadmin as an argument
	// create new admin
	if (argc == 2 && strcmp(argv[1], "createadmin") == 0)
	{
		std::string firstName;
		std::string lastName;
		std::string username;
		std::string email;
		std::string password;

		std::cout << "Enter first name for the admin:" << std::endl;
		std::cin >> firstName;

		std::cout << "Enter last name for the admin:" << std::endl;
		std::cin >> lastName;

		std::cout << "Enter username for the admin:" << std::endl;
		std::cin >> username;

		std::cout << "Enter email for the admin:" << std::endl;
		std::cin >> email;

		std::cout << "Enter password for the admin:" << std::endl;
		std::cin >> password;

		DBManager dbManager;
		ValidationManager validationManager;

		auto registerData = crow::query_string(
			"?" +
			std::string("firstName=") + firstName +
			"&lastName=" + lastName +
			"&username=" + username +
			"&email=" + email +
			"&password=" + password
		);

		std::vector<std::string> recordSet = validationManager.isRegisterDataValid(registerData);
		if (!recordSet.empty())
		{
			std::cout << "Validations of data failed at: \n";

			for (auto& el : recordSet)
			{
				std::cout << el << "\n";
			}

			return 0;
		}

		char salt[BCRYPT_HASHSIZE];
		char hash[BCRYPT_HASHSIZE];
		int ret;

		ret = bcrypt_gensalt(12, salt);

		if (ret != 0)
		{
			std::cout << "Failed to generate salt";
			return 0;
		}

		ret = bcrypt_hashpw(registerData.get("password"), salt, hash);

		if (ret != 0)
		{
			std::cout << "Failed to generate hash";
			return 0;
		}

		recordSet = dbManager.registerUser(registerData, hash, salt, 1);

		if (!recordSet.empty())
		{
			std::cout << "There was one or more problem with the registration of the admin: \n";

			for (auto& el : recordSet)
			{
				std::cout << el << "\n";
			}

			return 0;
		}

		std::cout << "Admin was saved successfully into the database!";
		return 0;
	}

	// Initialise crow app
	// and apply the default CORS handler
	crow::App<crow::CORSHandler, AuthorisationMiddleware> app;

	// Change the default handler to a new one
	// with output to a file and a SEQ (or any other graylog server)
	LoggerHandler loggerHandler;
	loggerHandler.initLogs();

	// Use blueprints for modularity
	crow::Blueprint api = initApi(app);
	app.register_blueprint(api);

	// We should run app in singlethreaded and non async mode
	// so that the request are handled in FIFO order
	app.port(18080).run();
}