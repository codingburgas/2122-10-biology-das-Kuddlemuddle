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
		recordSet.push_back("There is not a user with this email: " + std::string(data.get("loginCredential")));
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
		recordSet.push_back("There is not a user with this username: " + std::string(data.get("loginCredential")));
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
		recordSet.push_back("Could not find user with id: " + std::to_string(userId) + " or password is invalid.");
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

	recordSet.push_back("Could not find user with username: " + username + " or password is invalid.");
	return recordSet;
}

nlohmann::json DBManager::getJSONFromFile(std::string filename)
{
	std::ifstream JSONFile("users.json", std::fstream::app);

	if (!JSONFile.is_open())
	{
		throw std::string("Can not open file");
	}

	nlohmann::json userJSON;

	try
	{
		userJSON = nlohmann::json::parse(JSONFile);
	}
	catch (nlohmann::json::parse_error& ex)
	{
		CROW_LOG_WARNING << "users.json file is empty.";
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