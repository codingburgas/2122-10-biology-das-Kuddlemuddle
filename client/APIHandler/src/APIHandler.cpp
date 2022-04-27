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

    std::string returnVal = "";

    for (auto& el: JSONRes["fields"].items())
    {
        returnVal += el.value();
    }

    return returnVal;
}

std::string APIHandler::loginHandler(LoginData logData, LayerContex* ctx)
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

    std::string returnVal = "";

    for (auto& el : JSONRes["fields"].items())
    {
        returnVal += el.value();
    }

    return returnVal;
}

std::string APIHandler::getUserInfo(std::string userId, LayerContex* ctx, User &user)
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

void APIHandler::getImage(std::string& url, std::string fileExtension)
{
    auto ofstream = std::ofstream( "tmp." + fileExtension, std::ofstream::binary);
    auto session = cpr::Session();
    session.SetUrl(cpr::Url{ url });
    auto response = session.Download(ofstream);
    ofstream.close();
}
