/*! @file loggerHandler.h
*   @brief A header file for the logger system.
*/

#pragma once
#include <crow.h>

/**
 * @brief Class for managing logs
*/
class LoggerHandler : crow::ILogHandler
{
public:
	/**
	 * @brief Initialise logs
	*/
	void initLogs();

	/**
	 * @brief Constructor for the class
	*/
	LoggerHandler();

	/**
	 * @brief Function for logging to the handlers
	 * @param message Message to be logged
	 * @param level The level of the log
	*/
	void log(std::string message, crow::LogLevel level);
private:
	/**
	 * @brief Convert crow severity level to graylog severity level
	 * @param severity Crow severity level
	 * @return Graylog severity level
	*/
	int crowSeverityToGraylogSeverity(crow::LogLevel severity);
};