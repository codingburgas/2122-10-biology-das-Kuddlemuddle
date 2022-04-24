#pragma once
#include <crow.h>
#include <regex>

typedef bool (*ValidationHandler)(std::string data);

class ValidationManager
{
public:
	std::vector<std::string> isRegisterDataValid(crow::query_string data, bool allowNulls = false);
	std::vector<std::string> isLoginDataValid(crow::query_string data);
	std::vector<std::string> isOrgDataValid(crow::query_string data, bool allowNulls = false);
	std::vector<std::string> isJoinOrgDataValid(crow::query_string data);
	std::vector<std::string> isCourseDataValid(crow::query_string data, bool allowNulls = false);
private:
	ValidationHandler getValidationHandler(std::string field);
};
