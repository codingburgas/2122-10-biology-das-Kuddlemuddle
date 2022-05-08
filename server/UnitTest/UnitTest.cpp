#include "pch.h"
#include "CppUnitTest.h"
#include "../DB/include/db.h"
#include "../types/include/types.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// Arrange
			AnswerInfo AnswerInfo;
			DBManager DBManager;

			// Act
			AnswerInfo = DBManager.getAnswersInfo(0);

			// Assert
			Assert::
		}
	};
}
