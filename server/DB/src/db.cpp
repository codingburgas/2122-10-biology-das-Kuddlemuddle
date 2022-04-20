#include <db.h>

std::vector<std::string> DBManager::registerUser(crow::query_string data)
{
	std::vector<std::string> recordSet;

	nlohmann::json userJSON;

	// Get the JSON from the file
	try
	{
		userJSON = getJSONFromFile("users.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open user.json file");
		return recordSet;
	}

	// Check for duplicate email
	if (getUserByField(userJSON, "Email", data.get("email")))
	{
		recordSet.push_back("There is already a user with this email: " + std::string(data.get("email")));
		return recordSet;
	}

	// Check for duplicate username
	if (getUserByField(userJSON, "Username", data.get("username")))
	{
		recordSet.push_back("There is already a user with this username: " + std::string(data.get("username")));
		return recordSet;
	}

	// Check if json is []
	if (userJSON.is_array())
	{
		if (userJSON.empty())
		{
			userJSON = nullptr;
		}
	}

	// Add the user to the JSON
	userJSON.push_back(
		{
		{ "ID", getLastId(userJSON) + 1},
		{ "FirstName", data.get("firstName") },
		{ "LastName", data.get("lastName") },
		{ "Username", data.get("username") },
		{ "Email", data.get("email") },
		{ "Password", data.get("password") },
		// TODO: Add salt
		{ "RoleID", 0 },
		{ "AvatarURL", std::string("https://avatars.dicebear.com/api/identicon/" + std::to_string(time(0)) + ".svg") }
		}
	);

	// Save the json to the file
	if (!setJSONFile(userJSON, "users.json"))
	{
		recordSet.push_back("Could'n open user.json file");
		return recordSet;
	}

	return recordSet;
}

std::vector<std::string> DBManager::loginUser(crow::query_string data)
{
	EnvManager envManager;
	std::vector<std::string> recordSet;
	nlohmann::json userJSON;

	// Get the JSON from the file
	try
	{
		userJSON = getJSONFromFile("users.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open user.json file");
		return recordSet;
	}

	// Check if user is trying to log in with email or username
	if (std::string(data.get("loginCredential")).find("@") != std::string::npos)
	{
		for (auto it = userJSON.begin(); it != userJSON.end(); ++it)
		{
			// Authtenticate user
			if (it.value()["Email"] == data.get("loginCredential") && it.value()["Password"] == data.get("password"))
			{
				// Authorise user
				auto token = jwt::create<jwt::traits::nlohmann_json>()
					.set_type("JWS")
					.set_subject(std::to_string(int(it.value()["ID"])))
					.set_issued_at(std::chrono::system_clock::now())
					.set_expires_at(std::chrono::system_clock::now() + std::chrono::hours(24))
					.set_payload_claim("isAdmin", int(it.value()["RoleID"]))
					.sign(jwt::algorithm::hs256(envManager.getEnv("JWT_SECRET")));

				recordSet.push_back("Bearer " + token);
				return recordSet;
			}
		}

		// Login failure
		recordSet.push_back("There is not a user with this email: " + std::string(data.get("loginCredential")) + " or password is invalid.");
		return recordSet;
	}
	else
	{
		for (auto it = userJSON.begin(); it != userJSON.end(); ++it)
		{
			// Authtenticate user
			if (toLowerCase(it.value()["Username"]) == toLowerCase(data.get("loginCredential")) && it.value()["Password"] == data.get("password"))
			{
				// Authorise user
				auto token = jwt::create<jwt::traits::nlohmann_json>()
					.set_type("JWS")
					.set_subject(std::to_string(int(it.value()["ID"])))
					.set_issued_at(std::chrono::system_clock::now())
					.set_expires_at(std::chrono::system_clock::now() + std::chrono::hours(24))
					.set_payload_claim("isAdmin", int(it.value()["RoleID"]))
					.sign(jwt::algorithm::hs256(envManager.getEnv("JWT_SECRET")));

				recordSet.push_back("Bearer " + token);
				return recordSet;
			}
		}

		// Login failure
		recordSet.push_back("There is not a user with this username: " + std::string(data.get("loginCredential")) + " or password is invalid.");
		return recordSet;
	}

	return recordSet;
}

std::vector<std::string> DBManager::getUserInfo(std::string username, int userId)
{
	std::vector<std::string> recordSet;
	nlohmann::json userJSON;

	// Get the JSON from the file
	try
	{
		userJSON = getJSONFromFile("users.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open user.json file");
		return recordSet;
	}

	// Check if user is trying to access his account
	if (username == "@me")
	{
		for (auto it = userJSON.begin(); it != userJSON.end(); ++it)
		{
			if (it.value()["ID"] == userId)
			{
				return {
					it.value()["FirstName"],
					it.value()["LastName"],
					it.value()["Username"],
					it.value()["Email"],
					std::to_string(int(it.value()["RoleID"])),
					it.value()["AvatarURL"]
				};
			}
		}

		// If the execution goes here, there should be smt very wrong
		recordSet.push_back("Could not find user with id: " + std::to_string(userId));
		return recordSet;
	}

	for (auto it = userJSON.begin(); it != userJSON.end(); ++it)
	{
		// Authtenticate user
		if (toLowerCase(it.value()["Username"]) == toLowerCase(username))
		{
			return {
					it.value()["FirstName"],
					it.value()["LastName"],
					it.value()["Username"],
					it.value()["Email"],
					std::to_string(int(it.value()["RoleID"])),
					it.value()["AvatarURL"]
			};
		}
	}

	recordSet.push_back("Could not find user with username: " + username);
	return recordSet;
}

std::vector<std::string> DBManager::deleteUser(std::string username, int userId)
{
	std::vector<std::string> recordSet;
	nlohmann::json userJSON;

	// Get the JSON from the file
	try
	{
		userJSON = getJSONFromFile("users.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open user.json file");
		return recordSet;
	}

	// Check if user is trying to access his account
	if (username == "@me")
	{
		for (auto it = userJSON.begin(); it != userJSON.end(); ++it)
		{
			if (it.value()["ID"] == userId)
			{
				userJSON.erase(it);
				if (!setJSONFile(userJSON, "users.json"))
				{
					recordSet.push_back("Could'n open user.json file");
				}
				return recordSet;
			}
		}

		// If the execution goes here, there should be smt very wrong
		recordSet.push_back("Could not find user with id: " + std::to_string(userId));
		return recordSet;
	}

	for (auto it = userJSON.begin(); it != userJSON.end(); ++it)
	{
		// Authtenticate user
		if (toLowerCase(it.value()["Username"]) == toLowerCase(username))
		{
			userJSON.erase(it);
			if (!setJSONFile(userJSON, "users.json"))
			{
				recordSet.push_back("Could'n open user.json file");
			}
			return recordSet;
		}
	}

	recordSet.push_back("Could not find user with username: " + username);
	return recordSet;
}

std::vector<std::string> DBManager::updateUser(int userId, crow::query_string data)
{
	std::vector<std::string> recordSet;
	nlohmann::json userJSON;

	// Get the JSON from the file
	try
	{
		userJSON = getJSONFromFile("users.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open user.json file");
		return recordSet;
	}

	for (auto it = userJSON.begin(); it != userJSON.end(); ++it)
	{
		if (it.value()["ID"] == userId)
		{
			it.value()["FirstName"] = std::string(data.get("firstName")).empty() ? it.value()["FirstName"] : data.get("firstName");
			it.value()["LastName"] = std::string(data.get("lastName")).empty() ? it.value()["LastName"] : data.get("lastName");
			it.value()["Username"] = std::string(data.get("username")).empty() ? it.value()["Username"] : data.get("username");
			it.value()["Email"] = std::string(data.get("email")).empty() ? it.value()["Email"] : data.get("email");
			it.value()["Password"] = std::string(data.get("password")).empty() ? it.value()["Password"] : data.get("password");

			if (!setJSONFile(userJSON, "users.json"))
			{
				recordSet.push_back("Could'n open user.json file");
			}

			return recordSet;
		}
	}

	// If the execution goes here, there should be smt very wrong
	recordSet.push_back("Could not find user with id: " + std::to_string(userId));
	return recordSet;
}

std::vector<std::string> DBManager::updateUserAvatar(int userId, std::string imageName)
{
	std::vector<std::string> recordSet;
	nlohmann::json userJSON;
	EnvManager envManager;

	// Get the JSON from the file
	try
	{
		userJSON = getJSONFromFile("users.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open user.json file");
		return recordSet;
	}

	for (auto it = userJSON.begin(); it != userJSON.end(); ++it)
	{
		if (it.value()["ID"] == userId)
		{
			it.value()["AvatarURL"] = envManager.getEnv("SERVER_URL") + "static/avatars/" + imageName;

			if (!setJSONFile(userJSON, "users.json"))
			{
				recordSet.push_back("Could'n open user.json file");
			}
			return recordSet;
		}
	}

	// If the execution goes here, there should be smt very wrong
	recordSet.push_back("Could not find user with id: " + std::to_string(userId));
	return recordSet;
}

std::vector<std::string> DBManager::createOrg(int userId, crow::query_string data)
{
	std::vector<std::string> recordSet;

	nlohmann::json orgsJSON;

	// Get the JSON from the file
	try
	{
		orgsJSON = getJSONFromFile("orgs.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open orgs.json file");
		return recordSet;
	}

	// Check for duplicate name
	if (getUserByField(orgsJSON, "Name", data.get("orgName")))
	{
		recordSet.push_back("There is already a organisation with this name: " + std::string(data.get("orgName")));
		return recordSet;
	}

	// Check if json is []
	if (orgsJSON.is_array())
	{
		if (orgsJSON.empty())
		{
			orgsJSON = nullptr;
		}
	}

	// Add the user to the JSON
	orgsJSON.push_back(
		{
		{ "ID", getLastId(orgsJSON) + 1},
		{ "Name", data.get("orgName") },
		{ "Password", data.get("password") }
		// TODO: Add Salt
		}
	);

	// Save the json to the file
	if (!setJSONFile(orgsJSON, "orgs.json"))
	{
		recordSet.push_back("Could'n open orgs.json file");
		return recordSet;
	}

	return recordSet;
}

std::vector<std::string> DBManager::addUserToOrganisation(int userId, int orgId, UserRolesInOrgs userRolesInOrgs, bool createNewEntry)
{
	std::vector<std::string> recordSet;

	nlohmann::json userOrgRoleJSON;

	// Get the JSON from the file
	try
	{
		userOrgRoleJSON = getJSONFromFile("userOrgRole.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open userOrgRole.json file");
		return recordSet;
	}

	// Check if user is already in organisation
	for (auto it = userOrgRoleJSON.begin(); it != userOrgRoleJSON.end(); ++it)
	{
		if (it.value()["UserID"] == userId && it.value()["OrganisationID"] == orgId)
		{
			it.value()["Role"] = userRolesInOrgs;

			if (!setJSONFile(userOrgRoleJSON, "userOrgRole.json"))
			{
				recordSet.push_back("Could'n open userOrgRole.json file");
			}

			return recordSet;
		}
	}

	if (!createNewEntry)
	{
		recordSet.push_back("Couldn't find user with id: " + std::to_string(userId) + " or organisation with id: " + std::to_string(orgId));
		return recordSet;
	}

	// Check if json is []
	if (userOrgRoleJSON.is_array())
	{
		if (userOrgRoleJSON.empty())
		{
			userOrgRoleJSON = nullptr;
		}
	}

	// Add the user to the JSON
	userOrgRoleJSON.push_back(
		{
		{ "UserID", userId },
		{ "OrganisationID", orgId},
		{ "Role", userRolesInOrgs}
		}
	);

	// Save the json to the file
	if (!setJSONFile(userOrgRoleJSON, "userOrgRole.json"))
	{
		recordSet.push_back("Could'n open userOrgRole.json file");
		return recordSet;
	}

	return recordSet;
}

std::vector<std::string> DBManager::getOrgIdByName(std::string orgName)
{
	std::vector<std::string> recordSet;

	nlohmann::json orgsJSON;

	// Get the JSON from the file
	try
	{
		orgsJSON = getJSONFromFile("orgs.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open orgs.json file");
		return recordSet;
	}

	for (auto it = orgsJSON.begin(); it != orgsJSON.end(); ++it)
	{
		if (it.value()["Name"] == orgName)
		{
			recordSet.push_back(it.value()["ID"].dump());
			return recordSet;
		}
	}

	recordSet.push_back("Could not find organisation with name: " + orgName);
	return recordSet;
}

std::vector<std::string> DBManager::doesPasswordMatchOrg(std::string password, int orgId)
{
	std::vector<std::string> recordSet;

	nlohmann::json orgsJSON;

	// Get the JSON from the file
	try
	{
		orgsJSON = getJSONFromFile("orgs.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open orgs.json file");
		return recordSet;
	}

	for (auto it = orgsJSON.begin(); it != orgsJSON.end(); ++it)
	{
		if (it.value()["ID"] == orgId)
		{
			recordSet.push_back(std::to_string(it.value()["Password"] == password));
			return recordSet;
		}
	}

	recordSet.push_back("Could not find organisation with id: " + orgId);
	return recordSet;
}

std::vector<std::string> DBManager::isUserAdminInOrg(int userId, int orgId)
{
	std::vector<std::string> recordSet;

	nlohmann::json userOrgRoleJSON;

	// Get the JSON from the file
	try
	{
		userOrgRoleJSON = getJSONFromFile("userOrgRole.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open userOrgRole.json file");
		return recordSet;
	}

	for (auto it = userOrgRoleJSON.begin(); it != userOrgRoleJSON.end(); ++it)
	{
		if (it.value()["UserID"] == userId && it.value()["OrganisationID"] == orgId)
		{
			recordSet.push_back(std::to_string(it.value()["Role"] == 2));
			return recordSet;
		}
	}

	recordSet.push_back("Could not find organisation with id: " + std::to_string(orgId));
	return recordSet;
}

nlohmann::json DBManager::getJSONFromFile(std::string filename)
{
	std::ifstream JSONFile(filename, std::fstream::app);

	if (!JSONFile.is_open())
	{
		throw std::string("Can not open file" + filename);
	}

	nlohmann::json userJSON;

	try
	{
		userJSON = nlohmann::json::parse(JSONFile);
	}
	catch (nlohmann::json::parse_error& ex)
	{
		CROW_LOG_WARNING << filename << " file is empty.";
	}

	JSONFile.close();
	return userJSON;
}

bool DBManager::setJSONFile(nlohmann::json json, std::string filename)
{
	std::ofstream JSONFile(filename, std::ios::trunc);

	if (!JSONFile.is_open())
	{
		return false;
	}

	JSONFile << json.dump();

	JSONFile.close();
	return true;
}

int DBManager::getLastId(nlohmann::json json)
{
	try
	{
		return json.rbegin().value()["ID"];
	}
	catch (nlohmann::json::invalid_iterator& ex)
	{
		return 0;
	}
}

bool DBManager::getUserByField(nlohmann::json json, std::string field, std::string fieldData)
{
	for (auto it = json.begin(); it != json.end(); ++it)
	{
		if (field == "Username")
		{
			if (toLowerCase(it.value()[field]) == toLowerCase(fieldData))
			{
				return true;
			}
		}
		else
		{
			if (it.value()[field] == fieldData)
			{
				return true;
			}
		}
	}

	return false;
}