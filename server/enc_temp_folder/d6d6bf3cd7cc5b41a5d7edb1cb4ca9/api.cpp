/*! @file api.cpp
*   @brief A source file for the api blueprint.
*/

#include <api.h>
#include <filesystem>
#include <types.h>

crow::Blueprint initApi(crow::App<crow::CORSHandler, AuthorisationMiddleware>& app)
{
	crow::Blueprint api("api");
	ValidationManager validationManager;
	DBManager dbManager;
	ResponseJSONManager responseJSONManager;

	CROW_BP_ROUTE(api, "/register")
		.methods(crow::HTTPMethod::POST)
		([&](const crow::request& req, crow::response& res)
			{
				// Get data from request body in form of query string
				auto registerData = crow::query_string("?" + req.body);

				std::vector<std::string> recordSet = validationManager.checkForNullValues(
					registerData,
					{
						"firstName",
						"lastName",
						"username",
						"email",
						"password"
					}
				);

				// If there are null values
				if (recordSet.size() != 0)
				{
					res = responseJSONManager.createJSONResponse(
						false, recordSet, "register"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				CROW_LOG_INFO << "User with email: " << registerData.get("email") <<
					" is trying to register.";

				recordSet = validationManager.isRegisterDataValid(registerData);

				// If validation falls
				if (recordSet.size() != 0)
				{
					std::string log = "Failed validation/s at user with email: " +
						std::string(registerData.get("email")) + " at: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "register"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				// Hashing the password
				char salt[BCRYPT_HASHSIZE];
				char hash[BCRYPT_HASHSIZE];
				int ret;

				ret = bcrypt_gensalt(12, salt);

				if (ret != 0)
				{
					std::string log = "Failed to generate salt";

					res = responseJSONManager.createJSONResponse(
						false, { log }, "register"
					);

					res.code = crow::status::INTERNAL_SERVER_ERROR;
					res.end();

					return;
				}

				ret = bcrypt_hashpw(registerData.get("password"), salt, hash);

				if (ret != 0)
				{
					std::string log = "Failed to generate hash";

					res = responseJSONManager.createJSONResponse(
						false, { log }, "register"
					);

					res.code = crow::status::INTERNAL_SERVER_ERROR;
					res.end();

					return;
				}

				recordSet = dbManager.registerUser(registerData, hash, salt);

				// If saving to database fails
				if (recordSet.size() != 0)
				{
					std::string log = "Failed to save user with email: " +
						std::string(registerData.get("email")) + " to the database. Reason/s: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "register"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				CROW_LOG_INFO << "User with email: " + std::string(registerData.get("email")) +
					" is successfully register into the database.";

				// Create and send request
				res = responseJSONManager.createJSONResponse(true, recordSet, "register");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/login")
		.methods(crow::HTTPMethod::POST)
		([&](const crow::request& req, crow::response& res)
			{
				auto loginData = crow::query_string("?" + req.body);

				std::vector<std::string> recordSet = validationManager.checkForNullValues(
					loginData,
					{
						"loginCredential",
						"password"
					}
				);

				// If there are null values
				if (recordSet.size() != 0)
				{
					res = responseJSONManager.createJSONResponse(false, recordSet, "login");
					res.code = crow::status::BAD_REQUEST;
					res.end();
					return;
				}

				CROW_LOG_INFO << "User with email: " <<
					loginData.get("loginCredential") << " is trying to register.";

				recordSet = validationManager.isLoginDataValid(loginData);

				// If validation falls
				if (recordSet.size() != 0)
				{
					std::string log = "Failed validation/s at user with email: " +
						std::string(loginData.get("loginCredential")) + " at: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "login"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				recordSet = dbManager.loginUser(loginData);

				// If login was unsuccessful
				if (recordSet[0].substr(0, 7) != "Bearer ")
				{
					std::string log = "Failed to loggin user with credential: " +
						std::string(loginData.get("loginCredential")) + ". Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "login"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				CROW_LOG_INFO << "User with credential: " +
					std::string(loginData.get("loginCredential")) + " is successfully logged in.";

				// Remove the "Bearer "
				recordSet[0] = recordSet[0].substr(7);

				res = responseJSONManager.createJSONResponse(true, recordSet, "login");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/users/<int>")
		.methods(crow::HTTPMethod::GET)
		([&](const crow::request& req, crow::response& res, int id)
			{
				EnvManager envManager;

				CROW_LOG_INFO << "Trying to get information for user with id: " << id;

				std::vector<std::string> recordSet = dbManager.getUserInfo("@me", id);

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
			});

	CROW_BP_ROUTE(api, "/users/<string>")
		.methods(crow::HTTPMethod::GET)
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
						res = crow::response(crow::status::UNAUTHORIZED);
						res.end();
						return;
					}

					// Remove "Bearer "
					std::string mycreds = myauth.substr(7);

					// Try to decode and verify token
					try
					{
						auto decoded = jwt::decode<jwt::traits::nlohmann_json>(mycreds);
						verifier.verify(decoded);
						id = std::stoi(decoded.get_payload_claim("sub").as_string());
					}
					catch (...)
					{
						res = crow::response(crow::status::FORBIDDEN);
						res.end();
						return;
					}
				}

				if (username == "all")
				{
					auto verifier = jwt::verify<jwt::traits::nlohmann_json>()
						.allow_algorithm(jwt::algorithm::hs256(envManager.getEnv("JWT_SECRET")));

					std::string myauth = req.get_header_value("Authorization");

					// If values is missing
					if (myauth == "")
					{
						res = crow::response(crow::status::UNAUTHORIZED);
						res.end();
						return;
					}

					// Remove "Bearer "
					std::string mycreds = myauth.substr(7);

					// Try to decode and verify token
					try
					{
						auto decoded = jwt::decode<jwt::traits::nlohmann_json>(mycreds);
						verifier.verify(decoded);
						id = std::stoi(decoded.get_payload_claim("sub").as_string());
						int isAdmin = decoded.get_payload_claim("isAdmin").as_int();

						if (isAdmin == 0)
						{
							res = crow::response(crow::status::FORBIDDEN);
							res.end();
							return;
						}
					}
					catch (...)
					{
						res = crow::response(crow::status::FORBIDDEN);
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
						std::string log = "Failed to delete user. Reason: ";

						for (auto el : recordSet)
						{
							log += el + " ";
						}

						CROW_LOG_WARNING << log;

						res = responseJSONManager.createJSONResponse(
							false, recordSet, "user-deletion"
						);

						res.code = crow::status::BAD_REQUEST;
						res.end();

						return;
					}

					res = responseJSONManager.createJSONResponse(true, recordSet, "user-deletion");
					res.end();
					return;
				}

				CROW_LOG_WARNING << "Failed to delete user. Reason: Requesting user isn't authorised!";
				res = crow::response(crow::status::FORBIDDEN);
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/users/@me")
		.methods(crow::HTTPMethod::PATCH)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		//CROW_MIDDLEWARES(app, AuthorisationMiddleware)
		([&](const crow::request& req, crow::response& res)
			{
				auto updateData = crow::query_string("?" + req.body);
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				CROW_LOG_INFO << "Trying to update user info with id: " << ctx.userId;

				std::vector<std::string> recordSet = validationManager.checkForNullValues(
					updateData,
					{
						"firstName",
						"lastName",
						"username",
						"email",
						"password"
					}
				);

				// If there are null values
				if (recordSet.size() != 0)
				{
					std::string log = "Failed null validation/s at user with id: " +
						std::to_string(ctx.userId) + " at: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "user-update"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				recordSet = validationManager.isRegisterDataValid(updateData, true);

				// If validation falls
				if (recordSet.size() != 0)
				{
					std::string log = "Failed validation/s at user with id: " +
						std::to_string(ctx.userId) + " at: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "user-update"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				recordSet = dbManager.updateUser(ctx.userId, updateData);

				if (recordSet.size() != 0)
				{
					std::string log = "Failed to update user. Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "user-update"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				res = responseJSONManager.createJSONResponse(true, recordSet, "user-update");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/users/@me/updateAvatarURL")
		.methods(crow::HTTPMethod::PATCH)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		//CROW_MIDDLEWARES(app, AuthorisationMiddleware)
		([&](const crow::request& req, crow::response& res)
			{
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				CROW_LOG_INFO << "Trying to update user avatar with id: " << ctx.userId;

				try
				{
					crow::multipart::message msg(req);
				}
				catch (...)
				{
					CROW_LOG_WARNING << "Failed to update user avatar for user with id: " <<
						ctx.userId << ". Reason: Avatar data and file extensions are missing!";

					res = responseJSONManager.createJSONResponse(
						false, { "avatar", "fileExtensions" }, "user-update"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				crow::multipart::message msg(req);

				crow::multipart::part filePart;
				crow::multipart::part fileExtensionPart;

				try
				{
					filePart = msg.get_part_by_name("avatar");
				}
				catch (...)
				{
					CROW_LOG_WARNING << "Failed to update user avatar for user with id: " <<
						ctx.userId << ". Reason: Avatar data is missing!";

					res = responseJSONManager.createJSONResponse(false, { "avatar" }, "user-update");
					res.code = crow::status::BAD_REQUEST;
					res.end();
					return;
				}

				try
				{
					fileExtensionPart = msg.get_part_by_name("fileExtensions");
				}
				catch (...)
				{
					CROW_LOG_WARNING << "Failed to update user avatar for user with id: " <<
						ctx.userId << ". Reason: File extension is missing!";

					res = responseJSONManager.createJSONResponse(
						false, { "fileExtensions" }, "user-update"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				std::filesystem::path dir("static/avatars");
				if (!(std::filesystem::exists(dir)))
				{
					if (std::filesystem::create_directories(dir))
					{
						CROW_LOG_WARNING << "Avatars folder not found! New folder created successfully!";
					}
				}

				auto file = filePart.body;
				auto fileExtension = fileExtensionPart.body;

				auto file_handler = std::ofstream("static/avatars/" +
					std::to_string(ctx.userId) + fileExtension, std::ofstream::binary);

				if (!file_handler.is_open())
				{
					CROW_LOG_ERROR << "Cannot open file for avatar";

					res = crow::response(crow::status::INTERNAL_SERVER_ERROR);
					res.end();
					return;
				}

				file_handler << file;
				file_handler.close();

				std::vector<std::string> recordSet = dbManager.updateUserAvatar(
					ctx.userId, std::to_string(ctx.userId) + fileExtension
				);

				if (recordSet.size() != 0)
				{
					std::string log = "Failed to update user avatar. Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "user-avatar-update"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				res = responseJSONManager.createJSONResponse(true, recordSet, "user-avatar-update");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/createNewOrg")
		.methods(crow::HTTPMethod::POST)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		//CROW_MIDDLEWARES(app, AuthorisationMiddleware)
		([&](const crow::request& req, crow::response& res)
			{
				auto reqData = crow::query_string("?" + req.body);
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				std::vector<std::string> recordSet = validationManager.checkForNullValues(
					reqData,
					{
						"orgName",
						"password"
					}
				);

				// If there are null values
				if (recordSet.size() != 0)
				{
					res = responseJSONManager.createJSONResponse(
						false, recordSet, "organisation-register"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				CROW_LOG_INFO << "User with id: " << ctx.userId
					<< " is trying to create new organisation with name: " << reqData.get("orgName");

				recordSet = validationManager.isOrgDataValid(reqData);

				// If validation falls
				if (recordSet.size() != 0)
				{
					std::string log = "Failed validation/s at creating organisation with name: " +
						std::string(reqData.get("orgName")) + " at: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "organisation-register"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				char salt[BCRYPT_HASHSIZE];
				char hash[BCRYPT_HASHSIZE];
				int ret;

				ret = bcrypt_gensalt(12, salt);

				if (ret != 0)
				{
					std::string log = "Failed to generate salt";

					res = responseJSONManager.createJSONResponse(
						false, { log }, "organisation-register"
					);

					res.code = crow::status::INTERNAL_SERVER_ERROR;
					res.end();

					return;
				}

				ret = bcrypt_hashpw(reqData.get("password"), salt, hash);

				if (ret != 0)
				{
					std::string log = "Failed to generate hash";

					res = responseJSONManager.createJSONResponse(
						false, { log }, "organisation-register"
					);

					res.code = crow::status::INTERNAL_SERVER_ERROR;
					res.end();

					return;
				}

				recordSet = dbManager.createOrg(ctx.userId, reqData, hash, salt);

				// If saving to database fails
				if (recordSet.size() != 0)
				{
					std::string log = "Failed to save organisation with name: " +
						std::string(reqData.get("orgName")) + " to the database. Reason/s: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "organisation-register"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				recordSet = dbManager.getOrgIdByName(reqData.get("orgName"));

				// If return value is bigger that 10
				// then it is not a number
				if (recordSet[0].size() > 10)
				{
					std::string log = "Failed to get organisation with name: " +
						std::string(reqData.get("orgName")) + " from the database. Reason/s: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "organisation-register"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				recordSet = dbManager.addUserToOrganisation(
					ctx.userId, std::stoi(recordSet[0]), UserRolesInOrgs::ADMIN
				);

				if (recordSet.size() != 0)
				{
					std::string log = "Failed to add user with id: " +
						std::to_string(ctx.userId) + " to organisation with name : " +
						std::string(reqData.get("orgName"));

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "organisation-register"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				CROW_LOG_INFO << "Organisation with name: " + std::string(reqData.get("orgName")) +
					" is successfully saved into the database.";

				// Create and send request
				res = responseJSONManager.createJSONResponse(true, recordSet, "organisation-register");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/joinOrg")
		.methods(crow::HTTPMethod::POST)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		//CROW_MIDDLEWARES(app, AuthorisationMiddleware)
		([&](const crow::request& req, crow::response& res)
			{
				auto reqData = crow::query_string("?" + req.body);
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				std::vector<std::string> recordSet = validationManager.checkForNullValues(
					reqData,
					{
						"orgId",
						"password"
					},
					{
						"orgId"
					}
					);

				// If there are null values
				if (recordSet.size() != 0)
				{
					res = responseJSONManager.createJSONResponse(
						false, recordSet, "user-update"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				CROW_LOG_INFO << "User with id: " << ctx.userId << " is trying to join organisation with id: "
					<< reqData.get("orgId");

				recordSet = validationManager.isJoinOrgDataValid(reqData);

				// If validation falls
				if (recordSet.size() != 0)
				{
					std::string log = "Failed validation/s at joining in organisation with id: "
						+ std::string(reqData.get("orgId")) + " at: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "join-org"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				recordSet = dbManager.doesPasswordMatchOrg(
					reqData.get("password"),
					std::stoi(reqData.get("orgId"))
				);

				// If password is incorrect
				if (recordSet[0] == "0")
				{
					recordSet[0] = "Password is incorrect";
				}

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to join user with id: " + std::to_string(ctx.userId) +
						" to organisation with id: " + std::string(reqData.get("orgId")) + ". Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "join-org"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				recordSet = dbManager.addUserToOrganisation(
					ctx.userId,
					std::stoi(reqData.get("orgId")),
					UserRolesInOrgs::USER
				);

				if (recordSet.size() != 0)
				{
					std::string log = "Failed to add user with id: " + std::to_string(ctx.userId) +
						" to organisation with id: " +
						std::string(reqData.get("orgId")) + ". Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "join-org"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				CROW_LOG_INFO << "User with id: " << ctx.userId
					<< " is successfully added to organisation with id: " << reqData.get("orgId");

				res = responseJSONManager.createJSONResponse(true, recordSet, "join-org");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/updateRolesInOrg")
		.methods(crow::HTTPMethod::POST)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		//CROW_MIDDLEWARES(app, AuthorisationMiddleware)
		([&](const crow::request& req, crow::response& res)
			{
				auto reqData = crow::query_string("?" + req.body);
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				std::vector<std::string> recordSet = validationManager.checkForNullValues(
					reqData,
					{
						"orgId",
						"userId",
						"roleId"
					},
					{
						"orgId",
						"userId",
						"roleId"
					}
					);

				// If there are null values
				if (recordSet.size() != 0)
				{
					res = responseJSONManager.createJSONResponse(false, recordSet, "user-update");
					res.code = crow::status::BAD_REQUEST;
					res.end();
					return;
				}

				CROW_LOG_INFO << "User with id: " << ctx.userId << " is trying to update role on user with id: " <<
					reqData.get("userId") << " on organisation with id: " << reqData.get("orgId");

				// Is user authorised
				// first element - error or true/false
				// second element - Role of user
				recordSet = dbManager.isUserInOrgAndGetRole(ctx.userId, std::stoi(reqData.get("orgId")));

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to update role on user with id: " +
						std::string(reqData.get("userId")) + " on organisation with id: " +
						std::string(reqData.get("orgId")) + ". Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "update-user-role-org"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				// If password is incorrect
				if (recordSet[1] != "2")
				{
					recordSet[0] = "User is unauthorised";

					std::string log = "Failed to update role on user with id: " +
						std::string(reqData.get("userId")) + " on organisation with id: " +
						std::string(reqData.get("orgId")) + ". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "update-user-role-org"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();

					return;
				}

				int roleId = std::stoi(reqData.get("roleId"));

				switch (roleId)
				{
				case 1:
					recordSet = dbManager.addUserToOrganisation(
						std::stoi(reqData.get("userId")),
						std::stoi(reqData.get("orgId")),
						UserRolesInOrgs::TEACHER,
						false
					);

					break;
				case 2:
					recordSet = dbManager.addUserToOrganisation(
						std::stoi(reqData.get("userId")),
						std::stoi(reqData.get("orgId")),
						UserRolesInOrgs::ADMIN,
						false
					);

					break;
				case 0:
					recordSet = dbManager.addUserToOrganisation(
						std::stoi(reqData.get("userId")),
						std::stoi(reqData.get("orgId")),
						UserRolesInOrgs::USER,
						false
					);

					break;
				default:
					std::string log = "Failed to update role on user with id: " +
						std::string(reqData.get("userId")) + " on organisation with id: " +
						std::string(reqData.get("orgId")) + ". Reason: Role must be 0, 1 or 2";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "Role must be 0, 1 or 2" }, "update-user-role-org"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
					break;
				}

				if (recordSet.size() != 0)
				{
					std::string log = "Failed to update role on user with id: " +
						std::string(reqData.get("userId")) + " on organisation with id: " +
						std::string(reqData.get("orgId")) + ". Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "update-user-role-org"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				CROW_LOG_INFO << "The role on user with id: " << reqData.get("userId") <<
					" is successfully updated on organisation with id: " << reqData.get("orgId");

				res = responseJSONManager.createJSONResponse(true, recordSet, "update-user-role-org");;
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/orgs/<string>")
		.methods(crow::HTTPMethod::Get)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		([&](const crow::request& req, crow::response& res, std::string orgName)
			{
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				if (orgName == "all")
				{
					CROW_LOG_INFO << "User with id: " << ctx.userId <<
						" is trying to get information for all organisations.";

					auto orgInfo = dbManager.getAllOrgsInfo(ctx.userId);

					if (!orgInfo[0].errors.empty())
					{
						std::string log = "Failed to get all organisations. Reasons: ";

						for (auto el : orgInfo[0].errors)
						{
							log += el + " ";
						}

						CROW_LOG_WARNING << log;

						res = responseJSONManager.createJSONResponse(
							false, orgInfo[0].errors, "get-organisation"
						);

						res.code = crow::status::BAD_REQUEST;
						res.end();

						return;
					}

					res = responseJSONManager.createOrgsJSONResponse(orgInfo);
					res.end();
					return;
				}

				CROW_LOG_INFO << "User with id: " << ctx.userId <<
					" is trying to get information for organisation: " << orgName;

				// Try to get organisation's id
				std::vector<std::string> recordSet = dbManager.getOrgIdByName(orgName);

				// If return value is bigger that 10
				// then it is not a number
				if (recordSet[0].size() > 10)
				{
					std::string log = "Failed to get organisation's id with name: " +
						orgName + " from the database. Reason/s: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "get-organisation"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				recordSet = dbManager.isUserInOrgAndGetRole(ctx.userId, std::stoi(recordSet[0]));

				// Error happend
				if (recordSet[0] != "1")
				{
					std::string log = "Failed to get organisation info with name: " +
						orgName + ". Reason: User is unauthorised";
					recordSet[0] = "User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "get-organisation"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();

					return;
				}

				OrgInfo orgInfo = dbManager.getOrgInfo(orgName);

				if (!orgInfo.errors.empty())
				{
					std::string log = "Failed to get organisation info with name: " +
						orgName + ". Reasons: ";

					for (auto el : orgInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "get-organisation"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				res = responseJSONManager.createOrgJSONResponse(orgInfo);
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/orgs/<string>")
		.methods(crow::HTTPMethod::Delete)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		([&](const crow::request& req, crow::response& res, std::string orgName)
			{
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				CROW_LOG_INFO << "User with id: " << ctx.userId <<
					" is trying to delete organisation with name: " << orgName;

				// Try to get organisation's id
				std::vector<std::string> recordSet = dbManager.getOrgIdByName(orgName);

				// If return value's size is bigger that 10
				// then it is not a number
				if (recordSet[0].size() > 10)
				{
					std::string log = "Failed to get organisation's id with name: " +
						orgName + " from the database. Reason/s: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "organisation-deletion"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				std::vector<std::string> recordSet2 = dbManager.isUserInOrgAndGetRole(ctx.userId, std::stoi(recordSet[0]));

				// Error happend
				if (!ctx.isAdmin && (recordSet2[0] != "1" || recordSet2[1] != "2"))
				{
					std::string log = "Failed to delete organisation with name: " +
						orgName + ". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "organisation-deletion"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();

					return;
				}

				recordSet = dbManager.deleteOrg(std::stoi(dbManager.getOrgIdByName(orgName)[0]));

				if (recordSet.size() != 0)
				{
					std::string log = "Failed to delete org. Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "organisation-deletion"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				res = responseJSONManager.createJSONResponse(true, recordSet, "organisation-deletion");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/orgs/<string>")
		.methods(crow::HTTPMethod::Patch)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		([&](const crow::request& req, crow::response& res, std::string orgName)
			{
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);
				auto updateData = crow::query_string("?" + req.body);

				CROW_LOG_INFO << "User with id: " << ctx.userId <<
					" is trying to update information for organisation with name: " << orgName;

				std::vector<std::string> recordSet = validationManager.checkForNullValues(
					updateData,
					{
						"orgName",
						"password"
					}
				);

				// If there are null values
				if (recordSet.size() != 0)
				{
					std::string log = "Failed to update org. Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "organisation-deletion"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				recordSet = validationManager.isOrgDataValid(updateData, true);

				// If validation falls
				if (recordSet.size() != 0)
				{
					std::string log = "Failed validation/s at organisation with name: " + orgName + " at: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "organisation-update"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				// Try to get organisation's id
				recordSet = dbManager.getOrgIdByName(orgName);

				// If return value is bigger that 10
				// then it is not a number
				if (recordSet[0].size() > 10)
				{
					std::string log = "Failed to get organisation's id with name: " +
						orgName + " from the database. Reason/s: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "organisation-update"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				recordSet = dbManager.isUserInOrgAndGetRole(ctx.userId, std::stoi(recordSet[0]));

				// Error happend
				if (recordSet[0] != "1" || recordSet[1] != "2")
				{
					std::string log = "Failed to update organisation info with name: " +
						orgName + ". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false,
						{ "User is unauthorised" },
						"organisation-update"
					);

					res.code = crow::status::FORBIDDEN;;
					res.end();
					return;
				}

				recordSet = dbManager.updateOrg(
					std::stoi(dbManager.getOrgIdByName(orgName)[0]), updateData
				);

				if (recordSet.size() != 0)
				{
					std::string log = "Failed to update organisation. Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "organisation-update"
					);

					crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				res = responseJSONManager.createJSONResponse(true, recordSet, "organisation-update");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/createNewCourse")
		.methods(crow::HTTPMethod::Post)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		//CROW_MIDDLEWARES(app, AuthorisationMiddleware)
		([&](const crow::request& req, crow::response& res)
			{
				auto reqData = crow::query_string("?" + req.body);
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				std::vector<std::string> recordSet = validationManager.checkForNullValues(
					reqData,
					{
						"courseName",
						"password",
						"orgId"
					},
					{
						"orgId"
					}
					);

				// If there are null values
				if (recordSet.size() != 0)
				{
					res = responseJSONManager.createJSONResponse(
						false, recordSet, "organisation-deletion"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				CROW_LOG_INFO << "User with id: " << ctx.userId << " is trying to create new course with name: "
					<< reqData.get("courseName") << " in organisation with id: " << reqData.get("orgId");

				recordSet = dbManager.isUserInOrgAndGetRole(ctx.userId, std::stoi(reqData.get("orgId")));

				// Error happend
				if (recordSet[0] != "1" || recordSet[1] != "2")
				{
					std::string log = "Failed to create course with name: " +
						std::string(reqData.get("courseName")) + ". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "organisation-deletion"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();

					return;
				}

				recordSet = validationManager.isCourseDataValid(reqData);

				// If validation falls
				if (recordSet.size() != 0)
				{
					std::string log = "Failed validation/s at creating course with name: " +
						std::string(reqData.get("courseName")) + " at: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "course-register"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				char salt[BCRYPT_HASHSIZE];
				char hash[BCRYPT_HASHSIZE];
				int ret;

				ret = bcrypt_gensalt(12, salt);

				if (ret != 0)
				{
					std::string log = "Failed to generate salt";

					res = responseJSONManager.createJSONResponse(
						false, { log }, "course-register"
					);

					res.code = crow::status::INTERNAL_SERVER_ERROR;
					res.end();

					return;
				}

				ret = bcrypt_hashpw(reqData.get("password"), salt, hash);

				if (ret != 0)
				{
					std::string log = "Failed to generate hash";

					res = responseJSONManager.createJSONResponse(
						false, { log }, "course-register"
					);

					res.code = crow::status::INTERNAL_SERVER_ERROR;
					res.end();

					return;
				}

				recordSet = dbManager.createCourse(reqData, hash, salt);

				// If saving to database fails
				if (recordSet.size() != 0)
				{
					std::string log = "Failed to save course with name: " +
						std::string(reqData.get("courseName")) + " to the database. Reason/s: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "course-register"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				CROW_LOG_INFO << "Course with name: " + std::string(reqData.get("courseName")) +
					" is successfully saved into the database.";

				// Create and send request
				res = responseJSONManager.createJSONResponse(true, recordSet, "course-register");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/joinCourse")
		.methods(crow::HTTPMethod::Post)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		//CROW_MIDDLEWARES(app, AuthorisationMiddleware)
		([&](const crow::request& req, crow::response& res)
			{
				auto reqData = crow::query_string("?" + req.body);
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				std::vector<std::string> recordSet = validationManager.checkForNullValues(
					reqData,
					{
						"courseId",
						"password"
					},
					{
						"courseId"
					}
					);

				// If there are null values
				if (recordSet.size() != 0)
				{
					res = responseJSONManager.createJSONResponse(
						false, recordSet, "join-course"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				CROW_LOG_INFO << "User with id: " << ctx.userId <<
					" is trying to join course with id: " << reqData.get("courseId");

				CourseInfo courseInfo = dbManager.getCourseInfo(std::stoi(reqData.get("courseId")));

				if (!courseInfo.errors.empty())
				{
					std::string log = "Failed to get course info for course with id: " +
						std::string(reqData.get("courseId")) + ". Reasons: ";

					for (auto el : courseInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, courseInfo.errors, "join-course"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				recordSet = dbManager.isUserInOrgAndGetRole(ctx.userId, courseInfo.orgId);

				// Error happend
				if (recordSet[0] != "1")
				{
					std::string log = "Failed to join in course with id: " +
						std::string(reqData.get("courseId")) + ". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "join-course"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();

					return;
				}

				recordSet = validationManager.isJoinOrgDataValid(reqData);

				// If validation falls
				if (recordSet.size() != 0)
				{
					std::string log = "Failed validation/s at joining in course with id: " +
						std::string(reqData.get("courseId")) + " at: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "join-course"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				recordSet = dbManager.doesPasswordMatchCourse(
					reqData.get("password"), std::stoi(reqData.get("courseId"))
				);

				// If password is incorrect
				if (recordSet[0] == "0")
				{
					recordSet[0] = "Password is incorrect";
				}

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to join user with id: " +
						std::to_string(ctx.userId) + " to course with id: " +
						std::string(reqData.get("courseId")) + ". Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "join-course"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				recordSet = dbManager.addUserToCourse(
					ctx.userId, std::stoi(reqData.get("courseId")), UserRolesInOrgs::USER
				);

				if (recordSet.size() != 0)
				{
					std::string log = "Failed to add user with id: " +
						std::to_string(ctx.userId) + " to course with id: " +
						std::string(reqData.get("courseId")) + ". Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "join-course"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				CROW_LOG_INFO << "User with id: " << ctx.userId <<
					" is successfully added to course with id: " << reqData.get("courseId");

				res = responseJSONManager.createJSONResponse(true, recordSet, "join-course");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/addTeacher")
		.methods(crow::HTTPMethod::Post)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		//CROW_MIDDLEWARES(app, AuthorisationMiddleware)
		([&](const crow::request& req, crow::response& res)
			{
				auto reqData = crow::query_string("?" + req.body);
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				std::vector<std::string> recordSet = validationManager.checkForNullValues(
					reqData,
					{
						"courseId",
						"teacherId"
					},
					{
						"courseId",
						"teacherId"
					}
					);

				// If there are null values
				if (recordSet.size() != 0)
				{
					res = responseJSONManager.createJSONResponse(
						false, recordSet, "add-teacher"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				CROW_LOG_INFO << "User with id: " << ctx.userId << " is trying to add teacher with id: " <<
					reqData.get("teacherId") << " to course with id: " << reqData.get("courseId");

				CourseInfo courseInfo = dbManager.getCourseInfo(std::stoi(reqData.get("courseId")));

				if (!courseInfo.errors.empty())
				{
					std::string log = "Failed to get course info for course with name: " +
						std::string(reqData.get("courseId")) + ". Reasons: ";

					for (auto el : courseInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, courseInfo.errors, "add-techer"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				recordSet = dbManager.isUserInOrgAndGetRole(ctx.userId, courseInfo.orgId);

				// User is not admin
				if (recordSet[0] != "1" || recordSet[1] != "2")
				{
					std::string log = "Failed to add teacher with id: " +
						std::string(reqData.get("teacherId")) +
						" to course with id : " + std::string(reqData.get("courseId")) +
						". Reason : User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "add-techer"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();

					return;
				}

				recordSet = dbManager.isUserInOrgAndGetRole(
					std::stoi(reqData.get("teacherId")), courseInfo.orgId
				);

				// User is not admin
				if (recordSet[0] != "1" || recordSet[1] != "1")
				{
					std::string log = "Failed to add teacher with id: " +
						std::string(reqData.get("teacherId")) + " to course with id: " +
						std::string(reqData.get("courseId")) + ". Reason : User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "The user is not a teacher in this org" }, "add-techer"
					);
					res.code = crow::status::FORBIDDEN;
					res.end();
					return;
				}

				recordSet = dbManager.addUserToCourse(
					std::stoi(reqData.get("teacherId")),
					std::stoi(reqData.get("courseId")),
					UserRolesInOrgs::TEACHER
				);

				if (recordSet.size() != 0)
				{
					std::string log = "Failed to add user with id: " + std::to_string(ctx.userId) +
						" to course with id: " + std::string(reqData.get("courseId")) + ". Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "add-techer"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				CROW_LOG_INFO << "Teacger with id: " << reqData.get("teacherId") <<
					" is successfully added to course with id: " << reqData.get("courseId");

				res = responseJSONManager.createJSONResponse(true, recordSet, "add-techer");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/courses/<int>")
		.methods(crow::HTTPMethod::Get)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		([&](const crow::request& req, crow::response& res, int courseId)
			{
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				CROW_LOG_INFO << "User with id: " << ctx.userId <<
					" is trying to get information for course: " << courseId;

				CourseInfo courseInfo = dbManager.getCourseInfo(courseId);

				if (!courseInfo.errors.empty())
				{
					std::string log = "Failed to get course info for course with name: " +
						std::to_string(courseId) + ". Reasons: ";

					for (auto el : courseInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, courseInfo.errors, "get-course"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				std::vector<std::string> recordSet = dbManager.isUserInOrgAndGetRole(
					ctx.userId, courseInfo.orgId
				);

				// Error happend
				if (recordSet[0] != "1")
				{
					std::string log = "Failed to get course info with id: " +
						std::to_string(courseId) + ". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "get-course"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();

					return;
				}

				if (recordSet[1] == "2")
				{
					res = responseJSONManager.createCourseJSONResponse(courseInfo);
					res.end();
					return;
				}

				recordSet = dbManager.isUserInCourseAndGetRole(ctx.userId, courseInfo.id);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to get course info with id: " +
						std::to_string(courseId) + ". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "get-course"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();

					return;
				}

				res = responseJSONManager.createCourseJSONResponse(courseInfo);
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/courses/<int>")
		.methods(crow::HTTPMethod::Delete)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		([&](const crow::request& req, crow::response& res, int courseId)
			{
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				CROW_LOG_INFO << "User with id: " << ctx.userId <<
					" is trying to delete course with id: " << courseId;

				CourseInfo courseInfo = dbManager.getCourseInfo(courseId);

				if (!courseInfo.errors.empty())
				{
					std::string log = "Failed to get course info for course with id: " +
						std::to_string(courseId) + ". Reasons: ";

					for (auto el : courseInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, courseInfo.errors, "course-deletion"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				std::vector<std::string> recordSet = dbManager.isUserInOrgAndGetRole(
					ctx.userId, courseInfo.orgId
				);

				// Error happend
				if (recordSet[0] != "1" || recordSet[1] != "2")
				{
					std::string log = "Failed to delete course with id: " +
						std::to_string(courseId) + ". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "course-deletion"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();

					return;
				}

				recordSet = dbManager.deleteCourse(courseInfo.id);

				if (recordSet.size() != 0)
				{
					std::string log = "Failed to delete course. Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "course-deletion"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				res = responseJSONManager.createJSONResponse(true, recordSet, "course-deletion");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/courses/<int>")
		.methods(crow::HTTPMethod::Patch)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		([&](const crow::request& req, crow::response& res, int courseId)
			{
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);
				auto updateData = crow::query_string("?" + req.body);

				CROW_LOG_INFO << "User with id: " << ctx.userId <<
					" is trying to update information for course with id: " << courseId;

				std::vector<std::string> recordSet = validationManager.checkForNullValues(
					updateData,
					{
						"courseName",
						"password"
					}
				);

				// If there are null values
				if (recordSet.size() != 0)
				{
					std::string log = "Failed to update course. Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "course-update"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				recordSet = validationManager.isCourseDataValid(updateData, true);

				// If validation falls
				if (recordSet.size() != 0)
				{
					std::string log = "Failed validation/s at course with id: " +
						std::to_string(courseId) + " at: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "course-update"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				recordSet = dbManager.canUserAccessCourse(courseId, ctx.userId, true);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to update course info with id: " +
						std::to_string(courseId) + ". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "course-update"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();

					return;
				}

				recordSet = dbManager.updateCourse(courseId, updateData);

				if (recordSet.size() != 0)
				{
					std::string log = "Failed to update course. Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "course-update"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				res = responseJSONManager.createJSONResponse(true, recordSet, "course-update");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/createNewTopic")
		.methods(crow::HTTPMethod::Post)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		//CROW_MIDDLEWARES(app, AuthorisationMiddleware)
		([&](const crow::request& req, crow::response& res)
			{
				auto reqData = crow::query_string("?" + req.body);
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				std::vector<std::string> recordSet = validationManager.checkForNullValues(
					reqData,
					{
						"topicName",
						"courseId"
					},
					{
						"courseId"
					}
					);

				// If there are null values
				if (recordSet.size() != 0)
				{
					res = responseJSONManager.createJSONResponse(
						false, recordSet, "topic-register"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				CROW_LOG_INFO << "User with id: " << ctx.userId <<
					" is trying to create new topic with name: " <<
					reqData.get("topicName") << " in course with id: " <<
					reqData.get("courseId");

				recordSet = dbManager.canUserAccessCourse(std::stoi(reqData.get("courseId")), ctx.userId, true);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to create new topic with name: " +
						std::string(reqData.get("topicName")) +
						". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "topic-register"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();
					return;
				}

				recordSet = dbManager.createTopic(reqData);

				// If saving to database fails
				if (recordSet.size() != 0)
				{
					std::string log = "Failed to save topic with name: " +
						std::string(reqData.get("topicName")) + " to the database. Reason/s: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "topic-register"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				CROW_LOG_INFO << "Topic with name: " + std::string(reqData.get("topicName")) +
					" is successfully saved into the database.";

				// Create and send request
				res = responseJSONManager.createJSONResponse(true, recordSet, "topic-register");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/topics/<int>")
		.methods(crow::HTTPMethod::Get)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		([&](const crow::request& req, crow::response& res, int topicId)
			{
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				CROW_LOG_INFO << "User with id: " << ctx.userId << \
					" is trying to get information for topic: " << topicId;

				TopicInfo topicInfo = dbManager.getTopicInfo(topicId);

				if (!topicInfo.errors.empty())
				{
					std::string log = "Failed to get topic info for topic with id: " +
						std::to_string(topicId) + ". Reasons: ";

					for (auto el : topicInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, topicInfo.errors, "get-topic"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				std::vector<std::string> recordSet = dbManager.canUserAccessCourse(
					topicInfo.courseId, ctx.userId
				);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to get topic with id: " +
						std::to_string(topicId) + ". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "get-topic"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();

					return;
				}

				res = responseJSONManager.createTopicJSONResponse(topicInfo);
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/topics/<int>")
		.methods(crow::HTTPMethod::Delete)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		([&](const crow::request& req, crow::response& res, int topicId)
			{
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				CROW_LOG_INFO << "User with id: " << ctx.userId <<
					" is trying to delete topic with id: " << topicId;

				TopicInfo topicInfo = dbManager.getTopicInfo(topicId);

				if (!topicInfo.errors.empty())
				{
					std::string log = "Failed to delete topic with id: " +
						std::to_string(topicId) + ". Reasons: ";

					for (auto el : topicInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, topicInfo.errors, "topic-deletion"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				std::vector<std::string> recordSet = dbManager.canUserAccessCourse(
					topicInfo.courseId, ctx.userId, true
				);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to delete topic with id: " +
						std::to_string(topicId) + ". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "topic-deletion"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();

					return;
				}

				recordSet = dbManager.deleteTopic(topicId);

				if (recordSet.size() != 0)
				{
					std::string log = "Failed to delete topic. Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "topic-deletion"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				res = responseJSONManager.createJSONResponse(true, recordSet, "topic-deletion");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/topics/<int>")
		.methods(crow::HTTPMethod::Patch)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		([&](const crow::request& req, crow::response& res, int topicId)
			{
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);
				auto updateData = crow::query_string("?" + req.body);

				CROW_LOG_INFO << "User with id: " << ctx.userId <<
					" is trying to update information for topic with id: " << topicId;

				std::vector<std::string> recordSet = validationManager.checkForNullValues(
					updateData,
					{
						"topicName"
					}
				);

				// If there are null values
				if (recordSet.size() != 0)
				{
					res = responseJSONManager.createJSONResponse(
						false, recordSet, "topic-update"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				TopicInfo topicInfo = dbManager.getTopicInfo(topicId);

				if (!topicInfo.errors.empty())
				{
					std::string log = "Failed to update topic with id: " +
						std::to_string(topicId) + ". Reasons: ";

					for (auto el : topicInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, topicInfo.errors, "topic-update"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				recordSet = dbManager.canUserAccessCourse(topicInfo.courseId, ctx.userId, true);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to update topic with id: " +
						std::to_string(topicId) + ". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "topic-update"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();
					return;
				}

				recordSet = dbManager.updateTopic(topicId, updateData);

				if (recordSet.size() != 0)
				{
					std::string log = "Failed to update topic. Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "topic-update"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				res = responseJSONManager.createJSONResponse(true, recordSet, "topic-update");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/createNewLesson")
		.methods(crow::HTTPMethod::Post)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		//CROW_MIDDLEWARES(app, AuthorisationMiddleware)
		([&](const crow::request& req, crow::response& res)
			{
				auto reqData = crow::query_string("?" + req.body);
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				std::vector<std::string> recordSet = validationManager.checkForNullValues(
					reqData,
					{
						"lessonName",
						"lessonData",
						"topicId"
					},
					{
						"topicId"
					}
					);

				// If there are null values
				if (recordSet.size() != 0)
				{
					res = responseJSONManager.createJSONResponse(
						false, recordSet, "lesson-register"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				CROW_LOG_INFO << "User with id: " << ctx.userId <<
					" is trying to create new lesson with name: " <<
					reqData.get("lessonName") << " in topic with id: " <<
					reqData.get("topicId");

				if (
					std::string(reqData.get("lessonData")).size() == 0 ||
					std::string(reqData.get("lessonName")).size() == 0
					)
				{
					std::string log = "Failed to create new lesson with name: " +
						std::string(reqData.get("lessonName")) +
						". Reason: Lessond data or Lesson Name is empty";
					recordSet.push_back("Lessond data or name is empty");

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "lesson-register"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				TopicInfo topicInfo = dbManager.getTopicInfo(std::stoi(reqData.get("topicId")));

				if (!topicInfo.errors.empty())
				{
					std::string log = "Failed to get topic info for topic with id: " +
						std::string(reqData.get("topicId")) + ". Reasons: ";

					for (auto el : topicInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, topicInfo.errors, "lesson-register"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				recordSet = dbManager.canUserAccessCourse(
					std::stoi(reqData.get("topicId")), ctx.userId, true
				);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to create new lesson with name: " +
						std::string(reqData.get("lessonName")) +
						". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "lesson-register"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();

					return;
				}

				recordSet = dbManager.createLesson(reqData);

				// If saving to database fails
				if (recordSet.size() != 0)
				{
					std::string log = "Failed to save lesson with name: " +
						std::string(reqData.get("lessonName")) +
						" to the database. Reason/s: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "lesson-register"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				CROW_LOG_INFO << "Lesson with name: " +
					std::string(reqData.get("lessonName")) +
					" is successfully saved into the database.";

				// Create and send request
				res = responseJSONManager.createJSONResponse(true, recordSet, "lesson-register");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/lessons/<int>")
		.methods(crow::HTTPMethod::Get)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		([&](const crow::request& req, crow::response& res, int lessonId)
			{
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				CROW_LOG_INFO << "User with id: " << ctx.userId <<
					" is trying to get information for lesson with id: " << lessonId;

				LessonInfo lessonInfo = dbManager.getLessonInfo(lessonId);

				if (!lessonInfo.errors.empty())
				{
					std::string log = "Failed to get lesson info for topic with id: " +
						std::to_string(lessonId) + ". Reasons: ";

					for (auto el : lessonInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, lessonInfo.errors, "get-lesson"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				TopicInfo topicInfo = dbManager.getTopicInfo(lessonInfo.topicId);

				std::vector<std::string> recordSet = dbManager.canUserAccessCourse(
					topicInfo.courseId, ctx.userId
				);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to get lesson with id: " +
						std::to_string(lessonId) + ". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "get-lesson"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();

					return;
				}

				res = responseJSONManager.createLessonJSONResponse(lessonInfo);
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/lessons/<int>")
		.methods(crow::HTTPMethod::Delete)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		([&](const crow::request& req, crow::response& res, int lessonId)
			{
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				CROW_LOG_INFO << "User with id: " << ctx.userId <<
					" is trying to delete lesson with id: " << lessonId;

				LessonInfo lessonInfo = dbManager.getLessonInfo(lessonId);

				if (!lessonInfo.errors.empty())
				{
					std::string log = "Failed to delete lesson with id: " +
						std::to_string(lessonId) + ". Reasons: ";

					for (auto el : lessonInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, lessonInfo.errors, "lesson-deletion"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				TopicInfo topicInfo = dbManager.getTopicInfo(lessonInfo.topicId);

				std::vector<std::string> recordSet = dbManager.canUserAccessCourse(
					topicInfo.courseId, ctx.userId, true
				);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to delete lesson with id: " +
						std::to_string(lessonId) + ". Reason: User is unauthorised";
					recordSet[0] = "User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "lesson-deletion"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();

					return;
				}

				recordSet = dbManager.deleteLesson(lessonId);

				if (recordSet.size() != 0)
				{
					std::string log = "Failed to delete lesson. Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "lesson-deletion");
					res.end();
					return;
				}

				res = responseJSONManager.createJSONResponse(true, recordSet, "lesson-deletion");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/lessons/<int>")
		.methods(crow::HTTPMethod::Patch)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		([&](const crow::request& req, crow::response& res, int lessonId)
			{
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);
				auto updateData = crow::query_string("?" + req.body);

				CROW_LOG_INFO << "User with id: " << ctx.userId <<
					" is trying to update information for lesson with id: " << lessonId;

				std::vector<std::string> recordSet = validationManager.checkForNullValues(
					updateData,
					{
						"lessonName",
						"lessonData"
					}
				);

				// If there are null values
				if (recordSet.size() != 0)
				{
					std::string log = "Failed to update lesson with id: " +
						std::to_string(lessonId) + ". Reasons: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "lesson-update"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				if (
					std::string(updateData.get("lessonData")).size() == 0 ||
					std::string(updateData.get("lessonName")).size() == 0
					)
				{
					std::string log = "Failed to update lesson with name: " +
						std::string(updateData.get("lessonName")) +
						". Reason: Lessond data or Lesson name is empty";
					recordSet.push_back("Lessond data or name is empty");

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "lesson-update"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				LessonInfo lessonInfo = dbManager.getLessonInfo(lessonId);

				if (!lessonInfo.errors.empty())
				{
					std::string log = "Failed to update lesson with id: " +
						std::to_string(lessonId) + ". Reasons: ";

					for (auto el : lessonInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, lessonInfo.errors, "leson-update"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				TopicInfo topicInfo = dbManager.getTopicInfo(lessonInfo.topicId);

				recordSet = dbManager.canUserAccessCourse(topicInfo.courseId, ctx.userId, true);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to update lesson with id: " +
						std::to_string(lessonId) +
						". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "lesson-update"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();

					return;
				}

				recordSet = dbManager.updateLesson(lessonId, updateData);

				if (recordSet.size() != 0)
				{
					std::string log = "Failed to update lesson. Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "lesson-update"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				res = responseJSONManager.createJSONResponse(true, recordSet, "lesson-update");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/createNewQuiz")
		.methods(crow::HTTPMethod::Post)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		//CROW_MIDDLEWARES(app, AuthorisationMiddleware)
		([&](const crow::request& req, crow::response& res)
			{
				auto reqData = crow::query_string("?" + req.body);
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				std::vector<std::string> recordSet = validationManager.checkForNullValues(
					reqData,
					{
						"quizName",
						"topicId"
					},
					{
						"topicId"
					}
					);

				// If there are null values
				if (recordSet.size() != 0)
				{
					res = responseJSONManager.createJSONResponse(
						false, recordSet, "quiz-register"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				CROW_LOG_INFO << "User with id: " << ctx.userId <<
					" is trying to create new quiz with name: " <<
					reqData.get("quizName") << " in topic with id: " << reqData.get("topicId");

				if (std::string(reqData.get("quizName")).size() == 0)
				{
					std::string log = "Failed to create new quiz with name: " +
						std::string(reqData.get("quizName")) + ". Reason: Quiz name is empty";

					recordSet.push_back("Lessond data is empty");

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "quiz-register"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				TopicInfo topicInfo = dbManager.getTopicInfo(std::stoi(reqData.get("topicId")));

				if (!topicInfo.errors.empty())
				{
					std::string log = "Failed to get topic info for topic with id: " +
						std::string(reqData.get("topicId")) + ". Reasons: ";

					for (auto el : topicInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, topicInfo.errors, "quiz-register"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				recordSet = dbManager.canUserAccessCourse(topicInfo.courseId, ctx.userId, true);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to create new quiz with name: " +
						std::string(reqData.get("quizName")) + ". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "quiz-register"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();

					return;
				}

				recordSet = dbManager.createQuiz(reqData);

				// If saving to database fails
				if (recordSet.size() != 0)
				{
					std::string log = "Failed to save quiz with name: " +
						std::string(reqData.get("quizName")) +
						" to the database. Reason/s: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "quiz-register"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				CROW_LOG_INFO << "Quiz with name: " + std::string(reqData.get("quizName")) +
					" is successfully saved into the database.";

				// Create and send request
				res = responseJSONManager.createJSONResponse(true, recordSet, "quiz-register");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/quizzes/<int>")
		.methods(crow::HTTPMethod::Get)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		([&](const crow::request& req, crow::response& res, int quizId)
			{
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				CROW_LOG_INFO << "User with id: " << ctx.userId <<
					" is trying to get information for quiz with id: " << quizId;

				QuizInfo quizInfo = dbManager.getQuizInfo(quizId);

				if (!quizInfo.errors.empty())
				{
					std::string log = "Failed to get quiz info for topic with id: " +
						std::to_string(quizId) + ". Reasons: ";

					for (auto el : quizInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, quizInfo.errors, "get-quiz"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();
					return;
				}

				TopicInfo topicInfo = dbManager.getTopicInfo(quizInfo.topicId);

				std::vector<std::string> recordSet = dbManager.canUserAccessCourse(topicInfo.courseId, ctx.userId);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to get quiz with id: " +
						std::to_string(quizId) + ". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "get-quiz"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();

					return;
				}

				res = responseJSONManager.createQuizJSONResponse(
					quizInfo,
					dbManager.isUserInCourseAndGetRole(
						ctx.userId,
						topicInfo.courseId
					)[1] != "0"
				);

				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/quizzes/<int>")
		.methods(crow::HTTPMethod::Delete)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		([&](const crow::request& req, crow::response& res, int quizId)
			{
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				CROW_LOG_INFO << "User with id: " << ctx.userId <<
					" is trying to delete lesson with id: " << quizId;

				QuizInfo quizInfo = dbManager.getQuizInfo(quizId);

				if (!quizInfo.errors.empty())
				{
					std::string log = "Failed to delete quiz with id: " +
						std::to_string(quizId) + ". Reasons: ";

					for (auto el : quizInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, quizInfo.errors, "quiz-deletion"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				TopicInfo topicInfo = dbManager.getTopicInfo(quizInfo.topicId);

				std::vector<std::string> recordSet = dbManager.canUserAccessCourse(
					topicInfo.courseId, ctx.userId, true
				);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to delete quiz with id: " +
						std::to_string(quizId) + ". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "quiz-deletion"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();

					return;
				}

				recordSet = dbManager.deleteQuiz(quizId);

				if (recordSet.size() != 0)
				{
					std::string log = "Failed to delete quiz. Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "quiz-deletion"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				res = responseJSONManager.createJSONResponse(true, recordSet, "quiz-deletion");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/quizzes/<int>")
		.methods(crow::HTTPMethod::Patch)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		([&](const crow::request& req, crow::response& res, int quizId)
			{
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);
				auto updateData = crow::query_string("?" + req.body);

				CROW_LOG_INFO << "User with id: " << ctx.userId <<
					" is trying to update information for quiz with id: " << quizId;

				std::vector<std::string> recordSet = validationManager.checkForNullValues(
					updateData,
					{
						"quizName"
					}
				);

				// If there are null values
				if (recordSet.size() != 0)
				{
					res = responseJSONManager.createJSONResponse(
						false, recordSet, "quiz-update"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				if (std::string(updateData.get("quizName")).size() == 0)
				{
					std::string log = "Failed to create new quiz with name: " +
						std::string(updateData.get("quizName")) +
						". Reason: Quiz name is empty";

					recordSet.push_back("Quiz name is empty");

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "quiz-register"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				QuizInfo lessonInfo = dbManager.getQuizInfo(quizId);

				if (!lessonInfo.errors.empty())
				{
					std::string log = "Failed to update quiz with id: " +
						std::to_string(quizId) + ". Reasons: ";

					for (auto el : lessonInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, lessonInfo.errors, "quiz-update"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				TopicInfo topicInfo = dbManager.getTopicInfo(lessonInfo.topicId);

				recordSet = dbManager.canUserAccessCourse(topicInfo.courseId, ctx.userId, true);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to update quiz with id: " +
						std::to_string(quizId) + ". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "quiz-update"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();

					return;
				}

				recordSet = dbManager.updateQuiz(quizId, updateData);

				if (recordSet.size() != 0)
				{
					std::string log = "Failed to update quiz. Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "quiz-update"
					);
					res.code = crow::status::BAD_REQUEST;

					res.end();
					return;
				}

				res = responseJSONManager.createJSONResponse(true, recordSet, "quiz-update");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/createNewQuestion")
		.methods(crow::HTTPMethod::Post)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		//CROW_MIDDLEWARES(app, AuthorisationMiddleware)
		([&](const crow::request& req, crow::response& res)
			{
				auto reqData = crow::query_string("?" + req.body);
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				CROW_LOG_INFO << "User with id: " << ctx.userId <<
					" is trying to create new question in quiz with id: " << reqData.get("quizId");

				std::string type = reqData.get("type");

				if (type == "2")
				{
					std::vector<std::string> recordSet = validationManager.isPunnettSquareDataValid(reqData);

					// If validation falls
					if (recordSet.size() != 0)
					{
						std::string log = "Failed validation/s for punnett square at: ";

						for (auto el : recordSet)
						{
							log += el + " ";
						}

						CROW_LOG_WARNING << log;

						res = responseJSONManager.createJSONResponse(
							false, recordSet, "question-register"
						);

						res.code = crow::status::BAD_REQUEST;
						res.end();

						return;
					}
				}

				QuizInfo quizInfo = dbManager.getQuizInfo(std::stoi(reqData.get("quizId")));

				if (!quizInfo.errors.empty())
				{
					std::string log = "Failed to get quiz info for quiz with id: " +
						std::string(reqData.get("quizId")) + ". Reasons: ";

					for (auto el : quizInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, quizInfo.errors, "question-register"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();
					return;
				}

				TopicInfo topicInfo = dbManager.getTopicInfo(quizInfo.topicId);

				if (!topicInfo.errors.empty())
				{
					std::string log = "Failed to get topic info for topic with id: " +
						std::to_string(quizInfo.topicId) + ". Reasons: ";

					for (auto el : topicInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, topicInfo.errors, "question-register"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();
					return;
				}

				std::vector<std::string> recordSet = dbManager.canUserAccessCourse(
					topicInfo.courseId, ctx.userId, true
				);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to create new question in quiz with id: " +
						std::string(reqData.get("quizId")) + ". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "question-register"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();

					return;
				}

				recordSet = dbManager.createQuestion(reqData);

				// If saving to database fails
				if (recordSet.size() != 0)
				{
					std::string log = "Failed to save question in quiz with id: " +
						std::string(reqData.get("quizId")) + " to the database. Reason/s: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "question-register"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				CROW_LOG_INFO << "Question in quiz with id: " +
					std::string(reqData.get("quizId")) + " is successfully saved into the database.";

				// Create and send request
				res = responseJSONManager.createJSONResponse(true, recordSet, "question-register");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/questions/<int>")
		.methods(crow::HTTPMethod::Get)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		([&](const crow::request& req, crow::response& res, int questionId)
			{
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				CROW_LOG_INFO << "User with id: " << ctx.userId <<
					" is trying to get information for question with id: " << questionId;

				QuestionInfo questionInfo = dbManager.getQuestionInfo(questionId);

				if (!questionInfo.errors.empty())
				{
					std::string log = "Failed to get question info for question with id: " +
						std::to_string(questionId) + ". Reasons: ";

					for (auto el : questionInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, questionInfo.errors, "get-question"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				QuizInfo quizInfo = dbManager.getQuizInfo(questionInfo.quizId);

				TopicInfo topicInfo = dbManager.getTopicInfo(quizInfo.topicId);

				std::vector<std::string> recordSet = dbManager.canUserAccessCourse(
					topicInfo.courseId, ctx.userId, true
				);

				if (recordSet[0] != "1")
				{
					int result = dbManager.getAttemptIdByUserIdAndQuizId(ctx.userId, questionInfo.quizId);

					if (result != -1)
					{
						AttemptInfo attemptInfo = dbManager.getAttemptInfo(result);
						if (
							ctx.userId == attemptInfo.userId &&
							attemptInfo.currentQuestionId == questionId
							)
						{
							res = responseJSONManager.createQuestionJSONResponse(questionInfo, false);
							res.end();
							return;
						}
					}

					std::string log = "Failed to get quiz with id: " +
						std::to_string(questionId) + ". Reason: User is unauthorised";

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "get-question"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();

					return;
				}

				res = responseJSONManager.createQuestionJSONResponse(questionInfo);
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/questions/<int>")
		.methods(crow::HTTPMethod::Delete)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		([&](const crow::request& req, crow::response& res, int questionId)
			{
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				CROW_LOG_INFO << "User with id: " << ctx.userId <<
					" is trying to delete question with id: " << questionId;

				QuestionInfo questionInfo = dbManager.getQuestionInfo(questionId);

				if (!questionInfo.errors.empty())
				{
					std::string log = "Failed to get question info for question with id: " +
						std::to_string(questionId) + ". Reasons: ";

					for (auto el : questionInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, questionInfo.errors, "question-deletion"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				QuizInfo quizInfo = dbManager.getQuizInfo(questionInfo.quizId);

				TopicInfo topicInfo = dbManager.getTopicInfo(quizInfo.topicId);

				std::vector<std::string> recordSet = dbManager.canUserAccessCourse(
					topicInfo.courseId, ctx.userId, true
				);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to delete question with id: " +
						std::to_string(questionId) + ". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "question-deletion"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();

					return;
				}

				recordSet = dbManager.deleteQuestion(questionId);

				if (recordSet.size() != 0)
				{
					std::string log = "Failed to delete question. Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "question-deletion"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				res = responseJSONManager.createJSONResponse(true, recordSet, "question-deletion");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/questions/<int>")
		.methods(crow::HTTPMethod::Patch)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		([&](const crow::request& req, crow::response& res, int questionId)
			{
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);
				auto updateData = crow::query_string("?" + req.body);

				CROW_LOG_INFO << "User with id: " << ctx.userId <<
					" is trying to update information for question with id: " << questionId;

				std::string type = updateData.get("type");

				if (type == "2")
				{
					std::vector<std::string> recordSet = validationManager.isPunnettSquareDataValid(
						updateData, true
					);

					// If validation falls
					if (recordSet.size() != 0)
					{
						std::string log = "Failed validation/s for punnett square at: ";

						for (auto el : recordSet)
						{
							log += el + " ";
						}

						CROW_LOG_WARNING << log;

						res = responseJSONManager.createJSONResponse(
							false, recordSet, "question-update"
						);

						res.code = crow::status::BAD_REQUEST;
						res.end();

						return;
					}
				}

				QuestionInfo questionInfo = dbManager.getQuestionInfo(questionId);

				if (!questionInfo.errors.empty())
				{
					std::string log = "Failed to get question info for question with id: " +
						std::to_string(questionId) + ". Reasons: ";

					for (auto el : questionInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, questionInfo.errors, "question-update"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				QuizInfo quizInfo = dbManager.getQuizInfo(questionInfo.quizId);

				TopicInfo topicInfo = dbManager.getTopicInfo(quizInfo.topicId);

				std::vector<std::string> recordSet = dbManager.canUserAccessCourse(
					topicInfo.courseId, ctx.userId, true
				);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to update question with id: " +
						std::to_string(questionId) + ". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "question-update"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();

					return;
				}

				recordSet = dbManager.updateQuestion(questionId, updateData);

				if (recordSet.size() != 0)
				{
					std::string log = "Failed to update question. Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "question-update"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				res = responseJSONManager.createJSONResponse(true, recordSet, "question-update");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/startAttempt")
		.methods(crow::HTTPMethod::Post)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		//CROW_MIDDLEWARES(app, AuthorisationMiddleware)
		([&](const crow::request& req, crow::response& res)
			{
				auto reqData = crow::query_string("?" + req.body);
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				std::vector<std::string> recordSet = validationManager.checkForNullValues(
					reqData,
					{
						"quizId"
					},
					{
						"quizId"
					}
					);

				// If there are null values
				if (recordSet.size() != 0)
				{
					res = responseJSONManager.createJSONResponse(
						false, recordSet, "attempt-start"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				CROW_LOG_INFO << "User with id: " << ctx.userId <<
					" is trying to start attempt in quiz with id: " << reqData.get("quizId");

				QuizInfo quizInfo = dbManager.getQuizInfo(std::stoi(reqData.get("quizId")));

				if (!quizInfo.errors.empty())
				{
					std::string log = "Failed to get quiz info for quiz with id: " +
						std::string(reqData.get("quizId")) + ". Reasons: ";

					for (auto el : quizInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, quizInfo.errors, "attempt-start"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				TopicInfo topicInfo = dbManager.getTopicInfo(quizInfo.topicId);

				if (!topicInfo.errors.empty())
				{
					std::string log = "Failed to get topic info for topic with id: " +
						std::to_string(quizInfo.topicId) + ". Reasons: ";

					for (auto el : topicInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, topicInfo.errors, "attempt-start"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				recordSet = dbManager.canUserAccessCourse(topicInfo.courseId, ctx.userId);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to start attempt in quiz with id: " +
						std::string(reqData.get("quizId")) + ". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "attempt-start"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();

					return;
				}

				recordSet = dbManager.startAttempt(reqData, ctx.userId);

				// If saving to database fails
				if (recordSet[0].find("Attempt-id: ") == std::string::npos)
				{
					std::string log = "Failed to save attempt for quiz with id: " +
						std::string(reqData.get("quizId")) + " to the database. Reason/s: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "attempt-start"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				AttemptInfo attemptInfo = dbManager.getAttemptInfo(std::stoi(recordSet[0].substr(12)));

				CROW_LOG_INFO << "Attempt for quiz with id: " + std::string(reqData.get("quizId")) +
					" is successfully saved into the database.";

				// Create and send request
				res = responseJSONManager.createAttemptJSONResponse(attemptInfo, "attempt-start");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/attempts/<int>")
		.methods(crow::HTTPMethod::Get)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		([&](const crow::request& req, crow::response& res, int attemptId)
			{
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				CROW_LOG_INFO << "User with id: " << ctx.userId <<
					" is trying to get information for question with id: " << attemptId;

				AttemptInfo attemptInfo = dbManager.getAttemptInfo(attemptId);

				if (!attemptInfo.errors.empty())
				{
					std::string log = "Failed to get attempt info for attempt with id: " +
						std::to_string(attemptId) + ". Reasons: ";

					for (auto el : attemptInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, attemptInfo.errors, "get-attempt"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				QuizInfo quizInfo = dbManager.getQuizInfo(attemptInfo.quizId);

				TopicInfo topicInfo = dbManager.getTopicInfo(quizInfo.topicId);

				std::vector<std::string> recordSet = dbManager.canUserAccessCourse(
					topicInfo.courseId, ctx.userId, true
				);

				if (recordSet[0] != "1" && ctx.userId != attemptInfo.userId)
				{
					std::string log = "Failed to get attempt with id: " +
						std::to_string(attemptId) + ". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "get-attempt"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();

					return;
				}

				res = responseJSONManager.createAttemptJSONResponse(attemptInfo, "get-attempt");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/attempts/<int>")
		.methods(crow::HTTPMethod::Delete)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		([&](const crow::request& req, crow::response& res, int attemptId)
			{
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				CROW_LOG_INFO << "User with id: " << ctx.userId <<
					" is trying to delete attempt with id: " << attemptId;

				AttemptInfo attemptInfo = dbManager.getAttemptInfo(attemptId);

				if (!attemptInfo.errors.empty())
				{
					std::string log = "Failed to get attempt info for question with id: " +
						std::to_string(attemptId) + ". Reasons: ";

					for (auto el : attemptInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, attemptInfo.errors, "attempt-deletion"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				QuizInfo quizInfo = dbManager.getQuizInfo(attemptInfo.quizId);

				TopicInfo topicInfo = dbManager.getTopicInfo(quizInfo.topicId);

				std::vector<std::string> recordSet = dbManager.canUserAccessCourse(
					topicInfo.courseId, ctx.userId, true
				);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to delete attempt with id: " + 
						std::to_string(attemptId) + ". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "attempt-deletion"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();

					return;
				}

				recordSet = dbManager.deleteAttempt(attemptId);

				if (recordSet.size() != 0)
				{
					std::string log = "Failed to delete attempt. Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "attempt-deletion"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				res = responseJSONManager.createJSONResponse(true, recordSet, "question-deletion");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/answer")
		.methods(crow::HTTPMethod::Post)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		//CROW_MIDDLEWARES(app, AuthorisationMiddleware)
		([&](const crow::request& req, crow::response& res)
			{
				auto reqData = crow::query_string("?" + req.body);
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				CROW_LOG_INFO << "User with id: " << ctx.userId <<
					" is trying to answer question with id: " << reqData.get("questionId");

				QuestionInfo questionInfo = dbManager.getQuestionInfo(std::stoi(reqData.get("questionId")));

				if (!questionInfo.errors.empty())
				{
					std::string log = "Failed to get question info for question with id: " +
						std::string(reqData.get("questionId")) + ". Reasons: ";

					for (auto el : questionInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, questionInfo.errors, "answer"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				QuizInfo quizInfo = dbManager.getQuizInfo(questionInfo.quizId);

				if (!quizInfo.errors.empty())
				{
					std::string log = "Failed to get quiz info for quiz with id: " +
						std::to_string(questionInfo.quizId) + ". Reasons: ";

					for (auto el : quizInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, quizInfo.errors, "answer"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				AttemptInfo attemptInfo = dbManager.getAttemptInfo(std::stoi(reqData.get("attemptId")));

				if (!attemptInfo.errors.empty())
				{
					std::string log = "Failed to get attempt info for attempt with id: " +
						std::string(reqData.get("attemptId")) + ". Reasons: ";

					for (auto el : attemptInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, attemptInfo.errors, "answer"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				TopicInfo topicInfo = dbManager.getTopicInfo(quizInfo.topicId);

				if (!topicInfo.errors.empty())
				{
					std::string log = "Failed to get topic info for topic with id: " +
						std::to_string(quizInfo.topicId) + ". Reasons: ";

					for (auto el : topicInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, topicInfo.errors, "answer"
					);

					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				std::vector<std::string> recordSet = dbManager.canUserAccessCourse(topicInfo.courseId, ctx.userId);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to answer a question with id: " + 
						std::string(reqData.get("questionId")) + ". Reason: User is unauthorised";
					recordSet[0] = "User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "answer"
					);

					res.code = crow::status::FORBIDDEN;
					res.end();
					
					return;
				}

				if (attemptInfo.currentQuestionId != questionInfo.id || attemptInfo.userId != ctx.userId)
				{
					std::string log = "Failed to answer a question with id: " + 
						std::string(reqData.get("questionId")) + ". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "answer"
					);
					
					res.code = crow::status::FORBIDDEN;
					res.end();
					
					return;
				}

				recordSet = dbManager.answerQuestion(reqData);

				// If saving to database fails
				if (recordSet.size() != 0)
				{
					std::string log = "Failed to save answer for attempt with id: " + 
						std::string(reqData.get("attemptId")) + " to the database. Reason/s: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "answer"
					);
					
					res.code = crow::status::BAD_REQUEST;
					res.end();
					
					return;
				}

				recordSet = dbManager.goToNextQuestionInAttempt(attemptInfo, quizInfo);

				if (recordSet.size() != 0)
				{
					std::string log = "Failed to update attempt with id: " + 
						std::to_string(attemptInfo.id) + " to the database. Reason/s: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, recordSet, "answer"
					);
					
					res.code = crow::status::BAD_REQUEST;
					res.end();
					
					return;
				}

				//Update attempt info to get the new id of the nexy question
				attemptInfo = dbManager.getAttemptInfo(attemptInfo.id);

				CROW_LOG_INFO << "Answer for attempt with id: " +
					std::string(reqData.get("attemptId")) + " is successfully saved into the database.";

				// Create and send request
				res = responseJSONManager.createAttemptJSONResponse(attemptInfo, "answer");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/answers/<int>")
		.methods(crow::HTTPMethod::Get)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		([&](const crow::request& req, crow::response& res, int answerId)
			{
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				CROW_LOG_INFO << "User with id: " << ctx.userId << 
					" is trying to get information for answer with id: " << answerId;

				AnswerInfo answerInfo = dbManager.getAnswersInfo(answerId);

				if (!answerInfo.errors.empty())
				{
					std::string log = "Failed to get answer info for answer with id: " +
						std::to_string(answerId) + ". Reasons: ";

					for (auto el : answerInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, answerInfo.errors, "get-answer"
					);
					
					res.code = crow::status::BAD_REQUEST;
					res.end();

					return;
				}

				AttemptInfo attemptInfo = dbManager.getAttemptInfo(answerInfo.attemptId);

				QuizInfo quizInfo = dbManager.getQuizInfo(attemptInfo.quizId);

				TopicInfo topicInfo = dbManager.getTopicInfo(quizInfo.topicId);

				std::vector<std::string> recordSet = dbManager.canUserAccessCourse(
					topicInfo.courseId, ctx.userId, true
				);

				if (recordSet[0] != "1" && ctx.userId != attemptInfo.userId)
				{
					std::string log = "Failed to get answer with id: " +
						std::to_string(answerId) + ". Reason: User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(
						false, { "User is unauthorised" }, "get-answer"
					);
					
					res.code = crow::status::FORBIDDEN;
					res.end();
					
					return;
				}

				res = responseJSONManager.createAnswerJSONResponse(answerInfo);
				res.end();
				return;
			});

	return api;
}
