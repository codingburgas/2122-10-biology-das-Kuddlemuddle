#pragma once
#include <iostream>
#include <cpr/cpr.h>

enum class WINDOWS
{
    REGISTER = 1,
    LOGIN = 2,
    ACCOUNT = 3
};

struct User
{
    std::string fname;
    std::string lname;
    std::string username;
    std::string email;
    std::string avatarURL;
    std::string role;
};

struct LayerContex
{
    bool isAuth = false;
    User user;
    std::string JWTToken;
    WINDOWS currentWindow = WINDOWS::LOGIN;
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
	std::string loginHandler(LoginData logData, LayerContex* ctx);
	std::string getUserInfo(std::string userId, LayerContex* ctx, User& user);
	void getImage(std::string &url, std::string fileExtension);
    std::vector<OrgInfo> getAllOrgs(std::string JWTToken);
private:

};