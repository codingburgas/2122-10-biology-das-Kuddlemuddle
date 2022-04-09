#include <validations.h>

std::vector<std::string> ValidationManager::isRegisterDataValid(crow::query_string data, bool allowNulls)
{
	std::vector<std::string> incorrectValidation;

	std::vector<std::string> fields =
	{
		"firstName",
		"lastName",
		"username",
		"email",
		"password"
	};

	for (auto field : fields)
	{
		if (allowNulls)
		{
			if (std::string(data.get(field)) == "")
			{
				continue;
			}
		}

		if (!(getValidationHandler(field)(data.get(field))))
		{
			incorrectValidation.push_back(field);
		}
	}

	return incorrectValidation;
}

std::vector<std::string> ValidationManager::isLoginDataValid(crow::query_string data)
{
	std::vector<std::string> incorrectValidation;

	std::vector<std::string> fields =
	{
		"loginCredential",
		"password"
	};

	for (auto field : fields)
	{
		if (!(getValidationHandler(field)(data.get(field))))
		{
			incorrectValidation.push_back(field);
		}
	}

	return incorrectValidation;
}

ValidationHandler ValidationManager::getValidationHandler(std::string field)
{
	// Sorry, you cannot have string in switch statements :(
	if (field == "firstName" || field == "lastName")
	{
		return (
			[](std::string data) -> bool
			{
				const std::regex valRegex("^(?=.*[A-ZÀ-ßà-ÿa-z])([A-ZÀ-ß])([a-zà-ÿ]{2,29})$");

				return std::regex_match(data, valRegex);
			}
		);
	}
	else if (field == "username")
	{
		return (
			[](std::string data) -> bool
			{
				const std::regex valRegex("^(?=[a-zA-Z0-9._]{8,20}$)(?!.*[_.]{2})[^_.].*[^_.]$");

				return std::regex_match(data, valRegex);
			}
		);
	}
	else if (field == "email")
	{
		return (
			[](std::string data) -> bool
			{
				const std::regex valRegex("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");

				return std::regex_match(data, valRegex);
			}
		);
	}
	else if (field == "password")
	{
		return (
			[](std::string data) -> bool
			{
				const std::regex valRegex("(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])[a-zA-Z0-9]{8,}");

				return std::regex_match(data, valRegex);
			}
		);
	}
	else if (field == "loginCredential")
	{
		return (
			[](std::string data) -> bool
			{
				// Check if credential is email or username
				if (data.find("@") != std::string::npos)
				{
					const std::regex valRegex("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");

					return std::regex_match(data, valRegex);
				}
				else
				{
					const std::regex valRegex("^(?=[a-zA-Z0-9._]{8,20}$)(?!.*[_.]{2})[^_.].*[^_.]$");

					return std::regex_match(data, valRegex);
				}
			}
		);
	}
	else
	{
		return (
			[](std::string data) -> bool
			{
				return false;
			}
		);
	}
}
