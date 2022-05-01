/*! @file types.h
*   @brief A header file for the all custom types.
*/

#pragma once

/**
 * Enumeration representing the roles in organisation
*/
enum class UserRolesInOrgs
{
	USER = 0, /*!< User role */
	TEACHER = 1, /*!< Teacher role */
	ADMIN = 2 /*!< Admin role */
};

/**
 * @brief Struct that holds the role and id of a user in organisation
*/
struct OrgUser
{
	int id; /*!< ID of the user */
	int role; /*!< Role of the user */
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
	std::vector<std::string> errors; /*!< Errors in this course */
};

/**
 * @brief Struct that holds organisation data
*/
struct OrgInfo
{
	int id; /*!< ID of the organisation */
	std::string name; /*!< Name of the organisation */
	std::vector<OrgUser> users; /*!< Users in the organisation */
	std::vector<CourseInfo> courses; /*!< Courses in the organisation */
	std::vector<std::string> errors; /*!< Errors in the organisation */
};