#include <authorisationMiddleware.h>
#include <jwt-cpp/jwt.h>
#include <env.h>
#include <jwt-cpp/traits/nlohmann-json/traits.h>

void AuthorisationMiddleware::before_handle(crow::request& req, crow::response& res, context& ctx)
{
	EnvManager envManager;

	auto verifier = jwt::verify<jwt::traits::nlohmann_json>()
		.allow_algorithm(jwt::algorithm::hs256(envManager.getEnv("JWT_SECRET")));

	std::string myauth = req.get_header_value("Authorization");

	// If values is missing
	if (myauth == "")
	{
		CROW_LOG_WARNING << "Failed to authorise user. Reason: Authorization header is missing!";
		res = crow::response(401);
		res.end();
		return;
	}

	// Remove the Bearer part
	std::string mycreds = myauth.substr(7);

	// Decode and verify the token
	try
	{
		auto decoded = jwt::decode<jwt::traits::nlohmann_json>(mycreds);
		verifier.verify(decoded);
		ctx.isAdmin = decoded.get_payload_claim("isAdmin").as_int();
		ctx.userId = std::stoi(decoded.get_payload_claim("sub").as_string());
	}
	catch (...)
	{
		CROW_LOG_WARNING << "Failed to authorise user. Reason: Token is invalid!";
		res = crow::response(403);
		res.end();
		return;
	}
}

void AuthorisationMiddleware::after_handle(crow::request& req, crow::response& res, context& ctx)
{}
