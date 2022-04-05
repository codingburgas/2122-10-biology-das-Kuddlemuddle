#include <response.h>

crow::json::wvalue ResponseJSONManager::createJSONResponse(bool isSuccess, std::vector<std::string> fields, std::string type)
{
	crow::json::wvalue resBody;

	resBody["type"] = isSuccess ? type + "-success" : type + "-failure";
	if (fields.size() != 0)
	{
		resBody["filds"] = fields;
	}

	return resBody;
}
