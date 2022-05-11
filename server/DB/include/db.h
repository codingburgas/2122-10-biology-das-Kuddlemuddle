/*! @file db.h
*   @brief A header file for the database.
*/

#pragma once
#include <env.h>
#include <crow.h>
#include <misc.h>
#include <time.h>
#include <types.h>
#include <bcrypt-lib.h>
#include <jwt-cpp/jwt.h>
#include <nlohmann/json.hpp>
#include <jwt-cpp/traits/nlohmann-json/traits.h>

/**
 * @brief Class for interacting with the database
*/
class DBManager
{
public:
	/**
	 * @brief Function for registering a user
	 * @param data The data of the user
	 * @param hash The password hash of the user
	 * @param salt The password salt of the user
	 * @param role The role of the user
	 * @return Errors if any
	*/
	std::vector<std::string> registerUser(
		crow::query_string data, 
		char* hash,
		char* salt, 
		int role = 0
	);

	/**
	 * @brief Function to check if user exists
	 * @param data The data of the user
	 * @return Token/Errors if any
	*/
	std::vector<std::string> loginUser(crow::query_string data);

	/**
	 * @brief Function that gets the user info from the database
	 * @param username Username of the user
	 * @param userId Id of the user
	 * @return User info/Errors if any
	*/
	std::vector<std::string> getUserInfo(std::string username, int userId = NULL);

	/**
	 * @brief Function for deletin a user by username or id
	 * @param username Username of th user
	 * @param userId Id of the user
	 * @return Errors if any
	*/
	std::vector<std::string> deleteUser(std::string username, int userId = NULL);

	/**
	 * @brief Function for updating a user
	 * @param userId Id of the user
	 * @param data Data to be updated
	 * @return Errors if any
	*/
	std::vector<std::string> updateUser(int userId, crow::query_string data);

	/**
	 * @brief Update url of the avarat image
	 * @param userId The id of the user
	 * @param imageName The name of the image
	 * @return Errors if any
	*/
	std::vector<std::string> updateUserAvatar(int userId, std::string imageName);
	
	/**
	 * @brief Function to save organisation to the database
	 * @param userId Id of the user that saves the database
	 * @param data The data of the organisation
	 * @param hash The password has of the organisation
	 * @param salt The password salt of the organisation
	 * @return Errors if any
	*/
	std::vector<std::string> createOrg(int userId, crow::query_string data, char* hash, char* salt);
	
	/**
	 * @brief Function for adding users to organisation
	 * @param userId Id of the user
	 * @param orgId Id of the organisation
	 * @param userRolesInOrgs Role of the user in the organisation
	 * @param createNewEntry Should create new entry, if this doesn't exist
	 * @return Errors if any
	*/
	std::vector<std::string> addUserToOrganisation(
		int userId, 
		int orgId, 
		UserRolesInOrgs userRolesInOrgs, 
		bool createNewEntry = true
	);
	
	/**
	 * @brief Function for getting organisation id by name
	 * @param orgName Name of the organisation
	 * @return ID/Errors if any
	*/
	std::vector<std::string> getOrgIdByName(std::string orgName);
	
	/**
	 * @brief Does password match organisation
	 * @param password The password for the organisation
	 * @param orgId The ID of the organisation
	 * @return Errors if any
	*/
	std::vector<std::string> doesPasswordMatchOrg(std::string password, int orgId);

	/**
	 * @brief Checks if user is in organisation and gets it's role
	 * @param userId The ID of the user
	 * @param orgId The ID of the organisation
	 * @return Is user in organisation and role/Errors if any
	*/
	std::vector<std::string> isUserInOrgAndGetRole(int userId, int orgId);
	
	/**
	 * @brief Function for deleting an organisation
	 * @param orgId ID of the Organisation
	 * @return Erros if any
	*/
	std::vector<std::string> deleteOrg(int orgId);

	/**
	 * @brief Function for updating organisation info
	 * @param orgId ID of the Organisation
	 * @param data New data for the organisation
	 * @return Erros if any
	*/
	std::vector<std::string> updateOrg(int orgId, crow::query_string data);
	
	/**
	 * @brief Get data about organisation by Name
	 * @param orgName Name of the organisation
	 * @return Data about the organisation
	*/
	OrgInfo getOrgInfo(std::string orgName);

	/**
	 * @brief Function that gets all of the organisations
	 * @return All of the organisations
	*/
	std::vector<OrgInfo> getAllOrgsInfo(int userId);

	/**
	 * @brief Save Course to the database
	 * @param data Data of the course
	 * @param hash Password hash of the course
	 * @param salt Password salt of the course
	 * @return Erros if any
	*/
	std::vector<std::string> createCourse(crow::query_string data, char* hash, char* salt);

	/**
	 * @brief 
	 * @param password 
	 * @param courseId 
	 * @return 
	*/
	std::vector<std::string> doesPasswordMatchCourse(std::string password, int courseId);

	/**
	 * @brief Functions for adding user to course
	 * @param userId ID of the user
	 * @param courseId ID of the course
	 * @param userRolesInOrgs Role of the user
	 * @param createNewEntry Should create new entry, if this doesn't exist
	 * @return Errors if any
	*/
	std::vector<std::string> addUserToCourse(
		int userId, 
		int courseId, 
		UserRolesInOrgs userRolesInOrgs, 
		bool createNewEntry = true
	);

	/**
	 * @brief Get course info by id
	 * @param courseId Id of the course
	 * @return Course data
	*/
	CourseInfo getCourseInfo(int courseId);

	/**
	 * @brief Checks if user is in Course and get role
	 * @param userId ID of the user
	 * @param courseId ID of the course
	 * @return Is user in course and role/Erros if any
	*/
	std::vector<std::string> isUserInCourseAndGetRole(int userId, int courseId);

	/**
	 * @brief Delete course
	 * @param courseId ID of the course
	 * @return Errors if any
	*/
	std::vector<std::string> deleteCourse(int courseId);

	/**
	 * @brief Update course
	 * @param courseId ID of the course
	 * @param data New data for the course
	 * @return Errors if any
	*/
	std::vector<std::string> updateCourse(int courseId, crow::query_string data);

	/**
	 * @brief Function for checking if user can access course
	 * @param courseId Course ID
	 * @param userId User ID
	 * @param userCanEdit Should user be allowed
	 * @return Can user access course/Errors if any
	*/
	std::vector<std::string> canUserAccessCourse(int courseId, int userId, bool userCanEdit = false);
	
	/**
	 * @brief Function for saving topic to the database
	 * @param data Data of the topic
	 * @return Erros if any
	*/
	std::vector<std::string> createTopic(crow::query_string data);
	
	/**
	 * @brief Function for getting info about topic
	 * @param topicId ID of the topic
	 * @return Topic data
	*/
	TopicInfo getTopicInfo(int topicId);
	
	/**
	 * @brief Function for deleting topic
	 * @param courseId ID of the coutse
	 * @return Errors if any
	*/
	std::vector<std::string> deleteTopic(int courseId);
	
	/**
	 * @brief Function for updating topic 
	 * @param topicId ID of the topic
	 * @param data New data for the topic
	 * @return Errors if any
	*/
	std::vector<std::string> updateTopic(int topicId, crow::query_string data);
	
	/**
	 * @brief Function for saving lesson to the database
	 * @param data Data for the lesson
	 * @return Errors if any
	*/
	std::vector<std::string> createLesson(crow::query_string data);
	
	/**
	 * @brief Function for getting lesson info
	 * @param lessonId ID of the lesson
	 * @return Lessons data
	*/
	LessonInfo getLessonInfo(int lessonId);
	
	/**
	 * @brief Function for deleting a lesson from the database
	 * @param lessonId ID of the lesson
	 * @return Errors if any
	*/
	std::vector<std::string> deleteLesson(int lessonId);
	
	/**
	 * @brief Function for updating lesson
	 * @param lessonId ID of the lesson
	 * @param data new data for the lesson
	 * @return Errors if any
	*/
	std::vector<std::string> updateLesson(int lessonId, crow::query_string data);
	
	/**
	 * @brief Function for saving quiz to the database
	 * @param data Data of the quiz
	 * @return Errors if any
	*/
	std::vector<std::string> createQuiz(crow::query_string data);
	
	/**
	 * @brief Function for getting quiz info
	 * @param quizId ID of quiz
	 * @return Quiz data
	*/
	QuizInfo getQuizInfo(int quizId);
	
	/**
	 * @brief Function for deleting a quiz from the database
	 * @param quizId ID of the quiz
	 * @return Errors if any
	*/
	std::vector<std::string> deleteQuiz(int quizId);
	
	/**
	 * @brief Function for updating quiz data
	 * @param quizId ID of the quiz
	 * @param data New data for the quiz
	 * @return Errors if any
	*/
	std::vector<std::string> updateQuiz(int quizId, crow::query_string data);
	
	/**
	 * @brief Function for saving a question to the database
	 * @param data Data of the question
	 * @return Errors if any
	*/
	std::vector<std::string> createQuestion(crow::query_string data);
	
	/**
	 * @brief Get the question data from ID
	 * @param quizId ID of the quiz
	 * @return Question data
	*/
	QuestionInfo getQuestionInfo(int quizId);
	
	/**
	 * @brief Function for deleting a question
	 * @param questionId ID of the question
	 * @return Errors if any
	*/
	std::vector<std::string> deleteQuestion(int questionId);
	
	/**
	 * @brief Function for updating question data
	 * @param questionId ID of the question
	 * @param data New data for question
	 * @return Errors if any
	*/
	std::vector<std::string> updateQuestion(int questionId, crow::query_string data);
	
	/**
	 * @brief Function for saving an attempt to the database
	 * @param data Data of the attempt
	 * @param userId ID of the user
	 * @return Errors if any
	*/
	std::vector<std::string> startAttempt(crow::query_string data, int userId);
	
	/**
	 * @brief Function for getting info about attempt
	 * @param attemptId ID of the attempt
	 * @return Info about the attempt
	*/
	AttemptInfo getAttemptInfo(int attemptId);
	
	/**
	 * @brief Get attempt ID from user and quiz ID
	 * @param userId ID of the user
	 * @param quizId ID of the quiz
	 * @return ID of the Attempt/ -1 if there is a error
	*/
	int getAttemptIdByUserIdAndQuizId(int userId, int quizId);
	
	/**
	 * @brief Function for deleting an attempt from the database
	 * @param attemptId ID of the attempt
	 * @return Errors if any
	*/
	std::vector<std::string> deleteAttempt(int attemptId);
	
	/**
	 * @brief Function for answering and question
	 * @param data Data of the answer
	 * @return Errors if any
	*/
	std::vector<std::string> answerQuestion(crow::query_string data);
	
	/**
	 * @brief Function for saving answer to the database and giving the ID of the next one
	 * @param attemptInfo Info about the attempt
	 * @param quizInfo Info about the quiz
	 * @return Id of the next qustion/Erros if any
	*/
	std::vector<std::string> goToNextQuestionInAttempt(AttemptInfo attemptInfo, QuizInfo quizInfo);
	
	/**
	 * @brief Function for getting all answers from attempt
	 * @param attemptId ID of the attempt
	 * @return All answers from attempt
	*/
	std::vector<AnswerInfo> getAllAnswersInAttemptWithId(int attemptId);
	
	/**
	 * @brief Function for getting an answer info
	 * @param answerId Id of the answer
	 * @return Answer data
	*/
	AnswerInfo getAnswersInfo(int answerId);
private:
	/**
	 * @brief Function that parses a text from file to JSON
	 * @param filename The name of the file
	 * @return JSON parrsed from the file
	*/
	nlohmann::json getJSONFromFile(std::string filename);

	/**
	 * @brief Save the JSON to a file
	 * @param json The JSON to be saved
	 * @param filename The name of the file
	 * @return Is the save successful
	*/
	bool setJSONFile(nlohmann::json json, std::string filename);
	
	/**
	 * @brief Function for getting the last id in a JSON
	 * @param json The json
	 * @return The last id
	*/
	int getLastId(nlohmann::json json);

	/**
	 * @brief Function for checking if value already exists
	 * @param json The JSON to be checked
	 * @param field The filed to be checked
	 * @param fieldData The value to be checked
	 * @return Is there duplicate
	*/
	bool checkIfValueExistsInField(nlohmann::json json, std::string field, std::string fieldData);

	/**
	 * @brief Function for checking if value already exists
	 * @param json The JSON to be checked
	 * @param field The filed to be checked
	 * @param fieldData The value to be checked
	 * @param field2 The filed to be checked
	 * @param fieldData2 The value to be checked
	 * @return Is there duplicate
	*/
	bool checkIfValueExistsInField(
		nlohmann::json json,
		std::string field,
		std::string fieldData,
		std::string field2,
		std::string fieldData2
	);

	/**
	* @brief Function for checking if value already exists
	 * @param json The JSON to be checked
	 * @param field The filed to be checked
	 * @param fieldData The value to be checked
	 * @param field2 The filed to be checked
	 * @param fieldData2 The value to be checked
	 * @return Is there duplicate
	*/
	bool checkIfValueExistsInField(
		nlohmann::json json,
		std::string field, 
		std::string fieldData, 
		std::string field2, 
		int fieldData2
	);

	/**
	 * @brief Get all users from organisation
	 * @param orgId ID of the organisation
	 * @return All users from organisation
	*/
	std::vector<OrgUser> getOrgUsersByOrgId(int orgId);

	/**
	 * @brief Get data in field by criteria
	 * @param filename Name of the file
	 * @param criteria Criteria to be checked
	 * @param criteriaField Field for the criteria
	 * @param field Field
	 * @return Data in the field
	*/
	std::vector<std::string> getFieldDataInJSONByCriteria(
		std::string filename, 
		int criteria,
		std::string criteriaField,
		std::string field
	);

	/**
	 * @brief Get all quizzes from topic
	 * @param topicId ID of the topic
	 * @return All quizzes from topic
	*/
	std::vector<QuizInfo> getAllQuizzesInTopicWithID(int topicId);

	/**
	 * @brief Get all lessons from topic
	 * @param topicId ID of the topic
	 * @return All lessons from topic
	*/
	std::vector<LessonInfo> getAllLessonInTopicWithID(int topicId);

	/**
	 * @brief Get all topics from course
	 * @param course ID of the course
	 * @return All topics from course
	*/
	std::vector<TopicInfo> getAllTopicsInCourseWithID(int courseId);

	/**
	 * @brief Get all users from course
	 * @param courseId ID of the course
	 * @return All users from course
	*/
	std::vector<OrgUser> getCourseUsersByCourseId(int courseId);

	/**
	 * @brief Get all course from organisation
	 * @param orgId ID of the organisation
	 * @return All courses from organisation
	*/
	std::vector<CourseInfo> getAllCoursesInOrgWithID(int orgId);

	/**
	 * @brief Get all questions from quiz
	 * @param quizId ID of the quiz
	 * @return All questions from quiz
	*/
	std::vector<QuestionInfo> getAllQuestionsInQuizWithId(int quizId);

	/**
	 * @brief Get all attempts from quiz
	 * @param quizId ID of the quiz
	 * @return All attempts from quiz
	*/
	std::vector<AttemptInfo> getAllAttemptsInQuizWithId(int quizId);

	/**
	 * @brief Calculate score for attempt
	 * @param attemptId ID of the attempt
	 * @return Score for the attempt
	*/
	float calculateScoreForAttempt(int attemptId);

	/**
	 * @brief Calculate score for attempt
	 * @param userAnswers User answers
	 * @param questionId ID of the question
	 * @return Score for attempt
	*/
	float calculateScoreForPunnetSquare(std::vector<std::string> userAnswers, int questionId);

	/**
	 * @brief Delete answer
	 * @param answerId ID for answer
	 * @return Errors if any
	*/
	std::vector<std::string> deleteAnswer(int answerId);

	/**
	 * @brief Fet answer for Punnett square
	 * @param questionId ID of the question
	 * @return Answer for the Punnett square
	*/
	std::vector<std::string> getPunnetSquareAnswer(int questionId);
};