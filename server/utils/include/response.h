#pragma once
#include <crow.h>

class ResponseJSONManager
{
public:
	crow::json::wvalue createJSONResponse(bool isSuccess, std::vector<std::string> fields, std::string type);
	crow::json::wvalue createProfileJSONResponse(std::vector<std::string> fields);
};
