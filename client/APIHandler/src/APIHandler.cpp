#include <APIHandler.h>
#include <nlohmann/json.hpp>

std::string APIHandler::registerHandler(RegisterData regData)
{
    cpr::Response r = cpr::Post(cpr::Url{ "http://localhost:18080/api/register"},
                                   cpr::Payload{ 
                                       {"firstName", regData.fname},
                                       {"lastName", regData.lname},
                                       {"username", regData.username},
                                       {"email", regData.email},
                                       {"password", regData.password}
        });

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        return "There is a problem with the server! Please try again later!";
    }

    if (JSONRes["type"] == "register-success")
    {
        return "";
    }

    std::string returnVal;

    if (JSONRes["fields"].size() != 1)
    {
         returnVal = "The following fileds are incorrect: ";

        for (auto& el: JSONRes["fields"].items())
        {
            returnVal += el.value();
            returnVal += " ";
        }
    }

    return JSONRes["fields"][0];
}

std::string APIHandler::loginHandler(LoginData logData, SceneContex* ctx)
{
    cpr::Response r = cpr::Post(cpr::Url{ "http://localhost:18080/api/login" },
        cpr::Payload{
            {"loginCredential", logData.loginCred},
            {"password", logData.pass}
        });

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        return "There is a problem with the server! Please try again later!";
    }

    if (JSONRes["type"] == "login-success")
    {
        ctx->isAuth = true;
        ctx->JWTToken = JSONRes["data"][0];
        return "";
    }

    std::string returnVal;

    if (JSONRes["fields"].size() != 1)
    {
        returnVal = "The following fileds are incorrect: ";

        for (auto& el : JSONRes["fields"].items())
        {
            returnVal += el.value();
            returnVal += " ";
        }
    }

    return JSONRes["fields"][0];
}

std::string APIHandler::getUserInfo(std::string username, SceneContex* ctx, User &user)
{
    cpr::Response r;
    if (!ctx->JWTToken.empty())
    {
        r = cpr::Get(cpr::Url{ "http://localhost:18080/api/users/" + username },
            cpr::Bearer({ ctx->JWTToken }));
    }
    else
    {
        r = cpr::Get(cpr::Url{ "http://localhost:18080/api/users/" + username });
    }

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        return "There is a problem with the server! Please try again later!";
    }

    if (JSONRes["type"] == "user-success")
    {
        user.id = JSONRes["data"][0];
        user.fname = JSONRes["data"][1];
        user.lname = JSONRes["data"][2];
        user.username = JSONRes["data"][3];
        user.email = JSONRes["data"][4];
        user.role = JSONRes["data"][5];
        user.avatarURL = JSONRes["data"][6];
        return "";
    }

    return "The user wasn't found";
}

std::string APIHandler::getUserInfoById(std::string userId, SceneContex* ctx, User& user)
{
    cpr::Response r;
    if (!ctx->JWTToken.empty())
    {
        r = cpr::Get(cpr::Url{ "http://localhost:18080/api/users/" + userId },
            cpr::Bearer({ ctx->JWTToken }));
    }
    else
    {
        r = cpr::Get(cpr::Url{ "http://localhost:18080/api/users/" + userId });
    }

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        return "There is a problem with the server! Please try again later!";
    }

    if (JSONRes["type"] == "user-success")
    {
        user.id = JSONRes["data"][0];
        user.fname = JSONRes["data"][1];
        user.lname = JSONRes["data"][2];
        user.username = JSONRes["data"][3];
        user.email = JSONRes["data"][4];
        user.role = JSONRes["data"][5];
        user.avatarURL = JSONRes["data"][6];
        return "";
    }

    return "The user wasn't found";
}

std::string APIHandler::deleteUser(std::string username, std::string JWTToken)
{
    auto r = cpr::Delete(cpr::Url{ "http://localhost:18080/api/users/" + username },
        cpr::Bearer({ JWTToken }));

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        return "There is a problem with the server! Please try again later!";
    }

    if (r.status_code != 200)
    {
        return "There is a problem with the server! Please try again later!";
    }

    return "";
}

std::string APIHandler::updateUser(RegisterData userData, std::string JWTToken)
{
    cpr::Response r = cpr::Patch(cpr::Url{ "http://localhost:18080/api/users/@me" },
        cpr::Bearer({ JWTToken }),
        cpr::Payload{
            {"firstName", userData.fname},
            {"lastName", userData.lname},
            {"username", userData.username},
            {"email", userData.email},
            {"password", userData.password}
        });

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        return "There is a problem with the server! Please try again later!";
    }

    if (JSONRes["type"] == "user-update-success")
    {
        return "";
    }

    std::string returnVal;

    if (JSONRes["fields"].size() != 1)
    {
        returnVal = "The following fileds are incorrect: ";

        for (auto& el : JSONRes["fields"].items())
        {
            returnVal += el.value();
            returnVal += " ";
        }
    }

    return JSONRes["fields"][0];
}

std::vector<OrgInfo> APIHandler::getAllOrgs(std::string JWTToken)
{
    cpr::Response r;
    r = cpr::Get(cpr::Url{ "http://localhost:18080/api/orgs/all" },
        cpr::Bearer({ JWTToken }));

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        // Send error
        OrgInfo error;
        error.errors = "There is a problem with the server! Please try again later!";
        return { error };
    }

    std::vector<OrgInfo> orgsInfo;
    
    if (JSONRes["type"] == "get-organisation-success")
    {
        for (auto it = JSONRes["orgs"].begin(); it != JSONRes["orgs"].end(); ++it)
        {
            orgsInfo.push_back({ it.value()["org-id"].get<int>(),it.value()["org-name"].get<std::string>(), it.value()["is-admin"].get<bool>()});
        }
    }

    return orgsInfo;
}

std::string APIHandler::createOrg(OrgData orgData, std::string JWTToken)
{
    cpr::Response r = cpr::Post(cpr::Url{ "http://localhost:18080/api/createNewOrg" },
        cpr::Bearer({ JWTToken }),
        cpr::Payload{
            {"orgName", orgData.name},
            {"password", orgData.password}
        });

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        return "There is a problem with the server! Please try again later!";
    }

    if (JSONRes["type"] == "organisation-register-success")
    {
        return "";
    }

    std::string returnVal;

    if (JSONRes["fields"].size() != 1)
    {
        returnVal = "The following fileds are incorrect: ";

        for (auto& el : JSONRes["fields"].items())
        {
            returnVal += el.value();
            returnVal += " ";
        }
    }

    return JSONRes["fields"][0];
}

std::string APIHandler::createCourse(OrgData courseData, int orgId, std::string JWTToken)
{
    cpr::Response r = cpr::Post(cpr::Url{ "http://localhost:18080/api/createNewCourse" },
        cpr::Bearer({ JWTToken }),
        cpr::Payload{
            {"courseName", courseData.name},
            {"password", courseData.password},
            {"orgId", std::to_string(orgId)}
        });

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        return "There is a problem with the server! Please try again later!";
    }

    if (JSONRes["type"] == "course-register-success")
    {
        return "";
    }

    std::string returnVal;

    if (JSONRes["fields"].size() != 1)
    {
        returnVal = "The following fileds are incorrect: ";

        for (auto& el : JSONRes["fields"].items())
        {
            returnVal += el.value();
            returnVal += " ";
        }
    }

    return JSONRes["fields"][0];
}

std::string APIHandler::joinCourse(int courseId, std::string password, std::string JWTToken)
{
    cpr::Response r = cpr::Post(cpr::Url{ "http://localhost:18080/api/joinCourse" },
        cpr::Bearer({ JWTToken }),
        cpr::Payload{
            {"courseId", std::to_string(courseId)},
            {"password", password}
        });

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        return "There is a problem with the server! Please try again later!";
    }

    if (JSONRes["type"] == "join-course-success")
    {
        return "";
    }

    std::string returnVal;

    if (JSONRes["fields"].size() != 1)
    {
        returnVal = "The following fileds are incorrect: ";

        for (auto& el : JSONRes["fields"].items())
        {
            returnVal += el.value();
            returnVal += " ";
        }
    }

    return JSONRes["fields"][0];
}

std::string APIHandler::joinOrg(int orgId, std::string password, std::string JWTToken)
{
    cpr::Response r = cpr::Post(cpr::Url{ "http://localhost:18080/api/joinOrg" },
        cpr::Bearer({ JWTToken }),
        cpr::Payload{
            {"orgId", std::to_string(orgId)},
            {"password", password}
        });

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        return "There is a problem with the server! Please try again later!";
    }

    if (JSONRes["type"] == "join-org-success")
    {
        return "";
    }

    std::string returnVal;

    if (JSONRes["fields"].size() != 1)
    {
        returnVal = "The following fileds are incorrect: ";

        for (auto& el : JSONRes["fields"].items())
        {
            returnVal += el.value();
            returnVal += " ";
        }
    }

    return JSONRes["fields"][0];
}

std::string APIHandler::deleteOrg(std::string name, std::string JWTToken)
{
    auto r = cpr::Delete(cpr::Url{ "http://localhost:18080/api/orgs/" + name },
        cpr::Bearer({ JWTToken }));

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        return "There is a problem with the server! Please try again later!";
    }

    if (r.status_code != 200)
    {
        return "There is a problem with the server! Please try again later!";
    }

    return "";
}

std::string APIHandler::updateOrg(OrgData orgData, std::string name, std::string JWTToken)
{
    cpr::Response r = cpr::Patch(cpr::Url{ "http://localhost:18080/api/orgs/" + name },
        cpr::Bearer({ JWTToken }),
        cpr::Payload{
            {"orgName", orgData.name},
            {"password", orgData.password}
        });

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        return "There is a problem with the server! Please try again later!";
    }

    if (JSONRes["type"] == "organisation-update-success")
    {
        return "";
    }

    std::string returnVal;

    if (JSONRes["fields"].size() != 1)
    {
        returnVal = "The following fileds are incorrect: ";

        for (auto& el : JSONRes["fields"].items())
        {
            returnVal += el.value();
            returnVal += " ";
        }
    }

    return JSONRes["fields"][0];
}

std::string APIHandler::updateRoleInOrg(int orgId, int userId, int roleId, std::string JWTToken)
{
    cpr::Response r = cpr::Post(cpr::Url{ "http://localhost:18080/api/updateRolesInOrg" },
        cpr::Bearer({ JWTToken }),
        cpr::Payload{
            {"orgId", std::to_string(orgId)},
            {"userId", std::to_string(userId)},
            {"roleId", std::to_string(roleId)}
        });

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        return "There is a problem with the server! Please try again later!";
    }

    if (JSONRes["type"] == "update-user-role-org-success")
    {
        return "";
    }

    return JSONRes["fields"][0];
}

std::string APIHandler::deleteCourse(int id, std::string JWTToken)
{
    auto r = cpr::Delete(cpr::Url{ "http://localhost:18080/api/courses/" + std::to_string(id) },
        cpr::Bearer({ JWTToken }));

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        return "There is a problem with the server! Please try again later!";
    }

    if (r.status_code != 200)
    {
        return "There is a problem with the server! Please try again later!";
    }

    return "";
}

std::string APIHandler::updateCourse(OrgData orgData, int id, std::string JWTToken)
{
    cpr::Response r = cpr::Patch(cpr::Url{ "http://localhost:18080/api/courses/" + std::to_string(id) },
        cpr::Bearer({ JWTToken }),
        cpr::Payload{
            {"courseName", orgData.name},
            {"password", orgData.password}
        });

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        return "There is a problem with the server! Please try again later!";
    }

    if (JSONRes["type"] == "course-update-success")
    {
        return "";
    }

    std::string returnVal;

    if (JSONRes["fields"].size() != 1)
    {
        returnVal = "The following fileds are incorrect: ";

        for (auto& el : JSONRes["fields"].items())
        {
            returnVal += el.value();
            returnVal += " ";
        }
    }

    return JSONRes["fields"][0];
}

std::string APIHandler::addTeacherToCourse(int courseId, int teacherId, std::string JWTToken)
{
    cpr::Response r = cpr::Post(cpr::Url{ "http://localhost:18080/api/addTeacher" },
        cpr::Bearer({ JWTToken }),
        cpr::Payload{
            {"courseId", std::to_string(courseId)},
            {"teacherId", std::to_string(teacherId)}
        });

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        return "There is a problem with the server! Please try again later!";
    }

    if (JSONRes["type"] == "add-teacher-success")
    {
        return "";
    }

    return JSONRes["fields"][0];
}

std::string APIHandler::createTopic(std::string topicName, int courseId, std::string JWTToken)
{
    cpr::Response r = cpr::Post(cpr::Url{ "http://localhost:18080/api/createNewTopic" },
        cpr::Bearer({ JWTToken }),
        cpr::Payload{
            {"topicName",topicName},
            {"courseId", std::to_string(courseId)}
        });

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        return "There is a problem with the server! Please try again later!";
    }

    if (JSONRes["type"] == "topic-register-success")
    {
        return "";
    }

    std::string returnVal;

    if (JSONRes["fields"].size() != 1)
    {
        returnVal = "The following fileds are incorrect: ";

        for (auto& el : JSONRes["fields"].items())
        {
            returnVal += el.value();
            returnVal += " ";
        }
    }

    return JSONRes["fields"][0];
}

std::string APIHandler::updateTopic(std::string topicName, int id, std::string JWTToken)
{
    cpr::Response r = cpr::Patch(cpr::Url{ "http://localhost:18080/api/topics/" + std::to_string(id) },
        cpr::Bearer({ JWTToken }),
        cpr::Payload{
            {"topicName", topicName}
        });

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        return "There is a problem with the server! Please try again later!";
    }

    if (JSONRes["type"] == "topic-update-success")
    {
        return "";
    }

    std::string returnVal;

    if (JSONRes["fields"].size() != 1)
    {
        returnVal = "The following fileds are incorrect: ";

        for (auto& el : JSONRes["fields"].items())
        {
            returnVal += el.value();
            returnVal += " ";
        }
    }

    return JSONRes["fields"][0];
}

std::string APIHandler::deleteTopic(int id, std::string JWTToken)
{
    auto r = cpr::Delete(cpr::Url{ "http://localhost:18080/api/topics/" + std::to_string(id) },
        cpr::Bearer({ JWTToken }));

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        return "There is a problem with the server! Please try again later!";
    }

    if (r.status_code != 200)
    {
        return "There is a problem with the server! Please try again later!";
    }

    return "";
}

std::string APIHandler::createLesson(std::string lessonName, std::string lessonData, int topicId, std::string JWTToken)
{
    cpr::Response r = cpr::Post(cpr::Url{ "http://localhost:18080/api/createNewLesson" },
        cpr::Bearer({ JWTToken }),
        cpr::Payload{
            {"lessonName", lessonName},
            {"lessonData", lessonData},
            {"topicId", std::to_string(topicId)}
        });

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        return "There is a problem with the server! Please try again later!";
    }

    if (JSONRes["type"] == "lesson-register-success")
    {
        return "";
    }

    std::string returnVal;

    if (JSONRes["fields"].size() != 1)
    {
        returnVal = "The following fileds are incorrect: ";

        for (auto& el : JSONRes["fields"].items())
        {
            returnVal += el.value();
            returnVal += " ";
        }
    }

    return JSONRes["fields"][0];
}

std::string APIHandler::deleteLesson(int id, std::string JWTToken)
{
    auto r = cpr::Delete(cpr::Url{ "http://localhost:18080/api/lessons/" + std::to_string(id) },
        cpr::Bearer({ JWTToken }));

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        return "There is a problem with the server! Please try again later!";
    }

    if (r.status_code != 200)
    {
        return "There is a problem with the server! Please try again later!";
    }

    return "";
}

std::string APIHandler::updateLesson(std::string lessonName, std::string lessonData, int lessonId, std::string JWTToken)
{
    cpr::Response r = cpr::Patch(cpr::Url{ "http://localhost:18080/api/lessons/" + std::to_string(lessonId) },
        cpr::Bearer({ JWTToken }),
        cpr::Payload{
            {"lessonName", lessonName},
            {"lessonData", lessonData}
        });

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        return "There is a problem with the server! Please try again later!";
    }

    if (JSONRes["type"] == "lesson-update-success")
    {
        return "";
    }

    std::string returnVal;

    if (JSONRes["fields"].size() != 1)
    {
        returnVal = "The following fileds are incorrect: ";

        for (auto& el : JSONRes["fields"].items())
        {
            returnVal += el.value();
            returnVal += " ";
        }
    }

    return JSONRes["fields"][0];
}

std::string APIHandler::createQuiz(std::string quizName, int topicId, std::string JWTToken)
{
    cpr::Response r = cpr::Post(cpr::Url{ "http://localhost:18080/api/createNewQuiz" },
        cpr::Bearer({ JWTToken }),
        cpr::Payload{
            {"quizName", quizName},
            {"topicId", std::to_string(topicId)}
        });

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        return "There is a problem with the server! Please try again later!";
    }

    if (JSONRes["type"] == "quiz-register-success")
    {
        return "";
    }

    std::string returnVal;

    if (JSONRes["fields"].size() != 1)
    {
        returnVal = "The following fileds are incorrect: ";

        for (auto& el : JSONRes["fields"].items())
        {
            returnVal += el.value();
            returnVal += " ";
        }
    }

    return JSONRes["fields"][0];
}

std::string APIHandler::updateQuiz(std::string quizName, int quizId, std::string JWTToken)
{
    cpr::Response r = cpr::Patch(cpr::Url{ "http://localhost:18080/api/quizzes/" + std::to_string(quizId) },
        cpr::Bearer({ JWTToken }),
        cpr::Payload{
            {"quizName", quizName}
        });

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        return "There is a problem with the server! Please try again later!";
    }

    if (JSONRes["type"] == "quiz-update-success")
    {
        return "";
    }

    std::string returnVal;

    if (JSONRes["fields"].size() != 1)
    {
        returnVal = "The following fileds are incorrect: ";

        for (auto& el : JSONRes["fields"].items())
        {
            returnVal += el.value();
            returnVal += " ";
        }
    }

    return JSONRes["fields"][0];
}

std::string APIHandler::deleteQuiz(int id, std::string JWTToken)
{
    auto r = cpr::Delete(cpr::Url{ "http://localhost:18080/api/quizzes/" + std::to_string(id) },
        cpr::Bearer({ JWTToken }));

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        return "There is a problem with the server! Please try again later!";
    }

    if (r.status_code != 200)
    {
        return "There is a problem with the server! Please try again later!";
    }

    return "";
}

std::string APIHandler::createQuestion(std::string question, std::string answer, int quizId, std::string JWTToken)
{
    cpr::Response r = cpr::Post(cpr::Url{ "http://localhost:18080/api/createNewQuestion" },
        cpr::Bearer({ JWTToken }),
        cpr::Payload{
            {"question", question},
            {"answer", answer},
            {"quizId", std::to_string(quizId)},
            {"type", std::to_string(1)},
        });

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        return "There is a problem with the server! Please try again later!";
    }

    if (JSONRes["type"] == "question-register-success")
    {
        return "";
    }

    std::string returnVal;

    if (JSONRes["fields"].size() != 1)
    {
        returnVal = "The following fileds are incorrect: ";

        for (auto& el : JSONRes["fields"].items())
        {
            returnVal += el.value();
            returnVal += " ";
        }
    }

    return JSONRes["fields"][0];
}

std::string APIHandler::createPunnettQuestion(std::string P1, std::string P2, int quizId, std::string JWTToken)
{
    cpr::Response r = cpr::Post(cpr::Url{ "http://localhost:18080/api/createNewQuestion" },
        cpr::Bearer({ JWTToken }),
        cpr::Payload{
            {"P1", P1},
            {"P2", P2},
            {"quizId", std::to_string(quizId)},
            {"type", std::to_string(2)},
        });

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        return "There is a problem with the server! Please try again later!";
    }

    if (JSONRes["type"] == "question-register-success")
    {
        return "";
    }

    std::string returnVal;

    if (JSONRes["fields"].size() != 1)
    {
        returnVal = "The following fileds are incorrect: ";

        for (auto& el : JSONRes["fields"].items())
        {
            returnVal += el.value();
            returnVal += " ";
        }
    }

    return JSONRes["fields"][0];
}

bool APIHandler::doUserHaveAccessToOrg(std::string name, std::string JWTToken)
{
    auto r = cpr::Get(cpr::Url{ "http://localhost:18080/api/orgs/" + name },
        cpr::Bearer({ JWTToken }));

    return (r.status_code == 200);
}

std::vector<User> APIHandler::getAllUsers(std::string JWTToken)
{
    cpr::Response r;
    r = cpr::Get(cpr::Url{ "http://localhost:18080/api/users/all" },
        cpr::Bearer({ JWTToken }));

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        // Send error
        User error;
        error.errors = "There is a problem with the server! Please try again later!";
        return { error };
    }

    std::vector<User> usersInfo;

    if (JSONRes["type"] == "user-success")
    {
        for (size_t i = 0; i < JSONRes["data"].size(); i += 7)
        {
            usersInfo.push_back({ 
                JSONRes["data"][0 + i],
                JSONRes["data"][1 + i],
                JSONRes["data"][2 + i],
                JSONRes["data"][3 + i],
                JSONRes["data"][4 + i],
                JSONRes["data"][5 + i],
                JSONRes["data"][6 + i]
            });
        }
    }

    return usersInfo;
}

OrgInfo APIHandler::getOrg(std::string name, std::string JWTToken)
{
    auto r = cpr::Get(cpr::Url{ "http://localhost:18080/api/orgs/" + name },
        cpr::Bearer({ JWTToken }));

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        // Send error
        OrgInfo error;
        error.errors = "There is a problem with the server! Please try again later!";
        return { error };
    }
    
    OrgInfo orgInfo;

    if (JSONRes["type"] == "get-organisation-success")
    {

        orgInfo.id = JSONRes["org-id"];
        orgInfo.name = JSONRes["org-name"];
        
        for (auto it = JSONRes["org-users"].begin(); it != JSONRes["org-users"].end(); ++it)
        {
            if (it.value()["role-id"] == 1)
            {
                OrgUser orgUser;

                orgUser.id = it.value()["user-id"];
                orgUser.role = it.value()["role-id"];

                for (auto courseIt = it.value()["teacher-courses-id"].begin(); 
                    courseIt != it.value()["teacher-courses-id"].end();
                    ++courseIt
                    )
                {
                    orgUser.userCoursesId.push_back(courseIt.value());
                }

                orgInfo.users.push_back(orgUser);
            }
            else
            {
                orgInfo.users.push_back(
                    {
                        it.value()["user-id"],
                        it.value()["role-id"]
                    }
                );
            }
        }

        for(auto it = JSONRes["org-courses"].begin(); it != JSONRes["org-courses"].end(); ++it)
        {
            orgInfo.courses.push_back(
                {
                    it.value()["course-id"],
                    orgInfo.id,
                    it.value()["course-name"]
                }
            );
        }
    }
    else
    {
        OrgInfo error;
        error.errors = "There is a problem with the server! Please try again later!";
        return { error };
    }

    return orgInfo;
}

CourseInfo APIHandler::getCourse(int id, std::string JWTToken)
{
    auto r = cpr::Get(cpr::Url{ "http://localhost:18080/api/courses/" + std::to_string(id) },
        cpr::Bearer({ JWTToken }));

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        // Send error
        CourseInfo error;
        error.errors = "There is a problem with the server! Please try again later!";
        return { error };
    }

    CourseInfo courseInfo;

    if (JSONRes["type"] == "get-course-success")
    {

        courseInfo.id = JSONRes["course-id"];
        courseInfo.orgId = JSONRes["org-id"];
        courseInfo.name = JSONRes["course-name"];

        for (auto it = JSONRes["course-users"].begin(); it != JSONRes["course-users"].end(); ++it)
        {
            courseInfo.users.push_back(
                {
                    it.value()["user-id"],
                    it.value()["role-id"]
                }
            );
        }

        for (auto it = JSONRes["course-topics"].begin(); it != JSONRes["course-topics"].end(); ++it)
        {
            courseInfo.topics.push_back(
                {
                    it.value()["topic-id"],
                    courseInfo.id,
                    it.value()["topic-name"]
                }
            );
        }
    }
    else
    {
        CourseInfo error;
        error.errors = "There is a problem with the server! Please try again later!";
        return { error };
    }

    return courseInfo;
}

TopicInfo APIHandler::getTopic(int id, std::string JWTToken)
{
    auto r = cpr::Get(cpr::Url{ "http://localhost:18080/api/topics/" + std::to_string(id) },
        cpr::Bearer({ JWTToken }));

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        // Send error
        TopicInfo error;
        error.errors = "There is a problem with the server! Please try again later!";
        return { error };
    }

    TopicInfo topicInfo;

    if (JSONRes["type"] == "get-topic-success")
    {

        topicInfo.id = JSONRes["topic-id"];
        topicInfo.name = JSONRes["topic-name"];
        topicInfo.courseId = JSONRes["course-id"];

        for (auto it = JSONRes["topic-quizzes"].begin(); it != JSONRes["topic-quizzes"].end(); ++it)
        {
            topicInfo.quizzes.push_back(
                {
                    it.value()["quiz-id"],
                    topicInfo.id,
                    it.value()["quiz-name"]
                }
            );
        }

        for (auto it = JSONRes["topic-lessons"].begin(); it != JSONRes["topic-lessons"].end(); ++it)
        {
            topicInfo.lessons.push_back(
                {
                    it.value()["lesson-id"],
                    topicInfo.id,
                    it.value()["lesson-name"]
                }
            );
        }
    }
    else
    {
        TopicInfo error;
        error.errors = "There is a problem with the server! Please try again later!";
        return { error };
    }

    return topicInfo;
}

LessonInfo APIHandler::getLesson(int id, std::string JWTToken)
{
    auto r = cpr::Get(cpr::Url{ "http://localhost:18080/api/lessons/" + std::to_string(id) },
        cpr::Bearer({ JWTToken }));

    nlohmann::json JSONRes;

    try
    {
        JSONRes = nlohmann::json::parse(r.text);
    }
    catch (nlohmann::json::parse_error& ex)
    {
        // Send error
        LessonInfo error;
        error.errors = "There is a problem with the server! Please try again later!";
        return { error };
    }

    LessonInfo lessonInfo;

    if (JSONRes["type"] == "get-lesson-success")
    {

        lessonInfo.id = JSONRes["lesson-id"];
        lessonInfo.name = JSONRes["lesson-name"];
        lessonInfo.data = JSONRes["lesson-data"];
        lessonInfo.topicId = JSONRes["topic-id"];
    }
    else
    {
        LessonInfo error;
        error.errors = "There is a problem with the server! Please try again later!";
        return { error };
    }

    return lessonInfo;
}