/*! @file loggerHandler.cpp
*   @brief A source file for the logger system.
*/

#include <env.h>
#include <loggerHandler.h>
#include <graylog_logger/Log.hpp>
#include <graylog_logger/FileInterface.hpp>
#include <graylog_logger/GraylogInterface.hpp>

void LoggerHandler::initLogs()
{
	EnvManager envManager;

	// Remove the default logger handler
	Log::RemoveAllHandlers();

	// Setup logging to SEQ
	if (envManager.getEnv("LOG_TO_SEQ") == "true")
	{
		Log::AddLogHandler(new Log::GraylogInterface(
			envManager.getEnv("SEQ_GELF_HOST").c_str(),
			std::stoi(envManager.getEnv("SEQ_GELF_PORT").c_str())
		));
	}

	// Setup logging to file
	if (envManager.getEnv("LOG_TO_FILE") == "true")
	{
		Log::AddLogHandler(new Log::FileInterface("logfile.log"));
	}

	crow::logger::setHandler(this);
}

LoggerHandler::LoggerHandler() = default;

int LoggerHandler::crowSeverityToGraylogSeverity(crow::LogLevel severity)
{
	switch (severity)
	{
	case crow::LogLevel::Debug:
		return 6;
		break;
	case crow::LogLevel::Info:
		return 5;
		break;
	case crow::LogLevel::Warning:
		return 4;
		break;
	case crow::LogLevel::Error:
		return 3;
		break;
	case crow::LogLevel::Critical:
		return 2;
		break;
	default:
		return 6;
		break;
	}
}

void LoggerHandler::log(std::string message, crow::LogLevel level)
{
	// Log to SEQ/file
	Log::Msg(crowSeverityToGraylogSeverity(level), message);
	Log::Flush();

	// Log to console
	std::string prefix;

	switch (level)
	{
	case crow::LogLevel::Debug:
		prefix = "DEBUG   ";
		break;
	case crow::LogLevel::Info:
		prefix = "INFO    ";
		break;
	case crow::LogLevel::Warning:
		prefix = "WARNING ";
		break;
	case crow::LogLevel::Error:
		prefix = "ERROR   ";
		break;
	case crow::LogLevel::Critical:
		prefix = "CRITICAL";
		break;
	}

	std::cerr << "(" << timestamp() << ") [" << prefix << "] " << message << std::endl;
}
