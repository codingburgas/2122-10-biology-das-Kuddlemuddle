#include "pch.h"
#include "CppUnitTest.h"
#include "../logs/include/loggerHandler.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			int a = LoggerHandler::crowSeverityToGraylogSeverity(crow::LogLevel severity);
		}
	};
}
