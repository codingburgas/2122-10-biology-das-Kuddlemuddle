#pragma once
#include <env.h>
#include <crow.h>
#include <misc.h>
#include <time.h>
#include <jwt-cpp/jwt.h>
#include <nlohmann/json.hpp>

class DBManager
{
public:
	std::vector<std::string> registerUser(crow::query_string data);
	std::vector<std::string> loginUser(crow::query_string data);
	std::vector<std::string> getUserInfo(std::string username, int userId = NULL);
private:
	nlohmann::json getJSONFromFile(std::string filename);
	bool setJSONFile(nlohmann::json json, std::string filename);
	int getLastId(nlohmann::json json);
	// Might need to be changed to get the user class
	bool getUserByField(nlohmann::json json, std::string field, std::string fieldData);
};