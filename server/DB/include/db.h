#pragma once
#include <env.h>
#include <crow.h>
#include <misc.h>
#include <time.h>
#include <jwt-cpp/jwt.h>
#include <nlohmann/json.hpp>
#include <jwt-cpp/traits/nlohmann-json/traits.h>

enum class UserRolesInOrgs
{
	USER = 0,
	TEACHER = 1,
	ADMIN = 2
};

class DBManager
{
public:
	std::vector<std::string> registerUser(crow::query_string data);
	std::vector<std::string> loginUser(crow::query_string data);
	std::vector<std::string> getUserInfo(std::string username, int userId = NULL);
	std::vector<std::string> deleteUser(std::string username, int userId = NULL);
	std::vector<std::string> updateUser(int userId, crow::query_string data);
	std::vector<std::string> updateUserAvatar(int userId, std::string imageName);
	std::vector<std::string> createOrg(int userId, crow::query_string data);
	std::vector<std::string> addUserToOrganisation(int userId, int orgId, UserRolesInOrgs userRolesInOrgs, bool createNewEntry = true);
	std::vector<std::string> getOrgIdByName(std::string orgName);
	std::vector<std::string> doesPasswordMatchOrg(std::string password, int orgId);
	std::vector<std::string> isUserAdminInOrg(int userId, int orgId);
private:
	nlohmann::json getJSONFromFile(std::string filename);
	bool setJSONFile(nlohmann::json json, std::string filename);
	int getLastId(nlohmann::json json);
	// Might need to be changed to get the user class
	bool getUserByField(nlohmann::json json, std::string field, std::string fieldData);
};