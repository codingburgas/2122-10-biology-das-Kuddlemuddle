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
        user.fname = JSONRes["data"][0];
        user.lname = JSONRes["data"][1];
        user.username = JSONRes["data"][2];
        user.email = JSONRes["data"][3];
        user.role = JSONRes["data"][4];
        user.avatarURL = JSONRes["data"][5];
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
            orgsInfo.push_back({ it.value()["org-id"].get<int>(),it.value()["org-name"].get<std::string>() });
        }
    }

    return orgsInfo;
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
