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
				auto registerData = crow::query_string("?" + req.body);

				CROW_LOG_INFO << "User with email: " << registerData.get("email") << " is trying to register.";
				
				std::vector<std::string> recordSet = validationManager.isRegisterDataValid(registerData);
				
				// If validation falls
				if (recordSet.size() != 0)
				{
					std::string log = "Failed validation/s at user with email: " + std::string(registerData.get("email")) + " at: ";

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

				recordSet = dbManager.registerUser(registerData);

				// If saving to database fails
				if (recordSet.size() != 0)
				{
					std::string log = "Failed to save user with email: " + std::string(registerData.get("email")) + " to the database. Reason/s: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "register");
					res.end();
					return;
				}

				CROW_LOG_INFO << "User with email: " + std::string(registerData.get("email")) + " is successfully register into the database.";
				
				// Create and send request
				res = responseJSONManager.createJSONResponse(true, recordSet, "register");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/login")
		.methods(crow::HTTPMethod::Post)
		([&](const crow::request& req, crow::response& res)
			{
				auto loginData = crow::query_string("?" + req.body);

				CROW_LOG_INFO << "User with email: " << loginData.get("loginCredential") << " is trying to register.";

				std::vector<std::string> recordSet = validationManager.isLoginDataValid(loginData);

				// If validation falls
				if (recordSet.size() != 0)
				{
					std::string log = "Failed validation/s at user with email: " + std::string(loginData.get("loginCredential")) + " at: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "login");
					res.end();
					return;
				}

				// TODO: Hash password

				recordSet = dbManager.loginUser(loginData);

				// If login was unsuccessful
				if (recordSet[0].substr(0, 7) != "Bearer ")
				{
					std::string log = "Failed to loggin user with credential: " + std::string(loginData.get("loginCredential")) + ". Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "login");
					res.end();
					return;
				}

				CROW_LOG_INFO << "User with credential: " + std::string(loginData.get("loginCredential")) + " is successfully logged in.";

				// Remove the "Bearer "
				recordSet[0] = recordSet[0].substr(7);

				res = responseJSONManager.createJSONResponse(true, recordSet, "login");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/users/<string>")
		.methods(crow::HTTPMethod::Get)
		([&](const crow::request& req, crow::response& res, std::string username)
			{
				EnvManager envManager;
				std::cout << username;
				int id;

				if (username == "@me")
				{
					auto verifier = jwt::verify()
						.allow_algorithm(jwt::algorithm::hs256(envManager.getEnv("JWT_SECRET")));

					std::string myauth = req.get_header_value("Authorization");
					
					// If values is missing
					if (myauth == "")
					{
						res = crow::response(401);
						res.end();
						return;
					}

					// Decode the token
					std::string mycreds = myauth.substr(7);
					auto decoded = jwt::decode(mycreds);

					// Verify token
					try
					{
						verifier.verify(decoded);
					}
					catch (...)
					{
						res = crow::response(403);
						res.end();
						return;
					}

					// Get id from JWT
					for (auto& e : decoded.get_payload_claims())
					{
						if (e.first == "sub")
						{
							id = std::stoi(e.second.as_string());
						}
					}

					std::vector<std::string> recordSet = dbManager.getUserInfo(username, id);

					// Error happend
					if (recordSet.size() == 1)
					{
						std::string log = "Failed to get user info with id: " + std::to_string(id) + ". Reason: ";

						for (auto el : recordSet)
						{
							log += el + " ";
						}

						CROW_LOG_WARNING << log;
					}

					res = responseJSONManager.createProfileJSONResponse(recordSet);
					res.end();
					return;
				}

				res.end();
				return;
			});

	return api;
}
