/*! @file response.h
*   @brief A header file for the response system.
*/

#pragma once
#include <crow.h>
#include <db.h>

/**
 * @brief Class for the response system
*/
class ResponseJSONManager
{
public:
	/**
	 * @brief Create JSON response based on data
	 * @param isSuccess Is the response successful
	 * @param fields The fileds to be in the JSON response
	 * @param type Type of the response
	 * @return JSON to be sent to the client
	*/
	crow::json::wvalue createJSONResponse(bool isSuccess, std::vector<std::string> fields, std::string type);
	crow::json::wvalue createProfileJSONResponse(std::vector<std::string> fields);
	crow::json::wvalue createOrgJSONResponse(OrgInfo data);
	crow::json::wvalue createOrgsJSONResponse(std::vector<OrgInfo> data);
	crow::json::wvalue createCourseJSONResponse(CourseInfo data);
	crow::json::wvalue createTopicJSONResponse(TopicInfo data);
	crow::json::wvalue createLessonJSONResponse(LessonInfo data);
	crow::json::wvalue createQuizJSONResponse(QuizInfo data, bool shareData);
	crow::json::wvalue createQuestionJSONResponse(QuestionInfo data, bool shareAnswer = true);
	crow::json::wvalue createAttemptJSONResponse(AttemptInfo data, std::string type);
	crow::json::wvalue createAnswerJSONResponse(AnswerInfo data);
};
