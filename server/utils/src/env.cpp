
/*! @file env.cpp
*   @brief A source file for the managing the environment variables.
*/
#include <dotenv.h>
#include <env.h>

EnvManager::EnvManager() = default;

std::string EnvManager::getEnv(const char* var)
{
	dotenv::init("../../.env");

	std::string val = dotenv::getenv(var);

	if (val == "")
	{
		CROW_LOG_ERROR << "Environment variable not found! Please refer to troubleshooting page!";
	}
	else
	{
		return val;
	}
}