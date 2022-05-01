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
	std::vector<std::string> isPunnettSquareDataValid(crow::query_string data, bool allowNulls = false);
	std::vector<std::string> checkForNullValues(crow::query_string data, std::vector<std::string> fields, std::vector<std::string> numFields = {});
private:
	ValidationHandler getValidationHandler(std::string field);
};
