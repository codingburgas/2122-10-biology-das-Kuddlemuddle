#include "pch.h"
#include "CppUnitTest.h"
#include "../DB/include/db.h"
#include "../types/include/types.h" 

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DBUnitTest
{
	TEST_CLASS(DBUnitTest)
	{
	public:
		
		TEST_METHOD(AnswerInfoTest)
		{
			// Arrange
			AnswerInfo answerInfo;
			AnswerInfo answerInfo1;
			DBManager DBManager;

			// Act
			answerInfo = DBManager.getAnswersInfo(0);

			// Assert
			Assert::AreSame(answerInfo, answerInfo1);
		}



	};
}
