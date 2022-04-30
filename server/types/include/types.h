#pragma once
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

struct AnswerInfo
{
	int id;
	float score;
	int attemptId;
	bool isCorrect;
	int questionId;
	std::string answer;
	std::string type;
	std::vector<std::string> punnettAnswer;
	std::vector<std::string> errors;
};

struct AttemptInfo
{
	int id;
	int quizId;
	int userId;
	int currentQuestionId;
	float score;
	time_t timeStart;
	time_t timeEnd;
	bool inProgress;
	std::vector<AnswerInfo> answers;
	std::vector<std::string> errors;
};

struct QuestionInfo
{
	int id;
	int quizId;
	std::string type;
	std::string question;
	std::string answer;
	std::vector<std::string> punnettAnswer;
	std::vector<std::string> errors;
};

struct QuizInfo
{
	int id;
	int topicId;
	std::string name;
	std::vector<AttemptInfo> attempts;
	std::vector<QuestionInfo> questions;
	std::vector<std::string> errors;
};

struct LessonInfo
{
	int id;
	int topicId;
	std::string name;
	std::string data;
	std::vector<std::string> errors;
};

struct TopicInfo
{
	int id;
	int courseId;
	std::string name;
	std::vector<LessonInfo> lessons;
	std::vector<QuizInfo> quizzes;
	std::vector<std::string> errors;
};

struct CourseInfo
{
	int id;
	int orgId;
	std::string name;
	std::vector<OrgUser> users;
	std::vector<TopicInfo> topics;
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