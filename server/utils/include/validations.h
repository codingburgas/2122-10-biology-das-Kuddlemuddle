#pragma once
#include <crow.h>
#include <regex>

typedef bool (*ValidationHandler)(std::string data);

class ValidationManager
{
public:
	std::vector<std::string> isRegisterDataValid(crow::query_string data);
	std::vector<std::string> isLoginDataValid(crow::query_string data);
private:
	ValidationHandler getValidationHandler(std::string field);
};
