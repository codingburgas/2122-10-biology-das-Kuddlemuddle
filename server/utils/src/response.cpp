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

crow::json::wvalue ResponseJSONManager::createOrgJSONResponse(OrgInfo data)
{
	crow::json::wvalue resBody;

	resBody["type"] = "get-organisation-success";
	resBody["org-id"] = data.id;
	resBody["org-name"] = data.name;

	for (size_t i = 0; i < data.users.size(); i++)
	{
		resBody["org-users"][i] = { {"user-id:", data.users[i].id} , {"role-id", data.users[i].role} };
	}

	return resBody;
}

crow::json::wvalue ResponseJSONManager::createOrgsJSONResponse(std::vector<OrgInfo> data)
{
	crow::json::wvalue resBody;

	resBody["type"] = "get-organisation-success";
	
	for (size_t i = 0; i < data.size(); i++)
	{
		resBody["orgs"][i] = { {"org-id:", data[i].id} , {"org-name", data[i].name} };
	}

	return resBody;
}
