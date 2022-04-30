/*! @file env.h
*   @brief A header file for the managing the environment variables.
*/
#pragma once
#include <string>
#include <crow.h>

/**
 * @brief A class for the system for managing the environment variables.
*/
class EnvManager
{
public:
	EnvManager();
	
	/**
	 * @brief A function for getting environment variable
	 * @param var The name of the variable to be getted
	 * @return The environment variable
	*/
	std::string getEnv(const char* var);
};