#pragma once
#include <env.h>
#include <crow.h>
#include <misc.h>
#include <time.h>
#include <jwt-cpp/jwt.h>
#include <nlohmann/json.hpp>
#include <jwt-cpp/traits/nlohmann-json/traits.h>

// This should go in seperate .lib
enum class UserRolesInOrgs
{
	USER = 0,
	TEACHER = 1,
	ADMIN = 2
};

struct OrgUser
{
	int id;
	int role;
};

struct CourseInfo
{
	int id;
	int orgId;
	std::string name;
	std::vector<OrgUser> users;
	std::vector<std::string> errors;
};

struct OrgInfo
{
	int id;
	std::string name;
	std::vector<OrgUser> users;
	std::vector<CourseInfo> courses;
	std::vector<std::string> errors;
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
	std::vector<std::string> isUserInOrgAndGetRole(int userId, int orgId);
	std::vector<std::string> deleteOrg(int orgId);
	std::vector<std::string> updateOrg(int orgId, crow::query_string data);
	OrgInfo getOrgInfo(std::string orgName);
	std::vector<OrgInfo> getAllOrgsInfo();
	std::vector<std::string> createCourse(crow::query_string data);
	std::vector<std::string> doesPasswordMatchCourse(std::string password, int courseId);
	std::vector<std::string> addUserToCourse(int userId, int courseId, UserRolesInOrgs userRolesInOrgs, bool createNewEntry = true);
	CourseInfo getCourseInfo(int courseId);
	//std::vector<std::string> getCourseIdByName(std::string courseName);
	std::vector<std::string> getCourseNameById(int id);
	std::vector<std::string> isUserInCourseAndGetRole(int userId, int courseId);
	std::vector<CourseInfo> getAllCoursesInOrgWithID(int orgId);
	std::vector<std::string> deleteCourse(int courseId);
	std::vector<OrgUser> getCourseUsersByCourseId(int courseId);
	std::vector<std::string> updateCourse(int courseId, crow::query_string data);
private:
	nlohmann::json getJSONFromFile(std::string filename);
	bool setJSONFile(nlohmann::json json, std::string filename);
	int getLastId(nlohmann::json json);
	bool checkIfValueExistsInField(nlohmann::json json, std::string field, std::string fieldData);
	bool checkIfValueExistsInField(nlohmann::json json, std::string field, std::string fieldData, std::string field2, std::string fieldData2);
	std::vector<OrgUser> getOrgUsersByOrgId(int orgId);
	std::vector<std::string> getCourseOrgById(int id);
};