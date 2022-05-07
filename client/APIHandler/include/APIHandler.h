#pragma once
#include <iostream>
#include <cpr/cpr.h>

struct User
{
    std::string id;
    std::string fname;
    std::string lname;
    std::string username;
    std::string email;
    std::string avatarURL;
    std::string role;
    std::string errors;
};

struct SceneContex
{
    bool isAuth = false;
    User user;
    std::string JWTToken;
    bool isDataFetchOnPage = false;
};

struct OrgInfo
{
    int id;
    std::string name;
    /*
    std::vector<OrgUser> users;
    std::vector<CourseInfo> courses;
    */
    std::string errors;
};

struct OrgData
{
    std::string name;
    std::string password;
};

struct RegisterData
{
    std::string fname;
    std::string lname;
    std::string username;
    std::string email;
    std::string password;
};

struct LoginData
{
    std::string loginCred;
    std::string pass;
};


class APIHandler
{
public:
	std::string registerHandler(RegisterData regData);
	std::string loginHandler(LoginData logData, SceneContex* ctx);
	std::string getUserInfo(std::string userId, SceneContex* ctx, User& user);
    std::string deleteUser(std::string username, std::string JWTToken);
    std::string createOrg(OrgData orgData, std::string JWTToken);
    std::vector<User> getAllUsers(std::string JWTToken);
    std::vector<OrgInfo> getAllOrgs(std::string JWTToken);
private:

};