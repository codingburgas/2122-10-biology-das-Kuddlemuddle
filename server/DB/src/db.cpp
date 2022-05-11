/*! @file db.cpp
*   @brief A source file for the database.
*/

#include <db.h>


std::vector<std::string> DBManager::registerUser(
	crow::query_string data,
	char* hash,
	char* salt,
	int role
)
{
	std::vector<std::string> recordSet;

	nlohmann::json userJSON;

	// Get the JSON from the file
	try
	{
		userJSON = getJSONFromFile("users.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open user.json file");
		return recordSet;
	}

	// Check for duplicate email
	if (checkIfValueExistsInField(userJSON, "Email", data.get("email")))
	{
		recordSet.push_back("There is already a user with this email: " + std::string(data.get("email")));
		return recordSet;
	}

	// Check for duplicate username
	if (checkIfValueExistsInField(userJSON, "Username", data.get("username")))
	{
		recordSet.push_back("There is already a user with this username: " + std::string(data.get("username")));
		return recordSet;
	}

	// Check if json is []
	if (userJSON.is_array())
	{
		if (userJSON.empty())
		{
			userJSON = nullptr;
		}
	}

	// Add the user to the JSON
	userJSON.push_back(
		{
		{ "ID", getLastId(userJSON) + 1},
		{ "FirstName", data.get("firstName") },
		{ "LastName", data.get("lastName") },
		{ "Username", data.get("username") },
		{ "Email", data.get("email") },
		{ "Password", hash },
		{ "Salt", salt },
		{ "RoleID", role },
		{ "AvatarURL", std::string("https://avatars.dicebear.com/api/identicon/" + std::to_string(time(0)) + ".svg") }
		}
	);

	// Save the json to the file
	if (!setJSONFile(userJSON, "users.json"))
	{
		recordSet.push_back("Could'n open user.json file");
		return recordSet;
	}

	return recordSet;
}

std::vector<std::string> DBManager::loginUser(crow::query_string data)
{
	EnvManager envManager;
	std::vector<std::string> recordSet;
	nlohmann::json userJSON;

	// Get the JSON from the file
	try
	{
		userJSON = getJSONFromFile("users.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open user.json file");
		return recordSet;
	}

	// Check if user is trying to log in with email or username
	if (std::string(data.get("loginCredential")).find("@") != std::string::npos)
	{
		for (auto it = userJSON.begin(); it != userJSON.end(); ++it)
		{
			// Authtenticate user
			if (it.value()["Email"] == data.get("loginCredential") &&
				bcrypt_checkpw(
					data.get("password"),
					std::string(it.value()["Password"]).c_str()
				) == 0
				)
			{
				// Authorise user -> Generate JWT token for this user
				auto token = jwt::create<jwt::traits::nlohmann_json>()
					.set_type("JWS")
					.set_subject(std::to_string(int(it.value()["ID"])))
					.set_issued_at(std::chrono::system_clock::now())
					.set_expires_at(std::chrono::system_clock::now() + std::chrono::hours(24))
					.set_payload_claim("isAdmin", int(it.value()["RoleID"]))
					.sign(jwt::algorithm::hs256(envManager.getEnv("JWT_SECRET")));

				recordSet.push_back("Bearer " + token);
				return recordSet;
			}
		}

		// Login failure
		recordSet.push_back("There is not a user with this email: " +
			std::string(data.get("loginCredential")) + " or password is invalid.");

		return recordSet;
	}
	else
	{
		for (auto it = userJSON.begin(); it != userJSON.end(); ++it)
		{
			// Authtenticate user
			if (toLowerCase(it.value()["Username"]) == toLowerCase(data.get("loginCredential"))
				&& bcrypt_checkpw(
					data.get("password"), std::string(it.value()["Password"]).c_str()
				) == 0
				)
			{
				// Authorise user
				auto token = jwt::create<jwt::traits::nlohmann_json>()
					.set_type("JWS")
					.set_subject(std::to_string(int(it.value()["ID"])))
					.set_issued_at(std::chrono::system_clock::now())
					.set_expires_at(std::chrono::system_clock::now() + std::chrono::hours(24))
					.set_payload_claim("isAdmin", int(it.value()["RoleID"]))
					.sign(jwt::algorithm::hs256(envManager.getEnv("JWT_SECRET")));

				recordSet.push_back("Bearer " + token);
				return recordSet;
			}
		}

		// Login failure
		recordSet.push_back("There is not a user with this username: " + 
			std::string(data.get("loginCredential")) + " or password is invalid.");
		return recordSet;
	}

	return recordSet;
}

std::vector<std::string> DBManager::getUserInfo(std::string username, int userId)
{
	std::vector<std::string> recordSet;
	nlohmann::json userJSON;

	// Get the JSON from the file
	try
	{
		userJSON = getJSONFromFile("users.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open user.json file");
		return recordSet;
	}

	// Check if user is trying to access his account
	if (username == "@me")
	{
		for (auto it = userJSON.begin(); it != userJSON.end(); ++it)
		{
			if (it.value()["ID"] == userId)
			{
				return {
					std::to_string(it.value()["ID"].get<int>()),
					it.value()["FirstName"],
					it.value()["LastName"],
					it.value()["Username"],
					it.value()["Email"],
					std::to_string(it.value()["RoleID"].get<int>()),
					it.value()["AvatarURL"]
				};
			}
		}

		// If the execution goes here, there should be smt very wrong
		recordSet.push_back("Could not find user with id: " + std::to_string(userId));
		return recordSet;
	}

	if (username == "all")
	{
		for (auto it = userJSON.begin(); it != userJSON.end(); ++it)
		{
			recordSet.push_back(std::to_string(int(it.value()["ID"])));
			recordSet.push_back(it.value()["FirstName"]);
			recordSet.push_back(it.value()["LastName"]);
			recordSet.push_back(it.value()["Username"]);
			recordSet.push_back(it.value()["Email"]);
			recordSet.push_back(std::to_string(int(it.value()["RoleID"])));
			recordSet.push_back(it.value()["AvatarURL"]);
		}

		return recordSet;
	}

	for (auto it = userJSON.begin(); it != userJSON.end(); ++it)
	{
		// Authtenticate user
		if (toLowerCase(it.value()["Username"]) == toLowerCase(username))
		{
			return {
					it.value()["FirstName"],
					it.value()["LastName"],
					it.value()["Username"],
					it.value()["Email"],
					std::to_string(int(it.value()["RoleID"])),
					it.value()["AvatarURL"]
			};
		}
	}

	recordSet.push_back("Could not find user with username: " + username);
	return recordSet;
}

std::vector<std::string> DBManager::deleteUser(std::string username, int userId)
{
	std::vector<std::string> recordSet;
	nlohmann::json userJSON;

	// Get the JSON from the file
	try
	{
		userJSON = getJSONFromFile("users.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open user.json file");
		return recordSet;
	}

	// Check if user is trying to access his account
	if (username == "@me")
	{
		auto it = userJSON.cbegin();
		for (; it != userJSON.cend();)
		{
			if (it.value()["ID"] == userId)
			{
				it = userJSON.erase(it);

				if (!setJSONFile(userJSON, "users.json"))
				{
					recordSet.push_back("Could'n open user.json file");
				}

				return recordSet;
			}
			else
			{
				++it;
			}
		}

		// If the execution goes here, there should be smt very wrong
		recordSet.push_back("Could not find user with id: " + std::to_string(userId));
		return recordSet;
	}

	// Authtenticate user
	auto it = userJSON.cbegin();
	for (; it != userJSON.cend();)
	{
		if (toLowerCase(it.value()["Username"]) == toLowerCase(username))
		{
			it = userJSON.erase(it);

			if (!setJSONFile(userJSON, "users.json"))
			{
				recordSet.push_back("Could'n open user.json file");
			}

			return recordSet;
		}
		else
		{
			++it;
		}
	}

	recordSet.push_back("Could not find user with username: " + username);
	return recordSet;
}

std::vector<std::string> DBManager::updateUser(int userId, crow::query_string data)
{
	std::vector<std::string> recordSet;
	nlohmann::json userJSON;

	// Get the JSON from the file
	try
	{
		userJSON = getJSONFromFile("users.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open user.json file");
		return recordSet;
	}

	for (auto it = userJSON.begin(); it != userJSON.end(); ++it)
	{
		if (it.value()["ID"] == userId)
		{
			it.value()["FirstName"] = std::string(data.get("firstName")).empty() ? 
				it.value()["FirstName"] : data.get("firstName");

			it.value()["LastName"] = std::string(data.get("lastName")).empty() ? 
				it.value()["LastName"] : data.get("lastName");

			it.value()["Username"] = std::string(data.get("username")).empty() ? 
				it.value()["Username"] : data.get("username");

			it.value()["Email"] = std::string(data.get("email")).empty() ?
				it.value()["Email"] : data.get("email");

			it.value()["Password"] = std::string(data.get("password")).empty() ?
				it.value()["Password"] : data.get("password");

			if (!setJSONFile(userJSON, "users.json"))
			{
				recordSet.push_back("Could'n open user.json file");
			}

			return recordSet;
		}
	}

	// If the execution goes here, there should be smt very wrong
	recordSet.push_back("Could not find user with id: " + std::to_string(userId));
	return recordSet;
}

std::vector<std::string> DBManager::updateUserAvatar(int userId, std::string imageName)
{
	std::vector<std::string> recordSet;
	nlohmann::json userJSON;
	EnvManager envManager;

	// Get the JSON from the file
	try
	{
		userJSON = getJSONFromFile("users.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open user.json file");
		return recordSet;
	}

	for (auto it = userJSON.begin(); it != userJSON.end(); ++it)
	{
		if (it.value()["ID"] == userId)
		{
			it.value()["AvatarURL"] = envManager.getEnv("SERVER_URL") + "static/avatars/" + imageName;

			if (!setJSONFile(userJSON, "users.json"))
			{
				recordSet.push_back("Could'n open user.json file");
			}
			return recordSet;
		}
	}

	// If the execution goes here, there should be smt very wrong
	recordSet.push_back("Could not find user with id: " + std::to_string(userId));
	return recordSet;
}

std::vector<std::string> DBManager::createOrg(int userId, crow::query_string data, char* hash, char* salt)
{
	std::vector<std::string> recordSet;

	nlohmann::json orgsJSON;

	// Get the JSON from the file
	try
	{
		orgsJSON = getJSONFromFile("orgs.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open orgs.json file");
		return recordSet;
	}

	// Check for duplicate name
	if (checkIfValueExistsInField(orgsJSON, "Name", data.get("orgName")))
	{
		recordSet.push_back("There is already a organisation with this name: " + std::string(data.get("orgName")));
		return recordSet;
	}

	// Check if json is []
	if (orgsJSON.is_array())
	{
		if (orgsJSON.empty())
		{
			orgsJSON = nullptr;
		}
	}

	// Add the user to the JSON
	orgsJSON.push_back(
		{
		{ "ID", getLastId(orgsJSON) + 1},
		{ "Name", data.get("orgName") },
		{ "Password", hash },
		{ "Salt", salt }
		}
	);

	// Save the json to the file
	if (!setJSONFile(orgsJSON, "orgs.json"))
	{
		recordSet.push_back("Could'n open orgs.json file");
		return recordSet;
	}

	return recordSet;
}

std::vector<std::string> DBManager::addUserToOrganisation(
	int userId,
	int orgId,
	UserRolesInOrgs userRolesInOrgs,
	bool createNewEntry
)
{
	std::vector<std::string> recordSet;

	nlohmann::json userOrgRoleJSON;

	// Get the JSON from the file
	try
	{
		userOrgRoleJSON = getJSONFromFile("userOrgRole.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open userOrgRole.json file");
		return recordSet;
	}

	// Check if user is already in organisation
	for (auto it = userOrgRoleJSON.begin(); it != userOrgRoleJSON.end(); ++it)
	{
		if (it.value()["UserID"] == userId && it.value()["OrganisationID"] == orgId)
		{
			it.value()["Role"] = userRolesInOrgs;

			if (!setJSONFile(userOrgRoleJSON, "userOrgRole.json"))
			{
				recordSet.push_back("Could'n open userOrgRole.json file");
			}

			return recordSet;
		}
	}

	if (!createNewEntry)
	{
		recordSet.push_back("Couldn't find user with id: " + std::to_string(userId) +
			" or organisation with id: " + std::to_string(orgId));
		return recordSet;
	}

	// Check if json is []
	if (userOrgRoleJSON.is_array())
	{
		if (userOrgRoleJSON.empty())
		{
			userOrgRoleJSON = nullptr;
		}
	}

	// Add the user to the JSON
	userOrgRoleJSON.push_back(
		{
		{ "UserID", userId },
		{ "OrganisationID", orgId},
		{ "Role", userRolesInOrgs}
		}
	);

	// Save the json to the file
	if (!setJSONFile(userOrgRoleJSON, "userOrgRole.json"))
	{
		recordSet.push_back("Could'n open userOrgRole.json file");
		return recordSet;
	}

	return recordSet;
}

std::vector<std::string> DBManager::getOrgIdByName(std::string orgName)
{
	std::vector<std::string> recordSet;

	nlohmann::json orgsJSON;

	// Get the JSON from the file
	try
	{
		orgsJSON = getJSONFromFile("orgs.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open orgs.json file");
		return recordSet;
	}

	for (auto it = orgsJSON.begin(); it != orgsJSON.end(); ++it)
	{
		if (it.value()["Name"] == orgName)
		{
			recordSet.push_back(it.value()["ID"].dump());
			return recordSet;
		}
	}

	recordSet.push_back("Could not find organisation with name: " + orgName);
	return recordSet;
}

std::vector<std::string> DBManager::doesPasswordMatchOrg(std::string password, int orgId)
{
	std::vector<std::string> recordSet;

	nlohmann::json orgsJSON;

	// Get the JSON from the file
	try
	{
		orgsJSON = getJSONFromFile("orgs.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open orgs.json file");
		return recordSet;
	}

	for (auto it = orgsJSON.begin(); it != orgsJSON.end(); ++it)
	{
		if (it.value()["ID"] == orgId)
		{
			recordSet.push_back(std::to_string(bcrypt_checkpw(
				password.c_str(), std::string(it.value()["Password"]).c_str()) == 0)
			);

			return recordSet;
		}
	}

	recordSet.push_back("Could not find organisation with id: " + std::to_string(orgId));
	return recordSet;
}

std::vector<std::string> DBManager::isUserInOrgAndGetRole(int userId, int orgId)
{
	std::vector<std::string> recordSet;

	nlohmann::json userOrgRoleJSON;

	// Get the JSON from the file
	try
	{
		userOrgRoleJSON = getJSONFromFile("userOrgRole.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open userOrgRole.json file");
		return recordSet;
	}

	for (auto it = userOrgRoleJSON.begin(); it != userOrgRoleJSON.end(); ++it)
	{
		if (it.value()["UserID"] == userId && it.value()["OrganisationID"] == orgId)
		{
			recordSet.push_back(std::to_string(true));
			recordSet.push_back(it.value()["Role"].dump());
			return recordSet;
		}
	}

	recordSet.push_back("Could not find organisation with id: " + std::to_string(orgId) + 
		" or this user is not part of it");
	
	return recordSet;
}

std::vector<std::string> DBManager::deleteOrg(int orgId)
{
	std::vector<std::string> recordSet;
	nlohmann::json orgsJSON;
	nlohmann::json userOrgRoleJSON;
	nlohmann::json coursesJSON;

	// Get the JSON from the file
	try
	{
		orgsJSON = getJSONFromFile("orgs.json");
		userOrgRoleJSON = getJSONFromFile("userOrgRole.json");
		coursesJSON = getJSONFromFile("courses.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open orgs.json or userOrgRole.json file");
		return recordSet;
	}

	auto it = orgsJSON.cbegin();
	for (; it != orgsJSON.cend();)
	{
		if (it.value()["ID"] == orgId)
		{
			it = orgsJSON.erase(it);
			if (!setJSONFile(orgsJSON, "orgs.json"))
			{
				recordSet.push_back("Could'n open orgs.json file");
				return recordSet;
			}
		}
		else
		{
			++it;
		}
	}

	it = userOrgRoleJSON.cbegin();
	for (; it != userOrgRoleJSON.cend();)
	{
		if (it.value()["OrganisationID"] == orgId)
		{
			it = userOrgRoleJSON.erase(it);
		}
		else
		{
			++it;
		}
	}

	if (!setJSONFile(userOrgRoleJSON, "userOrgRole.json"))
	{
		recordSet.push_back("Could'n open userOrgRole.json file");
	}

	for (auto it = coursesJSON.begin(); it != coursesJSON.end(); ++it)
	{
		if (it.value()["OrgID"] == std::to_string(orgId))
		{
			deleteCourse(it.value()["ID"]);
		}
	}

	return recordSet;
}

std::vector<std::string> DBManager::updateOrg(int orgId, crow::query_string data)
{
	std::vector<std::string> recordSet;
	nlohmann::json orgJSON;

	// Get the JSON from the file
	try
	{
		orgJSON = getJSONFromFile("orgs.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open orgs.json file");
		return recordSet;
	}

	for (auto it = orgJSON.begin(); it != orgJSON.end(); ++it)
	{
		if (it.value()["ID"] == orgId)
		{
			it.value()["Name"] = std::string(data.get("orgName")).empty() ? 
				it.value()["Name"] : data.get("orgName");

			it.value()["Password"] = std::string(data.get("password")).empty() ? 
				it.value()["Password"] : data.get("password");

			if (!setJSONFile(orgJSON, "orgs.json"))
			{
				recordSet.push_back("Could'n open orgs.json file");
			}

			return recordSet;
		}
	}

	// If the execution goes here, there should be smt very wrong
	recordSet.push_back("Could not find org with id: " + std::to_string(orgId));
	return recordSet;
}

OrgInfo DBManager::getOrgInfo(std::string orgName)
{
	OrgInfo returnValue;

	// Please don't broke here
	returnValue.id = std::stoi(getOrgIdByName(orgName)[0]);
	returnValue.name = orgName;
	returnValue.courses = getAllCoursesInOrgWithID(returnValue.id);

	try
	{
		returnValue.users = getOrgUsersByOrgId(returnValue.id);
	}
	catch (std::string ex)
	{
		returnValue.errors.push_back(ex);
		return returnValue;
	}

	return returnValue;
}

std::vector<OrgInfo> DBManager::getAllOrgsInfo(int userId)
{
	std::vector<OrgInfo> recordSet;

	nlohmann::json orgsJSON;

	// Get the JSON from the file
	try
	{
		orgsJSON = getJSONFromFile("orgs.json");
	}
	catch (std::string ex)
	{
		recordSet[0].errors.push_back("Could'n open orgs.json file");
		return recordSet;
	}

	for (auto it = orgsJSON.begin(); it != orgsJSON.end(); ++it)
	{
		std::vector<std::string> recordSet2 = isUserInOrgAndGetRole(userId, it.value()["ID"]);

		if (recordSet2[0] == "1" && recordSet2[1] == "2")
		{
			recordSet.push_back({ it.value()["ID"], it.value()["Name"], { { 1 } } });
		}
		else
		{
			recordSet.push_back({ it.value()["ID"], it.value()["Name"], { { 0 } } });
		}

	}
	
	if (recordSet.empty())
	{
		OrgInfo error;
		error.errors.push_back("The organisation file is empty!");
		recordSet.push_back(error);
	}

	return recordSet;
}

std::vector<std::string> DBManager::createCourse(crow::query_string data, char* hash, char* salt)
{
	std::vector<std::string> recordSet;

	nlohmann::json coursesJSON;
	nlohmann::json orgsJSON;

	// Get the JSON from the file
	try
	{
		coursesJSON = getJSONFromFile("courses.json");
		orgsJSON = getJSONFromFile("orgs.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open courses.json or orgs.json file");
		return recordSet;
	}

	// Check for duplicate name
	if (checkIfValueExistsInField(coursesJSON, "Name", data.get("courseName"), "OrgID", data.get("orgId")))
	{
		recordSet.push_back("There is already a course with this name: " +
			std::string(data.get("courseName")));
		return recordSet;
	}

	if (checkIfValueExistsInField(orgsJSON, "OrgID", data.get("orgId")))
	{
		recordSet.push_back("There is not a organisation with this id: " +
			std::string(data.get("orgId")));
		return recordSet;
	}

	// Check if json is []
	if (coursesJSON.is_array())
	{
		if (coursesJSON.empty())
		{
			coursesJSON = nullptr;
		}
	}

	// Add the user to the JSON
	coursesJSON.push_back(
		{
		{ "ID", getLastId(coursesJSON) + 1},
		{ "Name", data.get("courseName") },
		{ "Password", hash },
		{ "OrgID", data.get("orgId") },
		{ "Salt", salt }
		}
	);

	// Save the json to the file
	if (!setJSONFile(coursesJSON, "courses.json"))
	{
		recordSet.push_back("Could'n open courses.json file");
		return recordSet;
	}

	return recordSet;
}

std::vector<std::string> DBManager::doesPasswordMatchCourse(std::string password, int courseId)
{
	std::vector<std::string> recordSet;

	nlohmann::json courseJSON;

	// Get the JSON from the file
	try
	{
		courseJSON = getJSONFromFile("courses.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open courses.json file");
		return recordSet;
	}

	for (auto it = courseJSON.begin(); it != courseJSON.end(); ++it)
	{
		if (it.value()["ID"] == courseId)
		{
			recordSet.push_back(
				std::to_string(bcrypt_checkpw(
					password.c_str(), std::string(it.value()["Password"]).c_str()
				) == 0)
			);
			return recordSet;
		}
	}

	recordSet.push_back("Could not find course with id: " + courseId);
	return recordSet;
}

std::vector<std::string> DBManager::addUserToCourse(
	int userId, 
	int courseId, 
	UserRolesInOrgs userRolesInOrgs, 
	bool createNewEntry
)
{
	std::vector<std::string> recordSet;

	nlohmann::json userCourseRoleJSON;

	// Get the JSON from the file
	try
	{
		userCourseRoleJSON = getJSONFromFile("userCourseRole.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open userCourseRole.json file");
		return recordSet;
	}

	// Check if user is already in organisation
	for (auto it = userCourseRoleJSON.begin(); it != userCourseRoleJSON.end(); ++it)
	{
		if (it.value()["UserID"] == userId && it.value()["CourseID"] == courseId)
		{
			it.value()["Role"] = userRolesInOrgs;

			if (!setJSONFile(userCourseRoleJSON, "userCourseRole.json"))
			{
				recordSet.push_back("Could'n open userCourseRole.json file");
			}

			return recordSet;
		}
	}

	if (!createNewEntry)
	{
		recordSet.push_back("Couldn't find user with id: " + std::to_string(userId) + 
			" or course with id: " + std::to_string(courseId));
		return recordSet;
	}

	// Check if json is []
	if (userCourseRoleJSON.is_array())
	{
		if (userCourseRoleJSON.empty())
		{
			userCourseRoleJSON = nullptr;
		}
	}

	// Add the user to the JSON
	userCourseRoleJSON.push_back(
		{
		{ "UserID", userId },
		{ "CourseID", courseId},
		{ "Role", userRolesInOrgs}
		}
	);

	// Save the json to the file
	if (!setJSONFile(userCourseRoleJSON, "userCourseRole.json"))
	{
		recordSet.push_back("Could'n open userCourseRole.json file");
		return recordSet;
	}

	return recordSet;
}

CourseInfo DBManager::getCourseInfo(int courseId)
{
	CourseInfo returnValue;

	returnValue.id = courseId;
	if (getFieldDataInJSONByCriteria("courses.json", courseId, "ID", "Name").empty())
	{
		returnValue.errors.push_back("Can not find course with id: " + std::to_string(courseId));
		return returnValue;
	}

	returnValue.name = getFieldDataInJSONByCriteria("courses.json", courseId, "ID", "Name")[0];
	returnValue.orgId = std::stoi(getFieldDataInJSONByCriteria("courses.json", returnValue.id, "ID", "OrgID")[0]);
	returnValue.topics = getAllTopicsInCourseWithID(courseId);

	try
	{
		returnValue.users = getCourseUsersByCourseId(returnValue.id);
	}
	catch (std::string ex)
	{
		returnValue.errors.push_back(ex);
		return returnValue;
	}

	return returnValue;
}

std::vector<std::string> DBManager::updateCourse(int courseId, crow::query_string data)
{
	std::vector<std::string> recordSet;
	nlohmann::json coursesJSON;

	// Get the JSON from the file
	try
	{
		coursesJSON = getJSONFromFile("courses.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open courses.json file");
		return recordSet;
	}

	for (auto it = coursesJSON.begin(); it != coursesJSON.end(); ++it)
	{
		if (it.value()["ID"] == courseId)
		{
			it.value()["Name"] = std::string(data.get("courseName")).empty() ? 
				it.value()["Name"] : data.get("courseName");

			it.value()["Password"] = std::string(data.get("password")).empty() ? 
				it.value()["Password"] : data.get("password");

			if (!setJSONFile(coursesJSON, "courses.json"))
			{
				recordSet.push_back("Could'n open courses.json file");
			}

			return recordSet;
		}
	}

	// If the execution goes here, there should be smt very wrong
	recordSet.push_back("Could not find course with id: " + std::to_string(courseId));
	return recordSet;
}

std::vector<std::string> DBManager::canUserAccessCourse(
	int courseId, 
	int userId, 
	bool allowOnlyTeachersAndAdmins
)
{
	std::vector<std::string> returnVal;

	CourseInfo courseInfo = getCourseInfo(courseId);

	if (!courseInfo.errors.empty())
	{
		std::string log = "Failed to get course info for course with name: " +
			std::to_string(courseId) + ". Reasons: ";

		for (auto el : courseInfo.errors)
		{
			returnVal.push_back(el + " ");
		}

		CROW_LOG_WARNING << log;

		return returnVal;
	}

	std::vector<std::string> recordSet = isUserInOrgAndGetRole(userId, courseInfo.orgId);

	if (allowOnlyTeachersAndAdmins)
	{
		if (recordSet[0] != "1" || recordSet[1] == "0")
		{
			std::string log = "Failed to get course info with id: " + 
				std::to_string(courseId) + ". Reason: User is unauthorised";
			returnVal.push_back("User is unauthorised");

			CROW_LOG_WARNING << log;

			return returnVal;
		}
	}
	else
	{
		if (recordSet[0] != "1")
		{
			std::string log = "Failed to get course info with id: " + 
				std::to_string(courseId) + ". Reason: User is unauthorised";
			returnVal.push_back("User is unauthorised");

			CROW_LOG_WARNING << log;

			return returnVal;
		}
	}

	std::vector<OrgUser> userInCourse = getCourseUsersByCourseId(courseId);

	bool isUserAuthorised = false;

	if (recordSet[1] == "2")
	{
		isUserAuthorised = true;
	}

	for (auto& el : userInCourse)
	{
		if (allowOnlyTeachersAndAdmins)
		{
			if (el.id == userId && el.role == 1)
			{
				isUserAuthorised = true;
			}
		}
		else
		{
			if (el.id == userId)
			{
				isUserAuthorised = true;
			}
		}
	}

	returnVal.push_back(std::to_string(isUserAuthorised));

	return returnVal;
}

std::vector<std::string> DBManager::createTopic(crow::query_string data)
{
	std::vector<std::string> recordSet;

	nlohmann::json topicsJSON;

	// Get the JSON from the file
	try
	{
		topicsJSON = getJSONFromFile("topics.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open topics.json file");
		return recordSet;
	}

	// Check for duplicate name
	if (checkIfValueExistsInField(topicsJSON, "Name", data.get("topicName"), "CourseId", data.get("courseId")))
	{
		recordSet.push_back("There is already a topic with this name: " + std::string(data.get("topicName")));
		return recordSet;
	}

	// Check if json is []
	if (topicsJSON.is_array())
	{
		if (topicsJSON.empty())
		{
			topicsJSON = nullptr;
		}
	}

	// Add the user to the JSON
	topicsJSON.push_back(
		{
		{ "ID", getLastId(topicsJSON) + 1},
		{ "Name", data.get("topicName") },
		{ "CourseID", data.get("courseId") }
		}
	);

	// Save the json to the file
	if (!setJSONFile(topicsJSON, "topics.json"))
	{
		recordSet.push_back("Could'n open topics.json file");
		return recordSet;
	}

	return recordSet;
}

TopicInfo DBManager::getTopicInfo(int topicId)
{
	TopicInfo returnValue;

	returnValue.id = topicId;

	if (getFieldDataInJSONByCriteria("topics.json", topicId, "ID", "Name").empty())
	{
		returnValue.errors.push_back("Can not find topic with id: " + std::to_string(topicId));
		return returnValue;
	}

	returnValue.name = getFieldDataInJSONByCriteria("topics.json", topicId, "ID", "Name")[0];
	returnValue.courseId = std::stoi(getFieldDataInJSONByCriteria("topics.json", topicId, "ID", "CourseID")[0]);
	returnValue.lessons = getAllLessonInTopicWithID(topicId);
	returnValue.quizzes = getAllQuizzesInTopicWithID(topicId);

	return returnValue;
}

/*
std::vector<std::string> DBManager::getCourseIdByName(std::string courseName)
{
	std::vector<std::string> recordSet;

	nlohmann::json coursesJSON;

	// Get the JSON from the file
	try
	{
		coursesJSON = getJSONFromFile("courses.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open courses.json file");
		return recordSet;
	}

	for (auto it = coursesJSON.begin(); it != coursesJSON.end(); ++it)
	{
		if (it.value()["Name"] == courseName)
		{
			recordSet.push_back(it.value()["ID"].dump());
			return recordSet;
		}
	}

	recordSet.push_back("Could not find course with name: " + courseName);
	return recordSet;
}
*/

std::vector<std::string> DBManager::isUserInCourseAndGetRole(int userId, int courseId)
{
	std::vector<std::string> recordSet;

	nlohmann::json userCourseRoleJSON;

	// Get the JSON from the file
	try
	{
		userCourseRoleJSON = getJSONFromFile("userCourseRole.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open userCourseRole.json file");
		return recordSet;
	}

	for (auto it = userCourseRoleJSON.begin(); it != userCourseRoleJSON.end(); ++it)
	{
		if (it.value()["UserID"] == userId && it.value()["CourseID"] == courseId)
		{
			recordSet.push_back(std::to_string(true));
			recordSet.push_back(it.value()["Role"].dump());
			return recordSet;
		}
	}

	recordSet.push_back("Could not find course with id: " + std::to_string(courseId));
	return recordSet;
}

std::vector<CourseInfo> DBManager::getAllCoursesInOrgWithID(int orgId)
{
	std::vector<CourseInfo> recordSet;

	nlohmann::json courseJSON;

	// Get the JSON from the file
	try
	{
		courseJSON = getJSONFromFile("courses.json");
	}
	catch (std::string ex)
	{
		recordSet[0].errors.push_back("Could'n open courses.json file");
		return recordSet;
	}

	for (auto it = courseJSON.begin(); it != courseJSON.end(); ++it)
	{
		if (it.value()["OrgID"] == std::to_string(orgId))
		{
			recordSet.push_back({ it.value()["ID"], orgId,  it.value()["Name"] });
		}
	}

	return recordSet;
}

std::vector<AttemptInfo> DBManager::getAllAttemptsInQuizWithId(int quizId)
{
	std::vector<AttemptInfo> recordSet;

	nlohmann::json attemptsJSON;

	// Get the JSON from the file
	try
	{
		attemptsJSON = getJSONFromFile("attempts.json");
	}
	catch (std::string ex)
	{
		throw "Could'n open attempts.json file";
	}

	for (auto it = attemptsJSON.begin(); it != attemptsJSON.end(); ++it)
	{
		if (it.value()["QuizID"] == std::to_string(quizId))
		{
			recordSet.push_back({ it.value()["ID"] });
		}
	}

	return recordSet;
}

float DBManager::calculateScoreForAttempt(int attemptId)
{
	float score = 0;
	nlohmann::json answersJSON;

	// Get the JSON from the file
	try
	{
		answersJSON = getJSONFromFile("answers.json");
	}
	catch (std::string ex)
	{
		throw "Could'n open questions.json file";
	}

	for (auto it = answersJSON.begin(); it != answersJSON.end(); ++it)
	{
		std::string type = it.value()["Type"];
		if (type == "2")
		{
			if (it.value()["AttemptID"] == std::to_string(attemptId))
			{
				score += it.value()["Score"];
			}
		}
		else
		{
			if (it.value()["AttemptID"] == std::to_string(attemptId) && it.value()["IsCorrect"])
			{
				score += 1;
			}
		}
	}

	return score;
}

std::vector<QuestionInfo> DBManager::getAllQuestionsInQuizWithId(int quizId)
{
	std::vector<QuestionInfo> recordSet;

	nlohmann::json questionsJSON;

	// Get the JSON from the file
	try
	{
		questionsJSON = getJSONFromFile("questions.json");
	}
	catch (std::string ex)
	{
		throw "Could'n open questions.json file";
	}

	for (auto it = questionsJSON.begin(); it != questionsJSON.end(); ++it)
	{
		if (it.value()["QuizID"] == std::to_string(quizId))
		{
			recordSet.push_back({ it.value()["ID"] });
		}
	}

	return recordSet;
}

std::vector<std::string> DBManager::deleteCourse(int courseId)
{
	std::vector<std::string> recordSet;
	nlohmann::json coursesJSON;
	nlohmann::json userCoursesRoleJSON;
	nlohmann::json topicsJSON;

	// Get the JSON from the file
	try
	{
		coursesJSON = getJSONFromFile("courses.json");
		userCoursesRoleJSON = getJSONFromFile("userCourseRole.json");
		topicsJSON = getJSONFromFile("topics.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open courses.json or userCourseRole.json file");
		return recordSet;
	}

	auto it = coursesJSON.cbegin();
	for (; it != coursesJSON.cend();)
	{
		if (it.value()["ID"] == courseId)
		{
			it = coursesJSON.erase(it);
			if (!setJSONFile(coursesJSON, "courses.json"))
			{
				recordSet.push_back("Could'n open courses.json file");
				return recordSet;
			}
		}
		else
		{
			++it;
		}
	}

	it = userCoursesRoleJSON.cbegin();
	for (; it != userCoursesRoleJSON.cend();)
	{
		if (it.value()["CourseID"] == courseId)
		{
			it = userCoursesRoleJSON.erase(it);
		}
		else
		{
			++it;
		}
	}

	if (!setJSONFile(userCoursesRoleJSON, "userCourseRole.json"))
	{
		recordSet.push_back("Could'n open userCourseRole.json file");
	}

	for (auto it = topicsJSON.begin(); it != topicsJSON.end(); ++it)
	{
		if (it.value()["CourseID"] == std::to_string(courseId))
		{
			deleteTopic(it.value()["ID"]);
		}
	}

	return recordSet;
}

std::vector<TopicInfo> DBManager::getAllTopicsInCourseWithID(int courseId)
{
	std::vector<TopicInfo> recordSet;

	nlohmann::json topicsJSON;

	// Get the JSON from the file
	try
	{
		topicsJSON = getJSONFromFile("topics.json");
	}
	catch (std::string ex)
	{
		recordSet[0].errors.push_back("Could'n open topics.json file");
		return recordSet;
	}

	for (auto it = topicsJSON.begin(); it != topicsJSON.end(); ++it)
	{
		if (it.value()["CourseID"] == std::to_string(courseId))
		{
			recordSet.push_back({ it.value()["ID"], courseId,  it.value()["Name"] });
		}
	}

	return recordSet;
}

std::vector<std::string> DBManager::deleteTopic(int topicId)
{
	std::vector<std::string> recordSet;
	nlohmann::json topicsJSON;
	nlohmann::json lessonsJSON;
	nlohmann::json quizzesJSON;

	// Get the JSON from the file
	try
	{
		topicsJSON = getJSONFromFile("topics.json");
		lessonsJSON = getJSONFromFile("lessons.json");
		quizzesJSON = getJSONFromFile("quizzes.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open topics.json file");
		return recordSet;
	}

	auto it = topicsJSON.cbegin();
	for (; it != topicsJSON.cend();)
	{
		if (it.value()["ID"] == topicId)
		{
			it = topicsJSON.erase(it);
			if (!setJSONFile(topicsJSON, "topics.json"))
			{
				recordSet.push_back("Could'n open topics.json file");
				return recordSet;
			}
		}
		else
		{
			++it;
		}
	}

	for (auto it = lessonsJSON.begin(); it != lessonsJSON.end(); ++it)
	{
		if (it.value()["TopicID"] == std::to_string(topicId))
		{
			deleteLesson(it.value()["ID"]);
		}
	}

	for (auto it = quizzesJSON.begin(); it != quizzesJSON.end(); ++it)
	{
		if (it.value()["TopicID"] == std::to_string(topicId))
		{
			deleteQuiz(it.value()["ID"]);
		}
	}

	return recordSet;
}

std::vector<std::string> DBManager::updateTopic(int topicId, crow::query_string data)
{
	std::vector<std::string> recordSet;
	nlohmann::json topicsJSON;

	// Get the JSON from the file
	try
	{
		topicsJSON = getJSONFromFile("topics.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open topics.json file");
		return recordSet;
	}

	for (auto it = topicsJSON.begin(); it != topicsJSON.end(); ++it)
	{
		if (it.value()["ID"] == topicId)
		{
			it.value()["Name"] = std::string(data.get("topicName")).empty() ? 
				it.value()["Name"] : data.get("topicName");

			if (!setJSONFile(topicsJSON, "topics.json"))
			{
				recordSet.push_back("Could'n open topics.json file");
			}

			return recordSet;
		}
	}

	// If the execution goes here, there should be smt very wrong
	recordSet.push_back("Could not find topic with id: " + std::to_string(topicId));
	return recordSet;
}

std::vector<std::string> DBManager::createLesson(crow::query_string data)
{
	std::vector<std::string> recordSet;

	nlohmann::json lessonsJSON;

	// Get the JSON from the file
	try
	{
		lessonsJSON = getJSONFromFile("lessons.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open lessons.json file");
		return recordSet;
	}

	// Check for duplicate name
	if (checkIfValueExistsInField(lessonsJSON, "Name", data.get("lessonName"), "TopicID", data.get("topicId")))
	{
		recordSet.push_back("There is already a lesson with this name: " + std::string(data.get("lessonName")));
		return recordSet;
	}

	// Check if json is []
	if (lessonsJSON.is_array())
	{
		if (lessonsJSON.empty())
		{
			lessonsJSON = nullptr;
		}
	}

	// Add the user to the JSON
	lessonsJSON.push_back(
		{
		{ "ID", getLastId(lessonsJSON) + 1},
		{ "Name", data.get("lessonName") },
		{ "Data", data.get("lessonData") },
		{ "TopicID", data.get("topicId") }
		}
	);

	// Save the json to the file
	if (!setJSONFile(lessonsJSON, "lessons.json"))
	{
		recordSet.push_back("Could'n open lessons.json file");
		return recordSet;
	}

	return recordSet;
}

LessonInfo DBManager::getLessonInfo(int lessonId)
{
	LessonInfo returnValue;

	returnValue.id = lessonId;

	if (getFieldDataInJSONByCriteria("lessons.json", lessonId, "ID", "Name").empty())
	{
		returnValue.errors.push_back("Can not find lesson with id: " + std::to_string(lessonId));
		return returnValue;
	}

	returnValue.name = getFieldDataInJSONByCriteria("lessons.json", lessonId, "ID", "Name")[0];
	returnValue.topicId = std::stoi(getFieldDataInJSONByCriteria("lessons.json", lessonId, "ID", "TopicID")[0]);
	returnValue.data = getFieldDataInJSONByCriteria("lessons.json", lessonId, "ID", "Data")[0];

	return returnValue;
}

std::vector<LessonInfo> DBManager::getAllLessonInTopicWithID(int topicId)
{
	std::vector<LessonInfo> recordSet;

	nlohmann::json lessonsJSON;

	// Get the JSON from the file
	try
	{
		lessonsJSON = getJSONFromFile("lessons.json");
	}
	catch (std::string ex)
	{
		recordSet[0].errors.push_back("Could'n open lessons.json file");
		return recordSet;
	}

	for (auto it = lessonsJSON.begin(); it != lessonsJSON.end(); ++it)
	{
		if (it.value()["TopicID"] == std::to_string(topicId))
		{
			recordSet.push_back({ it.value()["ID"], topicId,  it.value()["Name"] });
		}
	}

	return recordSet;
}


std::vector<std::string> DBManager::deleteLesson(int lessonId)
{
	std::vector<std::string> recordSet;
	nlohmann::json lessonsJSON;

	// Get the JSON from the file
	try
	{
		lessonsJSON = getJSONFromFile("lessons.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open lessons.json file");
		return recordSet;
	}

	auto it = lessonsJSON.cbegin();
	for (; it != lessonsJSON.cend();)
	{
		if (it.value()["ID"] == lessonId)
		{
			it = lessonsJSON.erase(it);
			if (!setJSONFile(lessonsJSON, "lessons.json"))
			{
				recordSet.push_back("Could'n open lessons.json file");
				return recordSet;
			}
		}
		else
		{
			++it;
		}
	}

	return recordSet;
}


std::vector<std::string> DBManager::updateLesson(int lessonId, crow::query_string data)
{
	std::vector<std::string> recordSet;
	nlohmann::json lessonsJSON;

	// Get the JSON from the file
	try
	{
		lessonsJSON = getJSONFromFile("lessons.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open lessons.json file");
		return recordSet;
	}

	for (auto it = lessonsJSON.begin(); it != lessonsJSON.end(); ++it)
	{
		if (it.value()["ID"] == lessonId)
		{
			it.value()["Name"] = std::string(data.get("lessonName")).empty() ? 
				it.value()["Name"] : data.get("lessonName");

			it.value()["Data"] = std::string(data.get("lessonData")).empty() ? 
				it.value()["Data"] : data.get("lessonData");

			if (!setJSONFile(lessonsJSON, "lessons.json"))
			{
				recordSet.push_back("Could'n open lessons.json file");
			}

			return recordSet;
		}
	}

	// If the execution goes here, there should be smt very wrong
	recordSet.push_back("Could not find lessons with id: " + std::to_string(lessonId));
	return recordSet;
}

std::vector<std::string> DBManager::createQuiz(crow::query_string data)
{
	std::vector<std::string> recordSet;

	nlohmann::json quizzesJSON;

	// Get the JSON from the file
	try
	{
		quizzesJSON = getJSONFromFile("quizzes.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open quizzes.json file");
		return recordSet;
	}

	// Check for duplicate name
	if (checkIfValueExistsInField(quizzesJSON, "Name", data.get("quizName"), "TopicID", data.get("topicId")))
	{
		recordSet.push_back("There is already a quiz with this name: " + std::string(data.get("quizName")));
		return recordSet;
	}

	// Check if json is []
	if (quizzesJSON.is_array())
	{
		if (quizzesJSON.empty())
		{
			quizzesJSON = nullptr;
		}
	}

	// Add the user to the JSON
	quizzesJSON.push_back(
		{
		{ "ID", getLastId(quizzesJSON) + 1},
		{ "Name", data.get("quizName") },
		{ "TopicID", data.get("topicId") }
		}
	);

	// Save the json to the file
	if (!setJSONFile(quizzesJSON, "quizzes.json"))
	{
		recordSet.push_back("Could'n open quizzes.json file");
		return recordSet;
	}

	return recordSet;
}

QuizInfo DBManager::getQuizInfo(int quizId)
{
	QuizInfo returnValue;

	returnValue.id = quizId;

	if (getFieldDataInJSONByCriteria("quizzes.json", quizId, "ID", "Name").empty())
	{
		returnValue.errors.push_back("Can not find quiz with id: " + std::to_string(quizId));
		return returnValue;
	}

	returnValue.name = getFieldDataInJSONByCriteria("quizzes.json", quizId, "ID", "Name")[0];
	returnValue.topicId = std::stoi(getFieldDataInJSONByCriteria("quizzes.json", quizId, "ID", "TopicID")[0]);
	returnValue.questions = getAllQuestionsInQuizWithId(quizId);
	returnValue.attempts = getAllAttemptsInQuizWithId(quizId);

	return returnValue;
}

std::vector<std::string> DBManager::deleteQuiz(int quizId)
{
	std::vector<std::string> recordSet;
	nlohmann::json quizzesJSON;
	nlohmann::json questionsJSON;


	// Get the JSON from the file
	try
	{
		quizzesJSON = getJSONFromFile("quizzes.json");
		questionsJSON = getJSONFromFile("questions.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open quizzes.json or questions.json file");
		return recordSet;
	}

	auto it = quizzesJSON.cbegin();
	for (; it != quizzesJSON.cend();)
	{
		if (it.value()["ID"] == quizId)
		{
			it = quizzesJSON.erase(it);
			if (!setJSONFile(quizzesJSON, "quizzes.json"))
			{
				recordSet.push_back("Could'n open quizzes.json file");
				return recordSet;
			}
		}
		else
		{
			++it;
		}
	}

	for (auto it = questionsJSON.begin(); it != questionsJSON.end(); ++it)
	{
		if (it.value()["QuizID"] == std::to_string(quizId))
		{
			deleteQuestion(it.value()["ID"]);
		}
	}

	return recordSet;
}

std::vector<std::string> DBManager::updateQuiz(int quizId, crow::query_string data)
{
	std::vector<std::string> recordSet;
	nlohmann::json quizzesJSON;
	nlohmann::json attemptsJSON;

	// Get the JSON from the file
	try
	{
		quizzesJSON = getJSONFromFile("quizzes.json");
		attemptsJSON = getJSONFromFile("attempts.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open quizzes.json file");
		return recordSet;
	}

	for (auto it = quizzesJSON.begin(); it != quizzesJSON.end(); ++it)
	{
		if (it.value()["ID"] == quizId)
		{
			it.value()["Name"] = std::string(data.get("quizName")).empty() ?
				it.value()["Name"] : data.get("quizName");

			if (!setJSONFile(quizzesJSON, "quizzes.json"))
			{
				recordSet.push_back("Could'n open quizzes.json file");
			}

			return recordSet;
		}
	}

	// If the execution goes here, there should be smt very wrong
	recordSet.push_back("Could not find quiz with id: " + std::to_string(quizId));
	return recordSet;
}

std::vector<std::string> DBManager::createQuestion(crow::query_string data)
{
	std::vector<std::string> recordSet;

	nlohmann::json questionsJSON;
	nlohmann::json attemptsJSON;

	// Get the JSON from the file
	try
	{
		questionsJSON = getJSONFromFile("questions.json");
		attemptsJSON = getJSONFromFile("attempts.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open questions.json file");
		return recordSet;
	}

	std::string question;

	std::string type = data.get("type");

	if (type == "2")
	{
		question = "Fill in the punnett square if the parents are " + 
			std::string(data.get("P1")) + " and " + std::string(data.get("P2"));
	}
	else
	{
		question = data.get("question");
	}

	// Check for duplicate name
	if (checkIfValueExistsInField(questionsJSON, "Question", question, "QuizID", data.get("quizId")))
	{
		recordSet.push_back("There is already the same question in this quiz");
		return recordSet;
	}

	// Check if json is []
	if (questionsJSON.is_array())
	{
		if (questionsJSON.empty())
		{
			questionsJSON = nullptr;
		}
	}

	if (type == "2")
	{
		std::vector<std::string> answers = convertP1andP2toAnswers(data.get("P1"), data.get("P2"));

		// Add the user to the JSON
		questionsJSON.push_back(
			{
			{ "ID", getLastId(questionsJSON) + 1},
			{ "Type", type },
			{ "QuizID", data.get("quizId") },
			{ "Question", question },
			{ "Cell0x0", answers[0] },
			{ "Cell0x1", answers[1] },
			{ "Cell0x2", answers[2] },
			{ "Cell0x3", answers[3] },
			{ "Cell1x0", answers[4] },
			{ "Cell1x1", answers[5] },
			{ "Cell1x2", answers[6] },
			{ "Cell1x3", answers[7] },
			{ "Cell2x0", answers[8] },
			{ "Cell2x1", answers[9] },
			{ "Cell2x2", answers[10] },
			{ "Cell2x3", answers[11] },
			{ "Cell3x0", answers[12] },
			{ "Cell3x1", answers[13] },
			{ "Cell3x2", answers[14] },
			{ "Cell3x3", answers[15] },
			}
		);
	}
	else
	{
		// Add the user to the JSON
		questionsJSON.push_back(
			{
			{ "ID", getLastId(questionsJSON) + 1},
			{ "Type", data.get("type")},
			{ "Question", question },
			{ "Answer", data.get("answer") },
			{ "QuizID", data.get("quizId") }
			}
		);
	}


	// Delete the attempts before these change
	for (auto it = attemptsJSON.begin(); it != attemptsJSON.end(); ++it)
	{
		if (it.value()["QuizID"] == data.get("quizId"))
		{
			deleteAttempt(it.value()["ID"]);
		}
	}

	// Save the json to the file
	if (!setJSONFile(questionsJSON, "questions.json"))
	{
		recordSet.push_back("Could'n open questions.json file");
		return recordSet;
	}

	return recordSet;
}

QuestionInfo DBManager::getQuestionInfo(int questionId)
{
	QuestionInfo returnValue;

	returnValue.id = questionId;

	if (getFieldDataInJSONByCriteria("questions.json", questionId, "ID", "Question").empty())
	{
		returnValue.errors.push_back("Can not find question with id: " + std::to_string(questionId));
		return returnValue;
	}

	returnValue.question = getFieldDataInJSONByCriteria("questions.json", questionId, "ID", "Question")[0];
	returnValue.type = getFieldDataInJSONByCriteria("questions.json", questionId, "ID", "Type")[0];

	if (returnValue.type == "2")
	{
		returnValue.punnettAnswer = getPunnetSquareAnswer(questionId);
	}
	else
	{
		returnValue.answer = getFieldDataInJSONByCriteria("questions.json", questionId, "ID", "Answer")[0];
	}

	returnValue.quizId = std::stoi(getFieldDataInJSONByCriteria("questions.json", questionId, "ID", "QuizID")[0]);

	return returnValue;
}

std::vector<std::string> DBManager::deleteQuestion(int questionId)
{
	std::vector<std::string> recordSet;
	nlohmann::json questionsJSON;
	nlohmann::json attemptsJSON;

	// Get the JSON from the file
	try
	{
		questionsJSON = getJSONFromFile("questions.json");
		attemptsJSON = getJSONFromFile("attempts.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open questions.json file");
		return recordSet;
	}

	auto it = questionsJSON.cbegin();
	for (; it != questionsJSON.cend();)
	{
		if (it.value()["ID"] == questionId)
		{
			std::string quizId = it.value()["QuizID"];

			auto itAttempt = attemptsJSON.cbegin();
			for (; itAttempt != attemptsJSON.cend();)
			{
				if (itAttempt.value()["QuizID"] == quizId)
				{
					deleteAttempt(itAttempt.value()["ID"]);
					++itAttempt;
				}
				else
				{
					++itAttempt;
				}
			}

			it = questionsJSON.erase(it);

			if (!setJSONFile(questionsJSON, "questions.json"))
			{
				recordSet.push_back("Could'n open questions.json file");
				return recordSet;
			}
		}
		else
		{
			++it;
		}
	}

	return recordSet;
}

std::vector<std::string> DBManager::updateQuestion(int questionId, crow::query_string data)
{
	std::vector<std::string> recordSet;
	nlohmann::json questionsJSON;
	nlohmann::json attemptsJSON;

	// Get the JSON from the file
	try
	{
		questionsJSON = getJSONFromFile("questions.json");
		attemptsJSON = getJSONFromFile("attempts.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open questions.json file");
		return recordSet;
	}

	for (auto it = questionsJSON.begin(); it != questionsJSON.end(); ++it)
	{
		if (it.value()["ID"] == questionId)
		{
			for (auto itAttempt = attemptsJSON.begin(); itAttempt != attemptsJSON.end(); ++itAttempt)
			{
				if (itAttempt.value()["QuizID"] == it.value()["QuizID"])
				{
					deleteAttempt(itAttempt.value()["ID"]);
				}
			}
			std::string type = it.value()["Type"];

			if (type == "2")
			{
				std::vector<std::string> answers = convertP1andP2toAnswers(
					std::string(data.get("P1")).empty() ? it.value()["P1"] : data.get("P1"),
					std::string(data.get("P2")).empty() ? it.value()["P2"] : data.get("P2")
				);

				it.value()["Question"] = "Fill in the punnett square if the parents are " +
					std::string(data.get("P1")) + " and " + std::string(data.get("P2"));
				it.value()["Cell0x0"] = answers[0];
				it.value()["Cell0x1"] = answers[1];
				it.value()["Cell0x2"] = answers[2];
				it.value()["Cell0x3"] = answers[3];
				it.value()["Cell1x0"] = answers[4];
				it.value()["Cell1x1"] = answers[5];
				it.value()["Cell1x2"] = answers[6];
				it.value()["Cell1x3"] = answers[7];
				it.value()["Cell2x0"] = answers[8];
				it.value()["Cell2x1"] = answers[9];
				it.value()["Cell2x2"] = answers[10];
				it.value()["Cell2x3"] = answers[11];
				it.value()["Cell3x0"] = answers[12];
				it.value()["Cell3x1"] = answers[13];
				it.value()["Cell3x2"] = answers[14];
				it.value()["Cell3x3"] = answers[15];
			}
			else
			{
				it.value()["Question"] = std::string(data.get("question")).empty() ? 
					it.value()["Question"] : data.get("question");

				it.value()["Answer"] = std::string(data.get("answer")).empty() ?
					it.value()["Answer"] : data.get("answer");
			}

			if (!setJSONFile(questionsJSON, "questions.json"))
			{
				recordSet.push_back("Could'n open questions.json file");
			}

			return recordSet;
		}
	}

	// If the execution goes here, there should be smt very wrong
	recordSet.push_back("Could not find question with id: " + std::to_string(questionId));
	return recordSet;
}

std::vector<std::string> DBManager::startAttempt(crow::query_string data, int userId)
{
	std::vector<std::string> recordSet;

	nlohmann::json attemptsJSON;

	// Get the JSON from the file
	try
	{
		attemptsJSON = getJSONFromFile("attempts.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open attempts.json file");
		return recordSet;
	}

	// Check for duplicate name
	if (checkIfValueExistsInField(attemptsJSON, "QuizID", data.get("quizId"), "UserID", userId))
	{
		QuizInfo quizInfo = getQuizInfo(std::stoi(data.get("quizId")));
		recordSet.push_back("Attempt-id: " + std::to_string(quizInfo.id));
		return recordSet;
	}

	// Check if json is []
	if (attemptsJSON.is_array())
	{
		if (attemptsJSON.empty())
		{
			attemptsJSON = nullptr;
		}
	}

	time_t currentTime = time(0);
	QuizInfo quizInfo = getQuizInfo(std::stoi(data.get("quizId")));

	// Add the user to the JSON
	attemptsJSON.push_back(
		{
		{ "ID", getLastId(attemptsJSON) + 1},
		{ "TimeStart", currentTime },
		{ "TimeEnd", NULL },
		{ "CurrentQuestionID", quizInfo.questions[0].id },
		{ "QuizID", data.get("quizId") },
		{ "UserID", userId },
		{ "Score", NULL },
		{ "inProgress", true}
		}
	);

	// Save the json to the file
	if (!setJSONFile(attemptsJSON, "attempts.json"))
	{
		recordSet.push_back("Could'n open attempts.json file");
		return recordSet;
	}


	recordSet.push_back("Attempt-id: " + std::to_string(getLastId(attemptsJSON)));
	return recordSet;
}

AttemptInfo DBManager::getAttemptInfo(int attemptId)
{
	AttemptInfo returnValue;

	returnValue.id = attemptId;

	if (getFieldDataInJSONByCriteria("attempts.json", attemptId, "ID", "CurrentQuestionID").empty())
	{
		returnValue.errors.push_back("Can not find attempt with id: " + std::to_string(attemptId));
		return returnValue;
	}

	returnValue.currentQuestionId = std::stoi(getFieldDataInJSONByCriteria(
		"attempts.json", attemptId, "ID", "CurrentQuestionID")[0]
	);

	returnValue.quizId = std::stoi(getFieldDataInJSONByCriteria(
		"attempts.json", attemptId, "ID", "QuizID")[0]
	);

	returnValue.score = std::stof(getFieldDataInJSONByCriteria(
		"attempts.json", attemptId, "ID", "Score")[0]
	);

	returnValue.timeStart = std::stoi(getFieldDataInJSONByCriteria(
		"attempts.json", attemptId, "ID", "TimeStart")[0]
	);

	returnValue.timeEnd = std::stoi(getFieldDataInJSONByCriteria(
		"attempts.json", attemptId, "ID", "TimeEnd")[0]
	);

	returnValue.userId = std::stoi(getFieldDataInJSONByCriteria(
		"attempts.json", attemptId, "ID", "UserID")[0]
	);
	
	returnValue.inProgress = std::stoi(getFieldDataInJSONByCriteria(
		"attempts.json", attemptId, "ID", "inProgress")[0]
	);

	returnValue.answers = getAllAnswersInAttemptWithId(attemptId);

	return returnValue;
}

int DBManager::getAttemptIdByUserIdAndQuizId(int userId, int quizId)
{
	nlohmann::json attemptsJSON;

	// Get the JSON from the file
	try
	{
		attemptsJSON = getJSONFromFile("attempts.json");
	}
	catch (std::string ex)
	{
		return -1;
	}

	for (auto it = attemptsJSON.begin(); it != attemptsJSON.end(); ++it)
	{
		if (it.value()["QuizID"] == std::to_string(quizId) && it.value()["UserID"] == userId)
		{
			return it.value()["ID"];
		}
	}

	return -1;
}

std::vector<std::string> DBManager::deleteAttempt(int attemptId)
{
	std::vector<std::string> recordSet;
	nlohmann::json attemptsJSON;
	nlohmann::json answersJSON;

	// Get the JSON from the file
	try
	{
		attemptsJSON = getJSONFromFile("attempts.json");
		answersJSON = getJSONFromFile("answers.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open attempts.json file");
		return recordSet;
	}

	auto it = attemptsJSON.cbegin();
	for (; it != attemptsJSON.cend();)
	{
		if (it.value()["ID"] == attemptId)
		{
			it = attemptsJSON.erase(it);
			if (!setJSONFile(attemptsJSON, "attempts.json"))
			{
				recordSet.push_back("Could'n open attempts.json file");
				return recordSet;
			}
		}
		else
		{
			++it;
		}
	}

	for (auto it = answersJSON.begin(); it != answersJSON.end(); ++it)
	{
		if (it.value()["AttemptID"] == std::to_string(attemptId))
		{
			deleteAnswer(it.value()["ID"]);
		}
	}

	return recordSet;
}

std::vector<std::string> DBManager::answerQuestion(crow::query_string data)
{
	std::vector<std::string> recordSet;
	nlohmann::json answersJSON;

	// Get the JSON from the file
	try
	{
		answersJSON = getJSONFromFile("answers.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open answers.json file");
		return recordSet;
	}

	// Check for duplicate name
	if (checkIfValueExistsInField(answersJSON, "QuestionID", data.get("questionId"), "AttemptID", data.get("attemptId")))
	{
		recordSet.push_back("There is already an answer for this question in this attempt");
		return recordSet;
	}

	// Check if json is []
	if (answersJSON.is_array())
	{
		if (answersJSON.empty())
		{
			answersJSON = nullptr;
		}
	}

	QuestionInfo questionInfo = getQuestionInfo(std::stoi(data.get("questionId")));

	std::string type = data.get("type");

	if (type == "2")
	{
		std::vector<std::string> answers =
		{
			data.get("cell 0x0"),
			data.get("cell 0x1"),
			data.get("cell 0x2"),
			data.get("cell 0x3"),
			data.get("cell 1x0"),
			data.get("cell 1x1"),
			data.get("cell 1x2"),
			data.get("cell 1x3"),
			data.get("cell 2x0"),
			data.get("cell 2x1"),
			data.get("cell 2x2"),
			data.get("cell 2x3"),
			data.get("cell 3x0"),
			data.get("cell 3x1"),
			data.get("cell 3x2"),
			data.get("cell 3x3"),
		};

		answersJSON.push_back(
			{
			{ "ID", getLastId(answersJSON) + 1},
			{ "Type", type},
			{ "QuestionID", data.get("questionId") },
			{ "AttemptID", data.get("attemptId") },
			{ "Cell0x0", answers[0] },
			{ "Cell0x1", answers[1] },
			{ "Cell0x2", answers[2] },
			{ "Cell0x3", answers[3] },
			{ "Cell1x0", answers[4] },
			{ "Cell1x1", answers[5] },
			{ "Cell1x2", answers[6] },
			{ "Cell1x3", answers[7] },
			{ "Cell2x0", answers[8] },
			{ "Cell2x1", answers[9] },
			{ "Cell2x2", answers[10] },
			{ "Cell2x3", answers[11] },
			{ "Cell3x0", answers[12] },
			{ "Cell3x1", answers[13] },
			{ "Cell3x2", answers[14] },
			{ "Cell3x3", answers[15] },
			{ "Score", calculateScoreForPunnetSquare(answers, questionInfo.id)}
			}
		);
	}
	else
	{
		// Add the user to the JSON
		answersJSON.push_back(
			{
			{ "ID", getLastId(answersJSON) + 1},
			{ "Type", type},
			{ "QuestionID", data.get("questionId") },
			{ "AttemptID", data.get("attemptId") },
			{ "Answer", data.get("answer") },
			{ "IsCorrect", bool(toLowerCase(questionInfo.answer) == toLowerCase(data.get("answer")))}
			}
		);
	}


	// Save the json to the file
	if (!setJSONFile(answersJSON, "answers.json"))
	{
		recordSet.push_back("Could'n open answers.json file");
		return recordSet;
	}

	return recordSet;
}

std::vector<std::string> DBManager::goToNextQuestionInAttempt(AttemptInfo attemptInfo, QuizInfo quizInfo)
{
	std::vector<std::string> recordSet;
	nlohmann::json attemptsJSON;

	// Get the JSON from the file
	try
	{
		attemptsJSON = getJSONFromFile("attempts.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open quizzes.json file");
		return recordSet;
	}

	for (auto it = attemptsJSON.begin(); it != attemptsJSON.end(); ++it)
	{
		if (it.value()["ID"] == attemptInfo.id)
		{
			for (size_t i = 0; i < quizInfo.questions.size(); i++)
			{
				if (quizInfo.questions[i].id == attemptInfo.currentQuestionId)
				{
					if (i == quizInfo.questions.size() - 1)
					{
						it.value()["inProgress"] = false;
						it.value()["TimeEnd"] = time(0);
						it.value()["Score"] = calculateScoreForAttempt(attemptInfo.id);

						if (!setJSONFile(attemptsJSON, "attempts.json"))
						{
							recordSet.push_back("Could'n open attempts.json file");
						}

						return recordSet;
					}
					else
					{
						it.value()["CurrentQuestionID"] = quizInfo.questions[i + 1].id;

						if (!setJSONFile(attemptsJSON, "attempts.json"))
						{
							recordSet.push_back("Could'n open attempts.json file");
						}

						return recordSet;
					}
				}
			}
		}
	}

	// If the execution goes here, there should be smt very wrong
	recordSet.push_back("Could not find attempt with id: " + std::to_string(attemptInfo.id));
	return recordSet;
}

std::vector<QuizInfo> DBManager::getAllQuizzesInTopicWithID(int topicId)
{
	std::vector<QuizInfo> recordSet;

	nlohmann::json quizzesJSON;

	// Get the JSON from the file
	try
	{
		quizzesJSON = getJSONFromFile("quizzes.json");
	}
	catch (std::string ex)
	{
		recordSet[0].errors.push_back("Could'n open quizzes.json file");
		return recordSet;
	}

	for (auto it = quizzesJSON.begin(); it != quizzesJSON.end(); ++it)
	{
		if (it.value()["TopicID"] == std::to_string(topicId))
		{
			recordSet.push_back({ it.value()["ID"], topicId,  it.value()["Name"] });
		}
	}

	return recordSet;
}

AnswerInfo DBManager::getAnswersInfo(int answerId)
{
	nlohmann::json answersJSON;

	// Get the JSON from the file
	try
	{
		answersJSON = getJSONFromFile("answers.json");
	}
	catch (std::string ex)
	{
		throw "Could'n open answers.json file";
	}

	for (auto it = answersJSON.begin(); it != answersJSON.end(); ++it)
	{
		if (it.value()["ID"] == answerId)
		{
			if (it.value()["Type"] == "2")
			{
				std::vector<std::string> userAnswers =
				{
					it.value()["Cell0x0"],
					it.value()["Cell0x1"],
					it.value()["Cell0x2"],
					it.value()["Cell0x3"],
					it.value()["Cell1x0"],
					it.value()["Cell1x1"],
					it.value()["Cell1x2"],
					it.value()["Cell1x3"],
					it.value()["Cell2x0"],
					it.value()["Cell2x1"],
					it.value()["Cell2x2"],
					it.value()["Cell2x3"],
					it.value()["Cell3x0"],
					it.value()["Cell3x1"],
					it.value()["Cell3x2"],
					it.value()["Cell3x3"]
				};

				return (
					AnswerInfo{
					it.value()["ID"],
					it.value()["Score"],
					std::stoi(it.value()["AttemptID"].get<std::string>()),
					NULL,
					std::stoi(it.value()["QuestionID"].get<std::string>()),
					"",
					it.value()["Type"],
					userAnswers
					}
				);
			}
			else
			{
				return (
					AnswerInfo{
					it.value()["ID"],
					NULL,
					std::stoi(it.value()["AttemptID"].get<std::string>()),
					it.value()["IsCorrect"],
					std::stoi(it.value()["QuestionID"].get<std::string>()),
					it.value()["Answer"],
					it.value()["Type"]
					}
				);
			}
		}
	}
	AnswerInfo answerInfo;
	answerInfo.errors = { "Cannot find answer with this id" };
	return answerInfo;
}

/*
std::vector<std::string> DBManager::isUserAdminInOrg(int userId, int orgId)
{
	std::vector<std::string> recordSet;

	nlohmann::json userOrgRoleJSON;

	// Get the JSON from the file
	try
	{
		userOrgRoleJSON = getJSONFromFile("userOrgRole.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open userOrgRole.json file");
		return recordSet;
	}

	for (auto it = userOrgRoleJSON.begin(); it != userOrgRoleJSON.end(); ++it)
	{
		if (it.value()["UserID"] == userId && it.value()["OrganisationID"] == orgId)
		{
			recordSet.push_back(std::to_string(it.value()["Role"] == 2));
			return recordSet;
		}
	}

	recordSet.push_back("Could not find organisation with id: " + std::to_string(orgId));
	return recordSet;
}
*/

nlohmann::json DBManager::getJSONFromFile(std::string filename)
{
	std::ifstream JSONFile(filename, std::fstream::app);

	if (!JSONFile.is_open())
	{
		throw std::string("Can not open file" + filename);
	}

	nlohmann::json userJSON;

	try
	{
		userJSON = nlohmann::json::parse(JSONFile);
	}
	catch (nlohmann::json::parse_error& ex)
	{
		CROW_LOG_WARNING << filename << " file is empty.";
	}

	JSONFile.close();
	return userJSON;
}

bool DBManager::setJSONFile(nlohmann::json json, std::string filename)
{
	std::ofstream JSONFile(filename, std::ios::trunc);

	if (!JSONFile.is_open())
	{
		return false;
	}

	JSONFile << json.dump();

	JSONFile.close();
	return true;
}

int DBManager::getLastId(nlohmann::json json)
{
	try
	{
		return json.rbegin().value()["ID"];
	}
	catch (nlohmann::json::invalid_iterator& ex)
	{
		return 0;
	}
}

bool DBManager::checkIfValueExistsInField(nlohmann::json json, std::string field, std::string fieldData)
{
	for (auto it = json.begin(); it != json.end(); ++it)
	{
		if (field == "Username")
		{
			if (toLowerCase(it.value()[field]) == toLowerCase(fieldData))
			{
				return true;
			}
		}
		else
		{
			if (it.value()[field] == fieldData)
			{
				return true;
			}
		}
	}

	return false;
}

bool DBManager::checkIfValueExistsInField(
	nlohmann::json json,
	std::string field,
	std::string fieldData,
	std::string field2,
	std::string fieldData2
)
{
	for (auto it = json.begin(); it != json.end(); ++it)
	{
		if (it.value()[field] == fieldData && it.value()[field2] == fieldData2)
		{
			return true;
		}
	}

	return false;
}

bool DBManager::checkIfValueExistsInField(
	nlohmann::json json,
	std::string field,
	std::string fieldData,
	std::string field2,
	int fieldData2
)
{
	for (auto it = json.begin(); it != json.end(); ++it)
	{
		if (it.value()[field] == fieldData && it.value()[field2] == fieldData2)
		{
			return true;
		}
	}

	return false;
}

std::vector<OrgUser> DBManager::getOrgUsersByOrgId(int orgId)
{
	std::vector<OrgUser> recordSet;

	nlohmann::json userOrgRoleJSON;
	nlohmann::json coursesJSON;

	// Get the JSON from the file
	try
	{
		userOrgRoleJSON = getJSONFromFile("userOrgRole.json");
		coursesJSON = getJSONFromFile("courses.json");
	}
	catch (std::string ex)
	{
		throw "Could'n open userOrgRole.json file";
	}

	for (auto it = userOrgRoleJSON.begin(); it != userOrgRoleJSON.end(); ++it)
	{
		if (it.value()["OrganisationID"] == orgId)
		{
			OrgUser orgUser = { it.value()["UserID"], it.value()["Role"] };

			if (orgUser.role == 1)
			{
				for (auto courseIt = coursesJSON.begin(); courseIt != coursesJSON.end(); ++courseIt)
				{
					if (courseIt.value()["OrgID"] == std::to_string(orgId))
					{
						std::vector<OrgUser> usersInCourses = getCourseUsersByCourseId(courseIt.value()["ID"].get<int>());

						for (auto &user: usersInCourses)
						{
							if (user.id == it.value()["UserID"] && user.role == 1)
							{
								orgUser.userCoursesId.push_back(courseIt.value()["ID"].get<int>());
								break;
							}
						}
					}
				}

			}

			recordSet.push_back(orgUser);
		}
	}

	return recordSet;
}

std::vector<OrgUser> DBManager::getCourseUsersByCourseId(int courseId)
{
	std::vector<OrgUser> recordSet;

	nlohmann::json userCourseRoleJSON;

	// Get the JSON from the file
	try
	{
		userCourseRoleJSON = getJSONFromFile("userCourseRole.json");
	}
	catch (std::string ex)
	{
		throw "Could'n open userOrgRole.json file";
	}

	for (auto it = userCourseRoleJSON.begin(); it != userCourseRoleJSON.end(); ++it)
	{
		if (it.value()["CourseID"] == courseId)
		{
			recordSet.push_back({ it.value()["UserID"], it.value()["Role"] });
		}
	}

	return recordSet;
}

std::vector<std::string> DBManager::getFieldDataInJSONByCriteria(
	std::string filename, 
	int criteria, 
	std::string criteriaField,
	std::string field
)
{
	std::vector<std::string> recordSet;

	nlohmann::json JSON;

	// Get the JSON from the file
	try
	{
		JSON = getJSONFromFile(filename);
	}
	catch (std::string ex)
	{
		throw "Could'n open " + filename + " file";
	}

	for (auto it = JSON.begin(); it != JSON.end(); ++it)
	{
		if (it.value()[criteriaField] == criteria)
		{
			if (it.value()[field].is_string())
			{
				recordSet.push_back(it.value()[field]);
			}
			else
			{
				recordSet.push_back(std::to_string(it.value()[field].get<int>()));
			}
		}
	}

	return recordSet;
}

std::vector<std::string> DBManager::deleteAnswer(int answerId)
{
	std::vector<std::string> recordSet;
	nlohmann::json answersJSON;

	// Get the JSON from the file
	try
	{
		answersJSON = getJSONFromFile("answers.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open answers.json file");
		return recordSet;
	}

	auto it = answersJSON.cbegin();
	for (; it != answersJSON.cend();)
	{
		if (it.value()["ID"] == answerId)
		{
			it = answersJSON.erase(it);
			if (!setJSONFile(answersJSON, "answers.json"))
			{
				recordSet.push_back("Could'n open answers.json file");
				return recordSet;
			}
		}
		else
		{
			++it;
		}
	}

	return recordSet;
}

std::vector<std::string> DBManager::getPunnetSquareAnswer(int questionId)
{
	std::vector<std::string> recordSet;
	nlohmann::json questionsJSON;

	// Get the JSON from the file
	try
	{
		questionsJSON = getJSONFromFile("questions.json");
	}
	catch (std::string ex)
	{
		recordSet.push_back("Could'n open questions.json file");
		return recordSet;
	}

	for (auto it = questionsJSON.begin(); it != questionsJSON.end(); ++it)
	{
		if (it.value()["ID"] == questionId)
		{
			return
			{
				it.value()["Cell0x0"],
				it.value()["Cell0x1"],
				it.value()["Cell0x2"],
				it.value()["Cell0x3"],
				it.value()["Cell1x0"],
				it.value()["Cell1x1"],
				it.value()["Cell1x2"],
				it.value()["Cell1x3"],
				it.value()["Cell2x0"],
				it.value()["Cell2x1"],
				it.value()["Cell2x2"],
				it.value()["Cell2x3"],
				it.value()["Cell3x0"],
				it.value()["Cell3x1"],
				it.value()["Cell3x2"],
				it.value()["Cell3x3"]
			};
		}
	}

	return {};
}

std::vector<AnswerInfo> DBManager::getAllAnswersInAttemptWithId(int attemptId)
{
	std::vector<AnswerInfo> recordSet;

	nlohmann::json answersJSON;

	// Get the JSON from the file
	try
	{
		answersJSON = getJSONFromFile("answers.json");
	}
	catch (std::string ex)
	{
		throw "Could'n open answers.json file";
	}

	for (auto it = answersJSON.begin(); it != answersJSON.end(); ++it)
	{
		if (it.value()["AttemptID"] == std::to_string(attemptId))
		{
			if (it.value()["Type"] == "2")
			{
				std::vector<std::string> userAnswers =
				{
					it.value()["Cell0x0"],
					it.value()["Cell0x1"],
					it.value()["Cell0x2"],
					it.value()["Cell0x3"],
					it.value()["Cell1x0"],
					it.value()["Cell1x1"],
					it.value()["Cell1x2"],
					it.value()["Cell1x3"],
					it.value()["Cell2x0"],
					it.value()["Cell2x1"],
					it.value()["Cell2x2"],
					it.value()["Cell2x3"],
					it.value()["Cell3x0"],
					it.value()["Cell3x1"],
					it.value()["Cell3x2"],
					it.value()["Cell3x3"]
				};

				recordSet.push_back({
					it.value()["ID"],
					it.value()["Score"],
					std::stoi(it.value()["AttemptID"].get<std::string>()),
					NULL,
					std::stoi(it.value()["QuestionID"].get<std::string>()),
					"",
					it.value()["Type"],
					userAnswers
					});
			}
			else
			{
				recordSet.push_back({
					it.value()["ID"],
					NULL,
					std::stoi(it.value()["AttemptID"].get<std::string>()),
					it.value()["IsCorrect"],
					std::stoi(it.value()["QuestionID"].get<std::string>()),
					it.value()["Answer"],
					it.value()["Type"]
					});
			}
		}
	}

	return recordSet;
}

float DBManager::calculateScoreForPunnetSquare(std::vector<std::string> userAnswers, int questionId)
{
	float score = 0;

	std::vector<std::string> answers = getPunnetSquareAnswer(questionId);

	for (size_t i = 0; i < answers.size(); i++)
	{
		if (answers[i] == userAnswers[i])
		{
			score += 0.0625;
		}
	}

	return score;
}