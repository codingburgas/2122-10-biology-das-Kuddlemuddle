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
 * @brief Struct that holds answer data
*/
struct AnswerInfo
{
    int id; /*!< ID of the answer */
    float score; /*!< Score of the answer */
    int attemptId; /*!< ID of the attempt */
    bool isCorrect; /*!< Is the answer correct */
    int questionId; /*!< ID of the question */
    std::string answer; /*!< Answer of the answer*/
    std::string type; /*!< Type of the answer*/
    std::vector<std::string> punnettAnswer; /*!< Punnett answer of the answer*/
    std::vector<std::string> errors; /*!< Errors */
};

/**
 * @brief Struct that holds attempt data
*/
struct AttemptInfo
{
    int id; /*!< ID of the attempt */
    int quizId; /*!< ID of the quiz */
    int userId; /*!< ID of the user */
    int currentQuestionId; /*!< ID of the current question */
    float score; /*!< Score for this attempt */
    time_t timeStart; /*!< Time of start */
    time_t timeEnd; /*!< ID of end */
    bool inProgress; /*!< Is the attempt in prgress */
    std::vector<AnswerInfo> answers; /*!< Answers for this attempt */
    std::vector<std::string> errors; /*!< Errors for this attempt */
};

/**
 * @brief Struct that holds question data
*/
struct QuestionInfo
{
    int id; /*!< ID of the question */
    int quizId; /*!< ID of the quiz */
    std::string type; /*!< Type of the question */
    std::string question; /*!< Questions */
    std::string answer; /*!< Answer for this question */
    std::vector<std::string> punnettAnswer; /*!< Punnett answer for this question */
    std::vector<std::string> errors; /*!< Errors for this question */
};

/**
 * @brief Struct that holds quiz data
*/
struct QuizInfo
{
    int id; /*!< ID of the quiz */
    int topicId; /*!< ID of the topic */
    std::string name; /*!< Name of the quiz */
    std::vector<AttemptInfo> attempts; /*!< Attempts for this quiz */
    std::vector<QuestionInfo> questions; /*!< Questions for this quiz */
    std::vector<std::string> errors; /*!< Errors for this quiz */
};

/**
 * @brief Struct that holds lesson data
*/
struct LessonInfo
{
    int id; /*!< ID of the lesson */
    int topicId; /*!< ID of the topic */
    std::string name; /*!< Name of the lesson */
    std::string data; /*!< Data of the lesson */
    std::vector<std::string> errors; /*!< Errors for this the lesson */
};

/**
 * @brief Struct that holds topic data
*/
struct TopicInfo
{
    int id; /*!< ID of the topic */
    int courseId; /*!< ID of the course */
    std::string name; /*!< Name of the topic */
    std::vector<LessonInfo> lessons; /*!< Lessons for this topic */
    std::vector<QuizInfo> quizzes; /*!< Quizzes for this topic */
    std::string errors; /*!< Errors for this topic */
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
    TopicInfo topicInfo;
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
    std::string updateUser(RegisterData userData, std::string JWTToken);
    std::string createOrg(OrgData orgData, std::string JWTToken);
    std::string joinOrg(int orgId, std::string password, std::string JWTToken);
    std::string updateOrg(OrgData orgData, std::string name, std::string JWTToken);
    std::string deleteOrg(std::string name, std::string JWTToken);
    std::string updateRoleInOrg(int orgId, int userId, int roleId, std::string JWTToken);
    std::string createCourse(OrgData courseData, int orgId, std::string JWTToken);
    std::string joinCourse(int courseId, std::string password, std::string JWTToken);
    std::string deleteCourse(int id, std::string JWTToken);
    std::string updateCourse(OrgData orgData, int id, std::string JWTToken);
    std::string addTeacherToCourse(int courseId, int teacherId, std::string JWTToken);
    std::string createTopic(std::string topicName, int courseId, std::string JWTToken);
    std::string updateTopic(std::string topicName, int id, std::string JWTToken);
    std::string deleteTopic(int id, std::string JWTToken);
    std::string createLesson(std::string lessonName, std::string lessonData, int topicId, std::string JWTToken);
    bool doUserHaveAccessToOrg(std::string name, std::string JWTToken);
    std::vector<User> getAllUsers(std::string JWTToken);
    std::vector<OrgInfo> getAllOrgs(std::string JWTToken);
    OrgInfo getOrg(std::string name, std::string JWTToken);
    CourseInfo getCourse(int id, std::string JWTToken);
    TopicInfo getTopic(int id, std::string JWTToken);
private:

};