#include <db.h>

std::vector<std::string> DBManager::registerUser(crow::query_string data)
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
		{ "Password", data.get("password") },
		// TODO: Add salt
		{ "RoleID", 0 },
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
			if (it.value()["Email"] == data.get("loginCredential") && it.value()["Password"] == data.get("password"))
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
		recordSet.push_back("There is not a user with this email: " + std::string(data.get("loginCredential")) + " or password is invalid.");
		return recordSet;
	}
	else
	{
		for (auto it = userJSON.begin(); it != userJSON.end(); ++it)
		{
			// Authtenticate user
			if (toLowerCase(it.value()["Username"]) == toLowerCase(data.get("loginCredential")) && it.value()["Password"] == data.get("password"))
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
		recordSet.push_back("There is not a user with this username: " + std::string(data.get("loginCredential")) + " or password is invalid.");
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
					it.value()["FirstName"],
					it.value()["LastName"],
					it.value()["Username"],
					it.value()["Email"],
					std::to_string(int(it.value()["RoleID"])),
					it.value()["AvatarURL"]
				};
			}
		}

		// If the execution goes here, there should be smt very wrong
		recordSet.push_back("Could not find user with id: " + std::to_string(userId));
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
			it.value()["FirstName"] = std::string(data.get("firstName")).empty() ? it.value()["FirstName"] : data.get("firstName");
			it.value()["LastName"] = std::string(data.get("lastName")).empty() ? it.value()["LastName"] : data.get("lastName");
			it.value()["Username"] = std::string(data.get("username")).empty() ? it.value()["Username"] : data.get("username");
			it.value()["Email"] = std::string(data.get("email")).empty() ? it.value()["Email"] : data.get("email");
			it.value()["Password"] = std::string(data.get("password")).empty() ? it.value()["Password"] : data.get("password");

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

std::vector<std::string> DBManager::createOrg(int userId, crow::query_string data)
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
		{ "Password", data.get("password") }
		// TODO: Add Salt
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

std::vector<std::string> DBManager::addUserToOrganisation(int userId, int orgId, UserRolesInOrgs userRolesInOrgs, bool createNewEntry)
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
		recordSet.push_back("Couldn't find user with id: " + std::to_string(userId) + " or organisation with id: " + std::to_string(orgId));
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
			recordSet.push_back(std::to_string(it.value()["Password"] == password));
			return recordSet;
		}
	}

	recordSet.push_back("Could not find organisation with id: " + orgId);
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

	recordSet.push_back("Could not find organisation with id: " + std::to_string(orgId));
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
			it.value()["Name"] = std::string(data.get("orgName")).empty() ? it.value()["Name"] : data.get("orgName");
			it.value()["Password"] = std::string(data.get("password")).empty() ? it.value()["Password"] : data.get("password");

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

std::vector<OrgInfo> DBManager::getAllOrgsInfo()
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
		recordSet.push_back({ it.value()["ID"], it.value()["Name"] });
	}

	return recordSet;
}

std::vector<std::string> DBManager::createCourse(crow::query_string data)
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
		recordSet.push_back("There is already a course with this name: " + std::string(data.get("courseName")));
		return recordSet;
	}

	if (checkIfValueExistsInField(orgsJSON, "OrgID", data.get("orgId")))
	{
		recordSet.push_back("There is not a organisation with this id: " + std::string(data.get("orgId")));
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
		{ "Password", data.get("password") },
		{ "OrgID", data.get("orgId") }
		// TODO: Add Salt
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
			recordSet.push_back(std::to_string(it.value()["Password"] == password));
			return recordSet;
		}
	}

	recordSet.push_back("Could not find course with id: " + courseId);
	return recordSet;
}

std::vector<std::string> DBManager::addUserToCourse(int userId, int courseId, UserRolesInOrgs userRolesInOrgs, bool createNewEntry)
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
		recordSet.push_back("Couldn't find user with id: " + std::to_string(userId) + " or course with id: " + std::to_string(courseId));
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
	returnValue.name = getCourseNameById(courseId)[0];

	if (getCourseOrgById(returnValue.id).empty())
	{
		returnValue.errors.push_back("Can not find course with id: " + std::to_string(courseId));
		return returnValue;
	}

	returnValue.orgId = std::stoi(getCourseOrgById(returnValue.id)[0]);

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
			it.value()["Name"] = std::string(data.get("courseName")).empty() ? it.value()["Name"] : data.get("courseName");
			it.value()["Password"] = std::string(data.get("password")).empty() ? it.value()["Password"] : data.get("password");

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

std::vector<std::string> DBManager::canUserAccessCourse(int courseId, int userId, bool allowOnlyTeachersAndAdmins)
{
	std::vector<std::string> returnVal;

	CourseInfo courseInfo = getCourseInfo(courseId);

	if (!courseInfo.errors.empty())
	{
		std::string log = "Failed to get course info for course with name: " + std::to_string(courseId) + ". Reasons: ";

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
			std::string log = "Failed to get course info with id: " + std::to_string(courseId) + ". Reason: User is unauthorised";
			returnVal.push_back("User is unauthorised");

			CROW_LOG_WARNING << log;

			return returnVal;
		}
	}
	else
	{
		if (recordSet[0] != "1")
		{
			std::string log = "Failed to get course info with id: " + std::to_string(courseId) + ". Reason: User is unauthorised";
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
	returnValue.name = getTopicNameById(topicId)[0];

	if (getTopicCourseById(returnValue.id).empty())
	{
		returnValue.errors.push_back("Can not find topic with id: " + std::to_string(topicId));
		return returnValue;
	}

	returnValue.courseId = std::stoi(getTopicCourseById(returnValue.id)[0]);

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

std::vector<std::string> DBManager::getCourseNameById(int id)
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
		if (it.value()["ID"] == id)
		{
			recordSet.push_back(it.value()["Name"]);
			return recordSet;
		}
	}

	recordSet.push_back("Could not find course with id: " + std::to_string(id));
	return recordSet;
}

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
			recordSet.push_back({it.value()["ID"], orgId,  it.value()["Name"]});
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

	// Get the JSON from the file
	try
	{
		topicsJSON = getJSONFromFile("topics.json");
		lessonsJSON = getJSONFromFile("lessons.json");
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
			it.value()["Name"] = std::string(data.get("topicName")).empty() ? it.value()["Name"] : data.get("topicName");
			
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

	if (getLessonNameById(lessonId).empty())
	{
		returnValue.errors.push_back("Can not find lesson with id: " + std::to_string(lessonId));
		return returnValue;
	}
	
	returnValue.name = getLessonNameById(lessonId)[0];
	returnValue.topicId = std::stoi(getLessonTopicById(lessonId)[0]);
	returnValue.data = getLessonDataById(lessonId)[0];

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
			it.value()["Name"] = std::string(data.get("lessonName")).empty() ? it.value()["Name"] : data.get("lessonName");
			it.value()["Data"] = std::string(data.get("lessonData")).empty() ? it.value()["Data"] : data.get("lessonData");

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

bool DBManager::checkIfValueExistsInField(nlohmann::json json, std::string field, std::string fieldData, std::string field2, std::string fieldData2)
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

	// Get the JSON from the file
	try
	{
		userOrgRoleJSON = getJSONFromFile("userOrgRole.json");
	}
	catch (std::string ex)
	{
		throw "Could'n open userOrgRole.json file";
	}

	for (auto it = userOrgRoleJSON.begin(); it != userOrgRoleJSON.end(); ++it)
	{
		if (it.value()["OrganisationID"] == orgId)
		{
			recordSet.push_back({ it.value()["UserID"], it.value()["Role"] });
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

std::vector<std::string> DBManager::getCourseOrgById(int id)
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
		throw "Could'n open courses.json file";
	}

	for (auto it = coursesJSON.begin(); it != coursesJSON.end(); ++it)
	{
		if (it.value()["ID"] == id)
		{
			recordSet.push_back(it.value()["OrgID"]);
		}
	}

	return recordSet;
}

std::vector<std::string> DBManager::getTopicCourseById(int id)
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
		throw "Could'n open topics.json file";
	}

	for (auto it = topicsJSON.begin(); it != topicsJSON.end(); ++it)
	{
		if (it.value()["ID"] == id)
		{
			recordSet.push_back(it.value()["CourseID"]);
		}
	}

	return recordSet;
}

std::vector<std::string> DBManager::getTopicNameById(int id)
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
		throw "Could'n open topics.json file";
	}

	for (auto it = topicsJSON.begin(); it != topicsJSON.end(); ++it)
	{
		if (it.value()["ID"] == id)
		{
			recordSet.push_back(it.value()["Name"]);
		}
	}

	return recordSet;
}

std::vector<std::string> DBManager::getLessonTopicById(int id)
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
		throw "Could'n open lessons.json file";
	}

	for (auto it = lessonsJSON.begin(); it != lessonsJSON.end(); ++it)
	{
		if (it.value()["ID"] == id)
		{
			recordSet.push_back(it.value()["TopicID"]);
		}
	}

	return recordSet;
}

std::vector<std::string> DBManager::getLessonNameById(int id)
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
		throw "Could'n open lessons.json file";
	}

	for (auto it = lessonsJSON.begin(); it != lessonsJSON.end(); ++it)
	{
		if (it.value()["ID"] == id)
		{
			recordSet.push_back(it.value()["Name"]);
		}
	}

	return recordSet;
}

std::vector<std::string> DBManager::getLessonDataById(int id)
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
		throw "Could'n open lessons.json file";
	}

	for (auto it = lessonsJSON.begin(); it != lessonsJSON.end(); ++it)
	{
		if (it.value()["ID"] == id)
		{
			recordSet.push_back(it.value()["Data"]);
		}
	}

	return recordSet;
}