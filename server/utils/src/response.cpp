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
		resBody["org-users"][i] = { {"user-id:", data.users[i].id} , {"role-id", data.users[i].role} };
	}

	for (size_t i = 0; i < data.courses.size(); i++)
	{
		resBody["org-courses"][i] = { {"course-id:", data.courses[i].id} , {"course-name", data.courses[i].name} };
	}

	return resBody;
}

crow::json::wvalue ResponseJSONManager::createOrgsJSONResponse(std::vector<OrgInfo> data)
{
	crow::json::wvalue resBody;

	resBody["type"] = "get-organisation-success";
	
	for (size_t i = 0; i < data.size(); i++)
	{
		resBody["orgs"][i] = { {"org-id:", data[i].id} , {"org-name", data[i].name} };
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
		resBody["course-users"][i] = { {"user-id:", data.users[i].id} , {"role-id", data.users[i].role} };
	}

	for (size_t i = 0; i < data.topics.size(); i++)
	{
		resBody["course-topics"][i] = { {"topic-id:", data.topics[i].id} , {"topic-name", data.topics[i].name} };
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
		resBody["topic-lessons"][i] = { {"lesson-id:", data.lessons[i].id} , {"lesson-name", data.lessons[i].name} };
	}

	for (size_t i = 0; i < data.quizzes.size(); i++)
	{
		resBody["topic-quizzes"][i] = { {"quiz-id:", data.quizzes[i].id} , {"quiz-name", data.quizzes[i].name} };
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

crow::json::wvalue ResponseJSONManager::createQuizJSONResponse(QuizInfo data)
{
	crow::json::wvalue resBody;

	resBody["type"] = "get-quiz-success";
	resBody["quiz-id"] = data.id;
	resBody["quiz-name"] = data.name;
	resBody["topic-id"] = data.topicId;

	return resBody;
}
