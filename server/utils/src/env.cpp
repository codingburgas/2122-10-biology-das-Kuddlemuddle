
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
		//Should log
		//throw ErrorHandler("env.cpp", "getEnv()", "Environment variable not found!");
	}
	else
	{
		return val;
	}
}