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

/**
 * @brief Struct that holds the role and id of a user in organisation
*/
struct OrgUser
{
    int id; /*!< ID of the user */
    int role; /*!< Role of the user */
    std::vector<int> userCoursesId; /* ID of the courses in which this user is part of */
};

/**
 * @brief Struct that holds topic data
*/
struct TopicInfo
{
    int id; /*!< ID of the topic */
    int courseId; /*!< ID of the course */
    std::string name; /*!< Name of the topic */
    //std::vector<LessonInfo> lessons; /*!< Lessons for this topic */
    //std::vector<QuizInfo> quizzes; /*!< Quizzes for this topic */
    std::vector<std::string> errors; /*!< Errors for this topic */
};

/**
 * @brief Struct that holds course data
*/
struct CourseInfo
{
    int id; /*!< ID of the course */
    int orgId; /*!< ID of the organisation */
    std::string name; /*!< Name of the course */
    std::vector<OrgUser> users; /*!< Users in this course */
    std::vector<TopicInfo> topics; /*!< Topics in this course */
    std::string errors; /*!< Errors in this course */
};

struct OrgInfo
{
    int id;
    std::string name;
    bool isAdmin;
    std::vector<OrgUser> users;
    std::vector<CourseInfo> courses;
    std::string errors;
};

struct SceneContex
{
    bool isAuth = false;
    User user;
    std::string JWTToken;
    bool isDataFetchOnPage = false;
    OrgInfo orgInfo;
    CourseInfo courseInfo;
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
	std::string getUserInfo(std::string username, SceneContex* ctx, User& user);
	std::string getUserInfoById(std::string userId, SceneContex* ctx, User& user);
    std::string deleteUser(std::string username, std::string JWTToken);
    std::string createOrg(OrgData orgData, std::string JWTToken);
    std::string joinOrg(int orgId, std::string password, std::string JWTToken);
    std::string updateOrg(OrgData orgData, std::string name, std::string JWTToken);
    std::string deleteOrg(std::string name, std::string JWTToken);
    std::string updateRoleInOrg(int orgId, int userId, int roleId, std::string JWTToken);
    std::string createCourse(OrgData courseData, int orgId, std::string JWTToken);
    std::string joinCourse(int courseId, std::string password, std::string JWTToken);
    bool doUserHaveAccessToOrg(std::string name, std::string JWTToken);
    std::vector<User> getAllUsers(std::string JWTToken);
    std::vector<OrgInfo> getAllOrgs(std::string JWTToken);
    OrgInfo getOrg(std::string name, std::string JWTToken);
    CourseInfo getCourse(int id, std::string JWTToken);
private:

};