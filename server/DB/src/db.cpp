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
		//CROW_LOG_CRITICAL << "Could'n open user.json file";
		recordSet.push_back("Could'n open user.json file");
		return recordSet;
	}

	// Check for duplicate email
	if (getUserByField(userJSON, "Email", data.get("email")))
	{
		recordSet.push_back("There is already a user with this email");
		return recordSet;
	}

	// Check for duplicate username
	if (getUserByField(userJSON, "Username", data.get("username")))
	{
		recordSet.push_back("There is already a user with this username");
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
		{ "Token", NULL },
		{ "TokenExpireData", NULL},
		{ "Password", data.get("password") },
		{ "RoleID", 0 },
		// TODO: Add salt
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
		if (it.value()[field] == fieldData)
		{
			return true;
		}
	}

	return false;
}