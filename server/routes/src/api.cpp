#include <api.h>

crow::Blueprint initApi(crow::App<crow::CORSHandler, AuthorisationMiddleware> &app)
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
				int id = 0;
				
				CROW_LOG_INFO << "Trying to get information for user: " << username;

				if (username == "@me")
				{
					auto verifier = jwt::verify<jwt::traits::nlohmann_json>()
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

					// Verify token
					try
					{
						auto decoded = jwt::decode<jwt::traits::nlohmann_json>(mycreds);
						verifier.verify(decoded);
						id = std::stoi(decoded.get_payload_claim("sub").as_string());
					}
					catch (...)
					{
						res = crow::response(403);
						res.end();
						return;
					}
				}

				std::vector<std::string> recordSet = dbManager.getUserInfo(username, id);
				
				// Error happend
				if (recordSet.size() == 1)
				{
					std::string log = "Failed to get user info with username: " + username + ". Reason: ";
				
					for (auto el : recordSet)
					{
						log += el + " ";
					}
				
					CROW_LOG_WARNING << log;
				}
				
				res = responseJSONManager.createProfileJSONResponse(recordSet);
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/users/<string>")
		.methods(crow::HTTPMethod::Delete)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		//CROW_MIDDLEWARES(app, AuthorisationMiddleware)
		([&](const crow::request& req, crow::response& res, std::string username)
			{
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				CROW_LOG_INFO << "Trying to delete user with username: " << username;

				std::vector<std::string> recordSet;

				if (username == "@me" || ctx.isAdmin)
				{
					recordSet = dbManager.deleteUser(username, ctx.userId);
					
					if (recordSet.size() != 0)
					{
						res = responseJSONManager.createJSONResponse(false , recordSet, "used-deletion");
						res.end();
					}

					res = responseJSONManager.createJSONResponse(true, recordSet, "used-deletion");
					res.end();
					return;
				}

				CROW_LOG_WARNING << "Failed to delete user. Reason: Requesting user isn't authorised!";
				res = crow::response(403);
				res.end();
				return;
			});

	return api;
}
