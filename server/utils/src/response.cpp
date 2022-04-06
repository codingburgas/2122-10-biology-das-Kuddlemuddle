#include <response.h>

crow::json::wvalue ResponseJSONManager::createJSONResponse(
	bool isSuccess,
	std::vector<std::string> data, 
	std::string type
)
{
	crow::json::wvalue resBody;

	if (isSuccess)
	{
		resBody["type"] = type + "-success";
		
		if (data.size() != 0)
		{
			resBody["data"] = data;
		}
	}
	else
	{
		resBody["type"] = type + "-failure";

		if (data.size() != 0)
		{
			resBody["fields"] = data;
		}
	}

	return resBody;
}

crow::json::wvalue ResponseJSONManager::createProfileJSONResponse(std::vector<std::string> data)
{
	crow::json::wvalue resBody;

	if (data.size() == 1)
	{
		resBody["type"] = "user-failure";
		resBody["reason"] = "Error 404";
	}
	else
	{
		resBody["type"] = "user-success";
		resBody["data"] = data;
	}

	return resBody;
}
