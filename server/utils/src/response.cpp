/*! @file response.cpp
*   @brief A sourse file for the response system.
*/

#include <response.h>

crow::json::wvalue ResponseJSONManager::createJSONResponse(
	bool isSuccess,
	std::vector<std::string> data, 
	std::string type
)
{
	crow::json::wvalue resBody;

	if (isSuccess)
	{
		resBody["type"] = type + "-success";
		
		if (data.size() != 0)
		{
			resBody["data"] = data;
		}
	}
	else
	{
		resBody["type"] = type + "-failure";

		if (data.size() != 0)
		{
			resBody["fields"] = data;
		}
	}

	return resBody;
}

crow::json::wvalue ResponseJSONManager::createProfileJSONResponse(std::vector<std::string> data)
{
	crow::json::wvalue resBody;

	if (data.size() == 1)
	{
		resBody["type"] = "user-failure";
		resBody["reason"] = "Error 404";
	}
	else
	{
		resBody["type"] = "user-success";
		resBody["data"] = data;
	}

	return resBody;
}

crow::json::wvalue ResponseJSONManager::createOrgJSONResponse(OrgInfo data)
{
	crow::json::wvalue resBody;

	resBody["type"] = "get-organisation-success";
	resBody["org-id"] = data.id;
	resBody["org-name"] = data.name;

	for (size_t i = 0; i < data.users.size(); i++)
	{
		if (data.users[i].role != 1)
		{
			resBody["org-users"][i] = { {"user-id", data.users[i].id} , {"role-id", data.users[i].role} };
		}
		else
		{
			resBody["org-users"][i] = { {"user-id", data.users[i].id} , {"role-id", data.users[i].role} };

			for (size_t j = 0; j < data.users[i].userCoursesId.size(); j++)
			{
				resBody["org-users"][i]["teacher-courses-id"][j] = data.users[i].userCoursesId[j];
			}
		}
	}

	for (size_t i = 0; i < data.courses.size(); i++)
	{
		resBody["org-courses"][i] = { {"course-id", data.courses[i].id} , {"course-name", data.courses[i].name} };
	}

	return resBody;
}

crow::json::wvalue ResponseJSONManager::createOrgsJSONResponse(std::vector<OrgInfo> data)
{
	crow::json::wvalue resBody;

	resBody["type"] = "get-organisation-success";
	
	for (size_t i = 0; i < data.size(); i++)
	{
		resBody["orgs"][i] = { {"org-id", data[i].id} , {"org-name", data[i].name}, {"is-admin", bool(data[i].users[0].id == 1)} };
	}

	return resBody;
}

crow::json::wvalue ResponseJSONManager::createCourseJSONResponse(CourseInfo data)
{
	crow::json::wvalue resBody;

	resBody["type"] = "get-course-success";
	resBody["course-id"] = data.id;
	resBody["course-name"] = data.name;
	resBody["org-id"] = data.orgId;

	for (size_t i = 0; i < data.users.size(); i++)
	{
		resBody["course-users"][i] = { {"user-id", data.users[i].id} , {"role-id", data.users[i].role} };
	}

	for (size_t i = 0; i < data.topics.size(); i++)
	{
		resBody["course-topics"][i] = { {"topic-id", data.topics[i].id} , {"topic-name", data.topics[i].name} };
	}

	return resBody;
}

crow::json::wvalue ResponseJSONManager::createTopicJSONResponse(TopicInfo data)
{
	crow::json::wvalue resBody;

	resBody["type"] = "get-topic-success";
	resBody["topic-id"] = data.id;
	resBody["topic-name"] = data.name;
	resBody["course-id"] = data.courseId;

	for (size_t i = 0; i < data.lessons.size(); i++)
	{
		resBody["topic-lessons"][i] = { {"lesson-id", data.lessons[i].id} , {"lesson-name", data.lessons[i].name} };
	}

	for (size_t i = 0; i < data.quizzes.size(); i++)
	{
		resBody["topic-quizzes"][i] = { {"quiz-id", data.quizzes[i].id} , {"quiz-name", data.quizzes[i].name} };
	}

	return resBody;
}

crow::json::wvalue ResponseJSONManager::createLessonJSONResponse(LessonInfo data)
{
	crow::json::wvalue resBody;

	resBody["type"] = "get-lesson-success";
	resBody["lesson-id"] = data.id;
	resBody["lesson-data"] = data.data;
	resBody["lesson-name"] = data.name;
	resBody["topic-id"] = data.topicId;

	return resBody;
}

crow::json::wvalue ResponseJSONManager::createQuizJSONResponse(QuizInfo data, bool shareData)
{
	crow::json::wvalue resBody;

	resBody["type"] = "get-quiz-success";
	resBody["quiz-id"] = data.id;
	resBody["quiz-name"] = data.name;
	resBody["topic-id"] = data.topicId;
	
	if (shareData)
	{
		for (size_t i = 0; i < data.questions.size(); i++)
		{
			resBody["quiz-questions"][i] = { {"question-id", data.questions[i].id} };
		}

		for (size_t i = 0; i < data.attempts.size(); i++)
		{
			resBody["quiz-attempts"][i] = { {"attempt-id", data.attempts[i].id} };
		}
	}

	return resBody;
}

crow::json::wvalue ResponseJSONManager::createQuestionJSONResponse(QuestionInfo data, bool shareAnswer)
{
	crow::json::wvalue resBody;

	resBody["type"] = "get-question-success";
	resBody["question-id"] = data.id;
	resBody["question-question"] = data.question;
	resBody["question-type"] = data.type;
	
	if (shareAnswer)
	{
		data.type == "2" ? resBody["question-answer"] = data.punnettAnswer : resBody["question-answer"] = data.answer;
	}

	resBody["quiz-id"] = data.quizId;

	return resBody;
}

crow::json::wvalue ResponseJSONManager::createAttemptJSONResponse(AttemptInfo data, std::string type)
{
	crow::json::wvalue resBody;

	resBody["type"] = type + "-success";
	resBody["is-in-progress"] = int(data.inProgress);
	resBody["attempt-id"] = data.id;
	resBody["attempt-quiz-id"] = data.quizId;
	resBody["attempt-user-id"] = data.userId;

	if (data.inProgress)
	{
		resBody["next-question-id"] = data.currentQuestionId;
	}
	else
	{
		resBody["score"] = data.score;
		resBody["attempt-start"] = data.timeStart;
		resBody["attempt-end"] = data.timeEnd;
		
		for (size_t i = 0; i < data.answers.size(); i++)
		{
			resBody["attempt-answers"][i] = { {"answer-id", data.answers[i].id} };
		}
	}

	return resBody;
}

crow::json::wvalue ResponseJSONManager::createAnswerJSONResponse(AnswerInfo data)
{
	crow::json::wvalue resBody;


	if (data.type == "2")
	{
		resBody["type"] = "get-answer-success";
		resBody["answer-id"] = data.id;
		resBody["answer-score"] = data.score;
		resBody["answer-attempt-id"] = data.attemptId;
		resBody["answer-question-id"] = data.questionId;
		resBody["answer-type"] = data.type;
		resBody["answer-punnett-answers"] = data.punnettAnswer;
	}
	else
	{
		resBody["type"] = "get-answer-success";
		resBody["answer-id"] = data.id;
		resBody["answer-attempt-id"] = data.attemptId;
		resBody["answer-is-correct"] = data.isCorrect;
		resBody["answer-question-id"] = data.questionId;
		resBody["answer-answer"] = data.answer;
		resBody["answer-type"] = data.type;
	}

	return resBody;
}
