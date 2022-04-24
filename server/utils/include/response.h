#pragma once
#include <crow.h>
#include <db.h>

class ResponseJSONManager
{
public:
	crow::json::wvalue createJSONResponse(bool isSuccess, std::vector<std::string> fields, std::string type);
	crow::json::wvalue createProfileJSONResponse(std::vector<std::string> fields);
	crow::json::wvalue createOrgJSONResponse(OrgInfo data);
	crow::json::wvalue createOrgsJSONResponse(std::vector<OrgInfo> data);
	crow::json::wvalue createCourseJSONResponse(CourseInfo data);
	crow::json::wvalue createTopicJSONResponse(TopicInfo data);
};
