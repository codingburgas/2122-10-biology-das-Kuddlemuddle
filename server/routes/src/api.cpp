#include <api.h>
#include <filesystem>

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
						std::string log = "Failed to delete user. Reason: ";

						for (auto el : recordSet)
						{
							log += el + " ";
						}

						CROW_LOG_WARNING << log;

						res = responseJSONManager.createJSONResponse(false , recordSet, "user-deletion");
						res.end();
						return;
					}

					res = responseJSONManager.createJSONResponse(true, recordSet, "user-deletion");
					res.end();
					return;
				}

				CROW_LOG_WARNING << "Failed to delete user. Reason: Requesting user isn't authorised!";
				res = crow::response(403);
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/users/@me")
		.methods(crow::HTTPMethod::Patch)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		//CROW_MIDDLEWARES(app, AuthorisationMiddleware)
		([&](const crow::request& req, crow::response& res)
			{
				auto updateData = crow::query_string("?" + req.body);
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				CROW_LOG_INFO << "Trying to update user info with id: " << ctx.userId;

				std::vector<std::string> recordSet = validationManager.isRegisterDataValid(updateData, true);
				
				// If validation falls
				if (recordSet.size() != 0)
				{
					std::string log = "Failed validation/s at user with id: " + std::to_string(ctx.userId) + " at: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "user-update");
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

					res = responseJSONManager.createJSONResponse(false, recordSet, "user-update");
					res.end();
					return;
				}

				res = responseJSONManager.createJSONResponse(true, recordSet, "user-update");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/users/@me/updateAvatarURL")
		.methods(crow::HTTPMethod::Patch)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		//CROW_MIDDLEWARES(app, AuthorisationMiddleware)
		([&](const crow::request& req, crow::response& res)
			{
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				CROW_LOG_INFO << "Trying to update user avatar with id: " << ctx.userId;

				crow::multipart::message msg(req);

				std::filesystem::path dir("static/avatars");
				if (!(std::filesystem::exists(dir)))
				{
					if (std::filesystem::create_directories(dir))
					{
						CROW_LOG_WARNING << "Avatars folder not found! New folder created successfully!";
					}
				}

				auto file = msg.parts[0].body;
				auto fileExtension = msg.parts[1].body;

				auto file_handler = std::ofstream("static/avatars/" + std::to_string(ctx.userId) + fileExtension, std::ofstream::binary);

				if (!file_handler.is_open())
				{
					CROW_LOG_ERROR << "Cannot open file for avatar";
					res = crow::response(500);
					res.end();
					return;
				}

				file_handler << file;
				file_handler.close();

				std::vector<std::string> recordSet;
				recordSet = dbManager.updateUserAvatar(ctx.userId, std::to_string(ctx.userId) + fileExtension);

				if (recordSet.size() != 0)
				{
					std::string log = "Failed to update user avatar. Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "user-avatar-update");
					res.end();
					return;
				}

				res = responseJSONManager.createJSONResponse(true, recordSet, "user-avatar-update");
				res.end();
				return;
			});

	CROW_BP_ROUTE(api, "/createNewOrg")
		.methods(crow::HTTPMethod::Post)
		.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
		//CROW_MIDDLEWARES(app, AuthorisationMiddleware)
		([&](const crow::request& req, crow::response& res)
		{
				auto reqData = crow::query_string("?" + req.body);
				auto& ctx = app.get_context<AuthorisationMiddleware>(req);

				CROW_LOG_INFO << "User with id: " << ctx.userId << " is trying to create new organisation with name: " << reqData.get("orgName");

				std::vector<std::string> recordSet = validationManager.isOrgDataValid(reqData);

				// If validation falls
				if (recordSet.size() != 0)
				{
					std::string log = "Failed validation/s at creating organisation with name: " + std::string(reqData.get("orgName")) + " at: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "organisation-register");
					res.end();
					return;
				}

				// TODO: Hash password

				recordSet = dbManager.createOrg(ctx.userId, reqData);

				// If saving to database fails
				if (recordSet.size() != 0)
				{
					std::string log = "Failed to save organisation with name: " + std::string(reqData.get("orgName")) + " to the database. Reason/s: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "organisation-register");
					res.end();
					return;
				}
				
				recordSet = dbManager.getOrgIdByName(reqData.get("orgName"));

				// If return value is bigger that 10
				// then it is not a number
				if (recordSet[0].size() > 10)
				{
					std::string log = "Failed to get organisation with name: " + std::string(reqData.get("orgName")) + " from the database. Reason/s: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "organisation-register");
					res.end();
					return;
				}

				recordSet = dbManager.addUserToOrganisation(ctx.userId, std::stoi(recordSet[0]), UserRolesInOrgs::ADMIN);

				if (recordSet.size() != 0)
				{
					std::string log = "Failed to add user with id: " + std::to_string(ctx.userId) + " to organisation with name : " + std::string(reqData.get("orgName"));

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "organisation-register");
					res.end();
					return;
				}

				CROW_LOG_INFO << "Organisation with name: " + std::string(reqData.get("orgName")) + " is successfully saved into the database.";

				// Create and send request
				res = responseJSONManager.createJSONResponse(true, recordSet, "organisation-register");
				res.end();
				return;
		});

		CROW_BP_ROUTE(api, "/joinOrg")
			.methods(crow::HTTPMethod::Post)
			.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
			//CROW_MIDDLEWARES(app, AuthorisationMiddleware)
			([&](const crow::request& req, crow::response& res)
				{
					auto reqData = crow::query_string("?" + req.body);
					auto& ctx = app.get_context<AuthorisationMiddleware>(req);

					CROW_LOG_INFO << "User with id: " << ctx.userId << " is trying to join organisation with id: " << reqData.get("orgId");

					std::vector<std::string> recordSet = validationManager.isJoinOrgDataValid(reqData);

					// If validation falls
					if (recordSet.size() != 0)
					{
						std::string log = "Failed validation/s at joining in organisation with id: " + std::string(reqData.get("orgId")) + " at: ";

						for (auto el : recordSet)
						{
							log += el + " ";
						}

						CROW_LOG_WARNING << log;

						res = responseJSONManager.createJSONResponse(false, recordSet, "join-org");
						res.end();
						return;
					}

					// TODO: Hash password

					recordSet = dbManager.doesPasswordMatchOrg(reqData.get("password"), std::stoi(reqData.get("orgId")));

					// If password is incorrect
					if (recordSet[0] == "0")
					{
						recordSet[0] = "Password is incorrect";
					}

					if (recordSet[0] != "1")
					{
						std::string log = "Failed to join user with id: " + std::to_string(ctx.userId) + " to organisation with id: " + std::string(reqData.get("orgId")) + ". Reason: ";

						for (auto el : recordSet)
						{
							log += el + " ";
						}

						CROW_LOG_WARNING << log;

						res = responseJSONManager.createJSONResponse(false, recordSet, "join-org");
						res.end();
						return;
					}

					recordSet = dbManager.addUserToOrganisation(ctx.userId, std::stoi(reqData.get("orgId")), UserRolesInOrgs::USER);

					if (recordSet.size() != 0)
					{
						std::string log = "Failed to add user with id: " + std::to_string(ctx.userId) + " to organisation with id: " + std::string(reqData.get("orgId")) + ". Reason: ";

						for (auto el : recordSet)
						{
							log += el + " ";
						}

						CROW_LOG_WARNING << log;

						res = responseJSONManager.createJSONResponse(false, recordSet, "join-org");
						res.end();
						return;
					}

					CROW_LOG_INFO << "User with id: " << ctx.userId << " is successfully added to organisation with id: " << reqData.get("orgId");

					res = responseJSONManager.createJSONResponse(true, recordSet, "join-org");
					res.end();
					return;
				});

		CROW_BP_ROUTE(api, "/updateRolesInOrg")
			.methods(crow::HTTPMethod::Post)
			.middlewares<crow::App<crow::CORSHandler, AuthorisationMiddleware>, AuthorisationMiddleware>()
			//CROW_MIDDLEWARES(app, AuthorisationMiddleware)
			([&](const crow::request& req, crow::response& res)
				{
					auto reqData = crow::query_string("?" + req.body);
					auto& ctx = app.get_context<AuthorisationMiddleware>(req);

					CROW_LOG_INFO << "User with id: " << ctx.userId << " is trying to update role on user with id: " << reqData.get("userId") << " on organisation with id: " << reqData.get("orgId");

					// Is user authorised
					// first element - error or true/false
					// second element - Role of user
					std::vector<std::string> recordSet = dbManager.isUserInOrgAndGetRole(ctx.userId, std::stoi(reqData.get("orgId")));

					if (recordSet[0] != "1")
					{
						std::string log = "Failed to update role on user with id: " + std::string(reqData.get("userId")) + " on organisation with id: " + std::string(reqData.get("orgId")) + ". Reason: ";

						for (auto el : recordSet)
						{
							log += el + " ";
						}

						CROW_LOG_WARNING << log;

						res = responseJSONManager.createJSONResponse(false, recordSet, "update-user-role-org");
						res.end();
						return;
					}
					
					// If password is incorrect
					if (recordSet[1] != "2")
					{
						recordSet[0] = "User is unauthorised";
					}

					int roleId = std::stoi(reqData.get("roleId"));

					switch (roleId)
					{
					case 1:
						recordSet = dbManager.addUserToOrganisation(std::stoi(reqData.get("userId")), std::stoi(reqData.get("orgId")), UserRolesInOrgs::TEACHER, false);
						break;
					case 2:
						recordSet = dbManager.addUserToOrganisation(std::stoi(reqData.get("userId")), std::stoi(reqData.get("orgId")), UserRolesInOrgs::ADMIN, false);
						break;
					case 0:
					default:
						recordSet = dbManager.addUserToOrganisation(std::stoi(reqData.get("userId")), std::stoi(reqData.get("orgId")), UserRolesInOrgs::USER, false);
						break;
					}

					if (recordSet.size() != 0)
					{
						std::string log = "Failed to update role on user with id: " + std::string(reqData.get("userId")) + " on organisation with id: " + std::string(reqData.get("orgId")) + ". Reason: ";

						for (auto el : recordSet)
						{
							log += el + " ";
						}

						CROW_LOG_WARNING << log;

						res = responseJSONManager.createJSONResponse(false, recordSet, "update-user-role-org");
						res.end();
						return;
					}

					CROW_LOG_INFO << "The role on user with id: " << reqData.get("userId") << " is successfully updated on organisation with id: " << reqData.get("orgId");

					res = responseJSONManager.createJSONResponse(true, recordSet, "update-user-role-org");
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
						CROW_LOG_INFO << "User with id: " << ctx.userId << " is trying to get information for all organisations.";
						auto orgInfo = dbManager.getAllOrgsInfo();

						if (!orgInfo[0].errors.empty())
						{
							std::string log = "Failed to get all organisations. Reasons: ";

							for (auto el : orgInfo[0].errors)
							{
								log += el + " ";
							}

							CROW_LOG_WARNING << log;

							res = responseJSONManager.createJSONResponse(false, orgInfo[0].errors, "get-organisation");
							res.end();
							return;
						}

						res = responseJSONManager.createOrgsJSONResponse(orgInfo);
						res.end();
						return;
					}

					CROW_LOG_INFO << "User with id: " << ctx.userId << " is trying to get information for organisation: " << orgName;
					
					// Try to get organisation's id
					std::vector<std::string> recordSet = dbManager.getOrgIdByName(orgName);

					// If return value is bigger that 10
					// then it is not a number
					if (recordSet[0].size() > 10)
					{
						std::string log = "Failed to get organisation's id with name: " + orgName + " from the database. Reason/s: ";

						for (auto el : recordSet)
						{
							log += el + " ";
						}

						CROW_LOG_WARNING << log;

						res = responseJSONManager.createJSONResponse(false, recordSet, "get-organisation");
						res.end();
						return;
					}

					 recordSet = dbManager.isUserInOrgAndGetRole(ctx.userId, std::stoi(recordSet[0]));

					// Error happend
					if (recordSet[0] != "1")
					{
						std::string log = "Failed to get organisation info with name: " + orgName + ". Reason: User is unauthorised";
						recordSet[0] = "User is unauthorised";
						
						CROW_LOG_WARNING << log;

						res = responseJSONManager.createJSONResponse(false, recordSet, "get-organisation");
						res.code = 403;
						res.end();
						return;
					}

					OrgInfo orgInfo = dbManager.getOrgInfo(orgName);

					if (!orgInfo.errors.empty())
					{
						std::string log = "Failed to get organisation info with name: " + orgName + ". Reasons: ";
						
						for (auto el : orgInfo.errors)
						{
							log += el + " ";
						}

						CROW_LOG_WARNING << log;

						res = responseJSONManager.createJSONResponse(false, recordSet, "get-organisation");
						res.end();
						return;
					}

					orgInfo.courses = dbManager.getAllCoursesInOrgWithID(orgInfo.id);

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

					CROW_LOG_INFO << "User with id: " << ctx.userId << " is trying to delete organisation with name: " << orgName;

					// Try to get organisation's id
					std::vector<std::string> recordSet = dbManager.getOrgIdByName(orgName);

					// If return value's size is bigger that 10
					// then it is not a number
					if (recordSet[0].size() > 10)
					{
						std::string log = "Failed to get organisation's id with name: " + orgName + " from the database. Reason/s: ";

						for (auto el : recordSet)
						{
							log += el + " ";
						}

						CROW_LOG_WARNING << log;

						res = responseJSONManager.createJSONResponse(false, recordSet, "organisation-deletion");
						res.end();
						return;
					}

					recordSet = dbManager.isUserInOrgAndGetRole(ctx.userId, std::stoi(recordSet[0]));

					// Error happend
					if (recordSet[0] != "1" || recordSet[1] != "2")
					{
						std::string log = "Failed to delete organisation with name: " + orgName + ". Reason: User is unauthorised";
						recordSet[0] = "User is unauthorised";

						CROW_LOG_WARNING << log;

						res = responseJSONManager.createJSONResponse(false, recordSet, "organisation-deletion");
						res.code = 403;
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

						res = responseJSONManager.createJSONResponse(false, recordSet, "organisation-deletion");
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

					CROW_LOG_INFO << "User with id: " << ctx.userId << " is trying to update information for organisation with name: " << orgName;

					std::vector<std::string> recordSet = validationManager.isOrgDataValid(updateData, true);

					// If validation falls
					if (recordSet.size() != 0)
					{
						std::string log = "Failed validation/s at organisation with name: " + orgName + " at: ";

						for (auto el : recordSet)
						{
							log += el + " ";
						}

						CROW_LOG_WARNING << log;

						res = responseJSONManager.createJSONResponse(false, recordSet, "organisation-update");
						res.end();
						return;
					}

					// Try to get organisation's id
					recordSet = dbManager.getOrgIdByName(orgName);

					// If return value is bigger that 10
					// then it is not a number
					if (recordSet[0].size() > 10)
					{
						std::string log = "Failed to get organisation's id with name: " + orgName + " from the database. Reason/s: ";

						for (auto el : recordSet)
						{
							log += el + " ";
						}

						CROW_LOG_WARNING << log;

						res = responseJSONManager.createJSONResponse(false, recordSet, "organisation-update");
						res.end();
						return;
					}

					recordSet = dbManager.isUserInOrgAndGetRole(ctx.userId, std::stoi(recordSet[0]));

					// Error happend
					if (recordSet[0] != "1" || recordSet[1] != "2")
					{
						std::string log = "Failed to update organisation info with name: " + orgName + ". Reason: User is unauthorised";
						recordSet[0] = "User is unauthorised";

						CROW_LOG_WARNING << log;

						res = responseJSONManager.createJSONResponse(false, recordSet, "organisation-update");
						res.code = 403;
						res.end();
						return;
					}

					recordSet = dbManager.updateOrg(std::stoi(dbManager.getOrgIdByName(orgName)[0]), updateData);

					if (recordSet.size() != 0)
					{
						std::string log = "Failed to update organisation. Reason: ";

						for (auto el : recordSet)
						{
							log += el + " ";
						}

						CROW_LOG_WARNING << log;

						res = responseJSONManager.createJSONResponse(false, recordSet, "organisation-update");
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

					CROW_LOG_INFO << "User with id: " << ctx.userId << " is trying to create new course with name: " << reqData.get("courseName") << " in organisation with id: " << reqData.get("orgId");

					std::vector<std::string> recordSet = dbManager.isUserInOrgAndGetRole(ctx.userId, std::stoi(reqData.get("orgId")));

					// Error happend
					if (recordSet[0] != "1" || recordSet[1] != "2")
					{
						std::string log = "Failed to create course with name: " + std::string(reqData.get("courseName")) + ". Reason: User is unauthorised";
						recordSet[0] = "User is unauthorised";

						CROW_LOG_WARNING << log;

						res = responseJSONManager.createJSONResponse(false, recordSet, "organisation-deletion");
						res.code = 403;
						res.end();
						return;
					}

					recordSet = validationManager.isCourseDataValid(reqData);

					// If validation falls
					if (recordSet.size() != 0)
					{
						std::string log = "Failed validation/s at creating course with name: " + std::string(reqData.get("courseName")) + " at: ";

						for (auto el : recordSet)
						{
							log += el + " ";
						}

						CROW_LOG_WARNING << log;

						res = responseJSONManager.createJSONResponse(false, recordSet, "course-register");
						res.end();
						return;
					}

					// TODO: Hash password

					recordSet = dbManager.createCourse(reqData);

					// If saving to database fails
					if (recordSet.size() != 0)
					{
						std::string log = "Failed to save course with name: " + std::string(reqData.get("courseName")) + " to the database. Reason/s: ";

						for (auto el : recordSet)
						{
							log += el + " ";
						}

						CROW_LOG_WARNING << log;

						res = responseJSONManager.createJSONResponse(false, recordSet, "course-register");
						res.end();
						return;
					}

					CROW_LOG_INFO << "Course with name: " + std::string(reqData.get("courseName")) + " is successfully saved into the database.";

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

					CROW_LOG_INFO << "User with id: " << ctx.userId << " is trying to join course with id: " << reqData.get("courseId");

					CourseInfo courseInfo = dbManager.getCourseInfo(std::stoi(reqData.get("courseId")));
								
					if (!courseInfo.errors.empty())
					{
						std::string log = "Failed to get course info for course with name: " + std::string(reqData.get("courseId")) + ". Reasons: ";

						for (auto el : courseInfo.errors)
						{
							log += el + " ";
						}

						CROW_LOG_WARNING << log;

						res = responseJSONManager.createJSONResponse(false, courseInfo.errors, "join-course");
						res.end();
						return;
					}

					std::vector<std::string> recordSet = dbManager.isUserInOrgAndGetRole(ctx.userId, courseInfo.orgId);

					// Error happend
					if (recordSet[0] != "1")
					{
						std::string log = "Failed to join in course with id: " + std::string(reqData.get("courseId")) + ". Reason: User is unauthorised";
						recordSet[0] = "User is unauthorised";

						CROW_LOG_WARNING << log;

						res = responseJSONManager.createJSONResponse(false, recordSet, "join-course");
						res.code = 403;
						res.end();
						return;
					}

					recordSet = validationManager.isJoinOrgDataValid(reqData);

					// If validation falls
					if (recordSet.size() != 0)
					{
						std::string log = "Failed validation/s at joining in course with id: " + std::string(reqData.get("courseId")) + " at: ";

						for (auto el : recordSet)
						{
							log += el + " ";
						}

						CROW_LOG_WARNING << log;

						res = responseJSONManager.createJSONResponse(false, recordSet, "join-course");
						res.end();
						return;
					}

					// TODO: Hash password

					recordSet = dbManager.doesPasswordMatchCourse(reqData.get("password"), std::stoi(reqData.get("courseId")));

					// If password is incorrect
					if (recordSet[0] == "0")
					{
						recordSet[0] = "Password is incorrect";
					}

					if (recordSet[0] != "1")
					{
						std::string log = "Failed to join user with id: " + std::to_string(ctx.userId) + " to course with id: " + std::string(reqData.get("courseId")) + ". Reason: ";

						for (auto el : recordSet)
						{
							log += el + " ";
						}

						CROW_LOG_WARNING << log;

						res = responseJSONManager.createJSONResponse(false, recordSet, "join-course");
						res.end();
						return;
					}

					recordSet = dbManager.addUserToCourse(ctx.userId, std::stoi(reqData.get("courseId")), UserRolesInOrgs::USER);

					if (recordSet.size() != 0)
					{
						std::string log = "Failed to add user with id: " + std::to_string(ctx.userId) + " to course with id: " + std::string(reqData.get("courseId")) + ". Reason: ";

						for (auto el : recordSet)
						{
							log += el + " ";
						}

						CROW_LOG_WARNING << log;

						res = responseJSONManager.createJSONResponse(false, recordSet, "join-course");
						res.end();
						return;
					}

					CROW_LOG_INFO << "User with id: " << ctx.userId << " is successfully added to course with id: " << reqData.get("courseId");

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

				CROW_LOG_INFO << "User with id: " << ctx.userId << " is trying to add teacher with id: " << reqData.get("teacherId") << " to course with id: " << reqData.get("courseId");

				CourseInfo courseInfo = dbManager.getCourseInfo(std::stoi(reqData.get("courseId")));

				if (!courseInfo.errors.empty())
				{
					std::string log = "Failed to get course info for course with name: " + std::string(reqData.get("courseId")) + ". Reasons: ";

					for (auto el : courseInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, courseInfo.errors, "add-techer");
					res.end();
					return;
				}

				std::vector<std::string> recordSet = dbManager.isUserInOrgAndGetRole(ctx.userId, courseInfo.orgId);

				// User is not admin
				if (recordSet[0] != "1" || recordSet[1] != "2")
				{
					std::string log = "Failed to add teacher with id: " + std::string(reqData.get("teacherId")) + " to course with id : " + std::string(reqData.get("courseId")) + ". Reason : User is unauthorised";
					recordSet[0] = "User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "add-techer");
					res.code = 403;
					res.end();
					return;
				}

				recordSet = dbManager.isUserInOrgAndGetRole(std::stoi(reqData.get("teacherId")), courseInfo.orgId);

				// User is not admin
				if (recordSet[0] != "1" || recordSet[1] != "1")
				{
					std::string log = "Failed to add teacher with id: " + std::string(reqData.get("teacherId")) + " to course with id: " + std::string(reqData.get("courseId")) + ". Reason : User is unauthorised";
					recordSet[0] = "The user is not a teacher in this org";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "add-techer");
					res.end();
					return;
				}

				recordSet = dbManager.addUserToCourse(std::stoi(reqData.get("teacherId")), std::stoi(reqData.get("courseId")), UserRolesInOrgs::TEACHER);

				if (recordSet.size() != 0)
				{
					std::string log = "Failed to add user with id: " + std::to_string(ctx.userId) + " to course with id: " + std::string(reqData.get("courseId")) + ". Reason: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "add-techer");
					res.end();
					return;
				}

				CROW_LOG_INFO << "Teacger with id: " << reqData.get("teacherId") << " is successfully added to course with id: " << reqData.get("courseId");

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

				CROW_LOG_INFO << "User with id: " << ctx.userId << " is trying to get information for course: " << courseId;
				
				CourseInfo courseInfo = dbManager.getCourseInfo(courseId);

				if (!courseInfo.errors.empty())
				{
					std::string log = "Failed to get course info for course with name: " + std::to_string(courseId) + ". Reasons: ";

					for (auto el : courseInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, courseInfo.errors, "get-course");
					res.end();
					return;
				}

				std::vector<std::string> recordSet =  dbManager.isUserInOrgAndGetRole(ctx.userId, courseInfo.orgId);

				// Error happend
				if (recordSet[0] != "1")
				{
					std::string log = "Failed to get course info with id: " + std::to_string(courseId) + ". Reason: User is unauthorised";
					recordSet[0] = "User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "get-course");
					res.code = 403;
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
					std::string log = "Failed to get course info with id: " + std::to_string(courseId) + ". Reason: User is unauthorised";
					recordSet[0] = "User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "get-course");
					res.code = 403;
					res.end();
					return;
				}

				courseInfo.topics = dbManager.getAllTopicsInCourseWithID(courseId);

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

				CROW_LOG_INFO << "User with id: " << ctx.userId << " is trying to delete course with id: " << courseId;

				CourseInfo courseInfo = dbManager.getCourseInfo(courseId);

				if (!courseInfo.errors.empty())
				{
					std::string log = "Failed to get course info for course with id: " + std::to_string(courseId) + ". Reasons: ";

					for (auto el : courseInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, courseInfo.errors, "course-deletion");
					res.end();
					return;
				}

				std::vector<std::string> recordSet = dbManager.isUserInOrgAndGetRole(ctx.userId, courseInfo.orgId);

				// Error happend
				if (recordSet[0] != "1" || recordSet[1] != "2")
				{
					std::string log = "Failed to delete course with id: " + std::to_string(courseId) + ". Reason: User is unauthorised";
					recordSet[0] = "User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "course-deletion");
					res.code = 403;
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

					res = responseJSONManager.createJSONResponse(false, recordSet, "course-deletion");
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

				CROW_LOG_INFO << "User with id: " << ctx.userId << " is trying to update information for course with id: " << courseId;

				std::vector<std::string> recordSet = validationManager.isCourseDataValid(updateData, true);

				// If validation falls
				if (recordSet.size() != 0)
				{
					std::string log = "Failed validation/s at course with id: " + std::to_string(courseId) + " at: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "course-update");
					res.end();
					return;
				}

				recordSet = dbManager.canUserAccessCourse(courseId, ctx.userId, true);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to update course info with id: " + std::to_string(courseId) + ". Reason: User is unauthorised";
					recordSet[0] = "User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "course-update");
					res.code = 403;
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

					res = responseJSONManager.createJSONResponse(false, recordSet, "course-update");
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

				CROW_LOG_INFO << "User with id: " << ctx.userId << " is trying to create new topic with name: " << reqData.get("topicName") << " in course with id: " << reqData.get("courseId");

				std::vector<std::string> recordSet = dbManager.canUserAccessCourse(std::stoi(reqData.get("courseId")), ctx.userId, true);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to create new topic with name: " + std::string(reqData.get("topicName")) + ". Reason: User is unauthorised";
					recordSet[0] = "User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "topic-register");
					res.code = 403;
					res.end();
					return;
				}

				// TODO: Hash password

				recordSet = dbManager.createTopic(reqData);

				// If saving to database fails
				if (recordSet.size() != 0)
				{
					std::string log = "Failed to save topic with name: " + std::string(reqData.get("topicName")) + " to the database. Reason/s: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "topic-register");
					res.end();
					return;
				}

				CROW_LOG_INFO << "Topic with name: " + std::string(reqData.get("topicName")) + " is successfully saved into the database.";

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

				CROW_LOG_INFO << "User with id: " << ctx.userId << " is trying to get information for topic: " << topicId;

				TopicInfo topicInfo = dbManager.getTopicInfo(topicId);

				if (!topicInfo.errors.empty())
				{
					std::string log = "Failed to get topic info for topic with id: " + std::to_string(topicId) + ". Reasons: ";

					for (auto el : topicInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, topicInfo.errors, "get-topic");
					res.end();
					return;
				}

				std::vector<std::string> recordSet = dbManager.canUserAccessCourse(topicInfo.courseId, ctx.userId);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to get topic with id: " + std::to_string(topicId) + ". Reason: User is unauthorised";
					recordSet[0] = "User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "get-topic");
					res.code = 403;
					res.end();
					return;
				}

				topicInfo.lessons = dbManager.getAllLessonInTopicWithID(topicId);

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

				CROW_LOG_INFO << "User with id: " << ctx.userId << " is trying to delete topic with id: " << topicId;

				TopicInfo topicInfo = dbManager.getTopicInfo(topicId);

				if (!topicInfo.errors.empty())
				{
					std::string log = "Failed to delete topic with id: " + std::to_string(topicId) + ". Reasons: ";

					for (auto el : topicInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, topicInfo.errors, "topic-deletion");
					res.end();
					return;
				}

				std::vector<std::string> recordSet = dbManager.canUserAccessCourse(topicInfo.courseId, ctx.userId, true);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to delete topic with id: " + std::to_string(topicId) + ". Reason: User is unauthorised";
					recordSet[0] = "User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "topic-deletion");
					res.code = 403;
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

					res = responseJSONManager.createJSONResponse(false, recordSet, "topic-deletion");
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

				CROW_LOG_INFO << "User with id: " << ctx.userId << " is trying to update information for topic with id: " << topicId;

				TopicInfo topicInfo = dbManager.getTopicInfo(topicId);

				if (!topicInfo.errors.empty())
				{
					std::string log = "Failed to update topic with id: " + std::to_string(topicId) + ". Reasons: ";

					for (auto el : topicInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, topicInfo.errors, "topic-update");
					res.end();
					return;
				}

				std::vector<std::string> recordSet = dbManager.canUserAccessCourse(topicInfo.courseId, ctx.userId, true);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to update topic with id: " + std::to_string(topicId) + ". Reason: User is unauthorised";
					recordSet[0] = "User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "topic-update");
					res.code = 403;
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

					res = responseJSONManager.createJSONResponse(false, recordSet, "topic-update");
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

				CROW_LOG_INFO << "User with id: " << ctx.userId << " is trying to create new lesson with name: " << reqData.get("lessonName") << " in topic with id: " << reqData.get("topicId");

				TopicInfo topicInfo = dbManager.getTopicInfo(std::stoi(reqData.get("topicId")));

				if (!topicInfo.errors.empty())
				{
					std::string log = "Failed to get topic info for topic with id: " + std::string(reqData.get("topicId")) + ". Reasons: ";

					for (auto el : topicInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, topicInfo.errors, "lesson-register");
					res.end();
					return;
				}

				std::vector<std::string> recordSet = dbManager.canUserAccessCourse(std::stoi(reqData.get("topicId")), ctx.userId, true);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to create new lesson with name: " + std::string(reqData.get("lessonName")) + ". Reason: User is unauthorised";
					recordSet[0] = "User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "lesson-register");
					res.code = 403;
					res.end();
					return;
				}

				recordSet = dbManager.createLesson(reqData);

				// If saving to database fails
				if (recordSet.size() != 0)
				{
					std::string log = "Failed to save lesson with name: " + std::string(reqData.get("lessonName")) + " to the database. Reason/s: ";

					for (auto el : recordSet)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "lesson-register");
					res.end();
					return;
				}

				CROW_LOG_INFO << "Lesson with name: " + std::string(reqData.get("lessonName")) + " is successfully saved into the database.";

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

				CROW_LOG_INFO << "User with id: " << ctx.userId << " is trying to get information for lesson with id: " << lessonId;

				LessonInfo lessonInfo = dbManager.getLessonInfo(lessonId);

				if (!lessonInfo.errors.empty())
				{
					std::string log = "Failed to get lesson info for topic with id: " + std::to_string(lessonId) + ". Reasons: ";

					for (auto el : lessonInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, lessonInfo.errors, "get-lesson");
					res.end();
					return;
				}

				TopicInfo topicInfo = dbManager.getTopicInfo(lessonInfo.topicId);

				std::vector<std::string> recordSet = dbManager.canUserAccessCourse(topicInfo.courseId, ctx.userId);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to get lesson with id: " + std::to_string(lessonId) + ". Reason: User is unauthorised";
					recordSet[0] = "User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "get-lesson");
					res.code = 403;
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

				CROW_LOG_INFO << "User with id: " << ctx.userId << " is trying to delete lesson with id: " << lessonId;

				LessonInfo lessonInfo = dbManager.getLessonInfo(lessonId);

				if (!lessonInfo.errors.empty())
				{
					std::string log = "Failed to delete lesson with id: " + std::to_string(lessonId) + ". Reasons: ";

					for (auto el : lessonInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, lessonInfo.errors, "lesson-deletion");
					res.end();
					return;
				}

				TopicInfo topicInfo = dbManager.getTopicInfo(lessonInfo.topicId);

				std::vector<std::string> recordSet = dbManager.canUserAccessCourse(topicInfo.courseId, ctx.userId, true);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to delete lesson with id: " + std::to_string(lessonId) + ". Reason: User is unauthorised";
					recordSet[0] = "User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "lesson-deletion");
					res.code = 403;
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

				CROW_LOG_INFO << "User with id: " << ctx.userId << " is trying to update information for lesson with id: " << lessonId;

				LessonInfo lessonInfo = dbManager.getLessonInfo(lessonId);

				if (!lessonInfo.errors.empty())
				{
					std::string log = "Failed to update lesson with id: " + std::to_string(lessonId) + ". Reasons: ";

					for (auto el : lessonInfo.errors)
					{
						log += el + " ";
					}

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, lessonInfo.errors, "leson-update");
					res.end();
					return;
				}

				TopicInfo topicInfo = dbManager.getTopicInfo(lessonInfo.topicId);

				std::vector<std::string> recordSet = dbManager.canUserAccessCourse(topicInfo.courseId, ctx.userId, true);

				if (recordSet[0] != "1")
				{
					std::string log = "Failed to update lesson with id: " + std::to_string(lessonId) + ". Reason: User is unauthorised";
					recordSet[0] = "User is unauthorised";

					CROW_LOG_WARNING << log;

					res = responseJSONManager.createJSONResponse(false, recordSet, "lesson-update");
					res.code = 403;
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

					res = responseJSONManager.createJSONResponse(false, recordSet, "lesson-update");
					res.end();
					return;
				}

				res = responseJSONManager.createJSONResponse(true, recordSet, "lesson-update");
				res.end();
				return;
			});

	return api;
}
