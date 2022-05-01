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

std::vector<std::string> ValidationManager::isOrgDataValid(crow::query_string data, bool allowNulls)
{
	std::vector<std::string> incorrectValidation;

	std::vector<std::string> fields =
	{
		"orgName",
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

std::vector<std::string> ValidationManager::isJoinOrgDataValid(crow::query_string data)
{
	std::vector<std::string> incorrectValidation;

	std::vector<std::string> fields =
	{
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

std::vector<std::string> ValidationManager::isCourseDataValid(crow::query_string data, bool allowNulls)
{
	std::vector<std::string> incorrectValidation;

	std::vector<std::string> fields =
	{
		"courseName",
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

std::vector<std::string> ValidationManager::isPunnettSquareDataValid(crow::query_string data, bool allowNulls)
{
	std::vector<std::string> incorrectValidation;

	std::vector<std::string> fields =
	{
		"P1",
		"P2"
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

bool isValueNumber(std::string str) 
{
	for (auto& el : str)
	{
		if (!isdigit(el))
		{
			return false;
		}
	}

	return true;
}

std::vector<std::string> ValidationManager::checkForNullValues(crow::query_string data, std::vector<std::string> fields, std::vector<std::string> numFields)
{
	std::vector<std::string> nullValues;

	for (auto field : fields)
	{
		if (!data.get(field))
		{
			nullValues.push_back(field);
		}
	}

	if (nullValues.empty())
	{
		for (auto field : numFields)
		{
			if (!isValueNumber(data.get(field)))
			{
				nullValues.push_back(field);
			}
		}
	}

	return nullValues;
}

ValidationHandler ValidationManager::getValidationHandler(std::string field)
{
	// Sorry, you cannot have string in switch statements :(
	if (field == "firstName" || field == "lastName")
	{
		return (
			[](std::string data) -> bool
			{
				const std::regex valRegex("^(?=.*[A-Za-z])([A-Z])([a-z]{2,29})$");

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
	else if (field == "orgName" || field == "courseName")
	{
		return (
			[](std::string data) -> bool
			{
				const std::regex valRegex("^[A-Z][A-Za-z0-9_-]{2,19}$");

				return std::regex_match(data, valRegex);
			}
		);
	}
	else if (field == "P1" || field == "P2")
	{
		return (
			[](std::string data) -> bool
			{
				if (data.size() != 4)
				{
					return false;
				}

				for (size_t i = 0; i < data.size(); i++)
				{
					if (i == 0 || i == 1)
					{
						if (data[i] != 'a' && data[i] != 'A')
						{
							return false;
						}
					}
					else
					{
						if (data[i] != 'b' && data[i] != 'B')
						{
							return false;
						}
					}
				}

				return true;
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
