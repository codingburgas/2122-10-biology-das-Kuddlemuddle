#pragma once
#include <env.h>
#include <crow.h>
#include <misc.h>
#include <time.h>
#include <types.h>
#include <jwt-cpp/jwt.h>
#include <nlohmann/json.hpp>
#include <jwt-cpp/traits/nlohmann-json/traits.h>

class DBManager
{
public:
	std::vector<std::string> registerUser(crow::query_string data, char* hash, char* salt, int role = 0);
	std::vector<std::string> loginUser(crow::query_string data);
	std::vector<std::string> getUserInfo(std::string username, int userId = NULL);
	std::vector<std::string> deleteUser(std::string username, int userId = NULL);
	std::vector<std::string> updateUser(int userId, crow::query_string data);
	std::vector<std::string> updateUserAvatar(int userId, std::string imageName);
	std::vector<std::string> createOrg(int userId, crow::query_string data, char* hash, char* salt);
	std::vector<std::string> addUserToOrganisation(int userId, int orgId, UserRolesInOrgs userRolesInOrgs, bool createNewEntry = true);
	std::vector<std::string> getOrgIdByName(std::string orgName);
	std::vector<std::string> doesPasswordMatchOrg(std::string password, int orgId);
	std::vector<std::string> isUserInOrgAndGetRole(int userId, int orgId);
	std::vector<std::string> deleteOrg(int orgId);
	std::vector<std::string> updateOrg(int orgId, crow::query_string data);
	OrgInfo getOrgInfo(std::string orgName);
	std::vector<OrgInfo> getAllOrgsInfo();
	std::vector<std::string> createCourse(crow::query_string data, char* hash, char* salt);
	std::vector<std::string> doesPasswordMatchCourse(std::string password, int courseId);
	std::vector<std::string> addUserToCourse(int userId, int courseId, UserRolesInOrgs userRolesInOrgs, bool createNewEntry = true);
	CourseInfo getCourseInfo(int courseId);
	std::vector<std::string> isUserInCourseAndGetRole(int userId, int courseId);
	std::vector<std::string> deleteCourse(int courseId);
	std::vector<std::string> updateCourse(int courseId, crow::query_string data);
	std::vector<std::string> canUserAccessCourse(int courseId, int userId, bool userCanEdit = false);
	std::vector<std::string> createTopic(crow::query_string data);
	TopicInfo getTopicInfo(int topicId);
	std::vector<std::string> deleteTopic(int courseId);
	std::vector<std::string> updateTopic(int topicId, crow::query_string data);
	std::vector<std::string> createLesson(crow::query_string data);
	LessonInfo getLessonInfo(int lessonId);
	std::vector<std::string> deleteLesson(int lessonId);
	std::vector<std::string> updateLesson(int lessonId, crow::query_string data);
	std::vector<std::string> createQuiz(crow::query_string data);
	QuizInfo getQuizInfo(int quizId);
	std::vector<std::string> deleteQuiz(int quizId);
	std::vector<std::string> updateQuiz(int quizId, crow::query_string data);
	std::vector<std::string> createQuestion(crow::query_string data);
	QuestionInfo getQuestionInfo(int quizId);
	std::vector<std::string> deleteQuestion(int questionId);
	std::vector<std::string> updateQuestion(int questionId, crow::query_string data);
	std::vector<std::string> startAttempt(crow::query_string data, int userId);
	AttemptInfo getAttemptInfo(int attemptId);
	int getAttemptIdByUserIdAndQuizId(int userId, int quizId);
	std::vector<std::string> deleteAttempt(int attemptId);
	std::vector<std::string> answerQuestion(crow::query_string data);
	std::vector<std::string> goToNextQuestionInAttempt(AttemptInfo attemptInfo, QuizInfo quizInfo);
	std::vector<AnswerInfo> getAllAnswersInAttemptWithId(int attemptId);
	AnswerInfo getAnswersInfo(int answerId);
private:
	nlohmann::json getJSONFromFile(std::string filename);
	bool setJSONFile(nlohmann::json json, std::string filename);
	int getLastId(nlohmann::json json);
	bool checkIfValueExistsInField(nlohmann::json json, std::string field, std::string fieldData);
	bool checkIfValueExistsInField(nlohmann::json json, std::string field, std::string fieldData, std::string field2, std::string fieldData2);
	bool checkIfValueExistsInField(nlohmann::json json, std::string field, std::string fieldData, std::string field2, int fieldData2);
	std::vector<OrgUser> getOrgUsersByOrgId(int orgId);
	std::vector<std::string> getFieldDataInJSONByCriteria(std::string filename, int criteria, std::string criteriaField, std::string field);
	std::vector<QuizInfo> getAllQuizzesInTopicWithID(int topicId);
	std::vector<LessonInfo> getAllLessonInTopicWithID(int topicId);
	std::vector<TopicInfo> getAllTopicsInCourseWithID(int courseId);
	std::vector<OrgUser> getCourseUsersByCourseId(int courseId);
	std::vector<CourseInfo> getAllCoursesInOrgWithID(int orgId);
	std::vector<QuestionInfo> getAllQuestionsInQuizWithId(int quizId);
	std::vector<AttemptInfo> getAllAttemptsInQuizWithId(int quizId);
	float calculateScoreForAttempt(int attemptId);
	float calculateScoreForPunnetSquare(std::vector<std::string> userAnswers, int questionId);
	std::vector<std::string> deleteAnswer(int answerId);
	std::vector<std::string> getPunnetSquareAnswer(int questionId);
};