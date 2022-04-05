#include <api.h>

crow::Blueprint initApi()
{
	crow::Blueprint api("api");
	ValidationManager validationManager;
	DBManager dbManager;
	ResponseJSONManager responseJSONManager;

	CROW_BP_ROUTE(api, "/register")
		.methods(crow::HTTPMethod::Post)
		([&](const crow::request& req, crow::response& res)
			{
				auto url_params = crow::query_string("?" + req.body);

				CROW_LOG_INFO << "User with email: " << url_params.get("email") << " is trying to register.";
				
				std::vector<std::string> recordSet = validationManager.isRegisterDataValid(url_params);
				
				// If validation falls
				if (recordSet.size() != 0)
				{
					std::string log = "Failed validation/s at user with email: " + std::string(url_params.get("email")) + " at: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;
					
					res = responseJSONManager.createJSONResponse(false, recordSet, "register");
					res.end();
					return;
				}

				// TODO: Hash password

				recordSet = dbManager.registerUser(url_params);

				if (recordSet.size() != 0)
				{
					std::string log = "Failed to save user with email: " + std::string(url_params.get("email")) + " to the database. Reason/s: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "register");
					res.end();
					return;
				}

				CROW_LOG_INFO << "User with email: " + std::string(url_params.get("email")) + " is successfully register into the database.";
				
				// Create and send request
				res = responseJSONManager.createJSONResponse(true, recordSet, "register");
				res.end();
				return;
			});

	return api;
}
