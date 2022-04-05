#pragma once
#include <crow.h>
#include <nlohmann/json.hpp>
#include <time.h>

class DBManager
{
public:
	std::vector<std::string> registerUser(crow::query_string data);
private:
	nlohmann::json getJSONFromFile(std::string filename);
	bool setJSONFile(nlohmann::json json, std::string filename);
	int getLastId(nlohmann::json json);
	// Might need to be changed to get the user class
	bool getUserByField(nlohmann::json json, std::string field, std::string fieldData);
};