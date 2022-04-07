#pragma once
#include <crow.h>

class LoggerHandler : crow::ILogHandler
{
public:
	void initLogs();
	LoggerHandler();
	void log(std::string message, crow::LogLevel level);
private:
	int crowSeverityToGraylogSeverity(crow::LogLevel severity);
};