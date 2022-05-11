/*! @file validations.h
*   @brief A header file for the validation system.
*/

/*!	@var typedef bool (*ValidationHandler)(std::string data);
	@brief A type definition for a function pointer for validating data.
*/

#pragma once
#include <crow.h>
#include <regex>

typedef bool (*ValidationHandler)(std::string data);

/**
 * @brief Class for validation system
*/
class ValidationManager
{
public:
	/**
	 * @brief Function for checking if register data matches criteria
	 * @param data The data received from the request
	 * @param allowNulls To allow empty data
	 * @return The name of filed with validation errors
	*/
	std::vector<std::string> isRegisterDataValid(crow::query_string data, bool allowNulls = false);
	
	/**
	 * @brief Function for checking if login data matches criteria
	 * @param data The data received from the request
	 * @return The name of filed with validation errors
	*/
	std::vector<std::string> isLoginDataValid(crow::query_string data);
	
	/**
	 * @brief Function for checking if organisation data matches criteria
	 * @param data The data received from the request
	 * @param allowNulls To allow empty data
	 * @return The name of filed with validation errors
	*/
	std::vector<std::string> isOrgDataValid(crow::query_string data, bool allowNulls = false);
	
	/**
	 * @brief Function for checking if organisation data matches criteria
	 * @param data The data received from the request
	 * @return The name of filed with validation errors
	*/
	std::vector<std::string> isJoinOrgDataValid(crow::query_string data);
	
	/**
	 * @brief Function for checking if course data matches criteria
	 * @param data The data received from the request
	 * @param allowNulls To allow empty data
	 * @return The name of filed with validation errors
	*/
	std::vector<std::string> isCourseDataValid(crow::query_string data, bool allowNulls = false);
	
	/**
	 * @brief Function for checking if Punnett square data matches criteria
	 * @param data The data received from the request
	 * @param allowNulls To allow empty data
	 * @return The name of filed with validation errors
	*/
	std::vector<std::string> isPunnettSquareDataValid(crow::query_string data, bool allowNulls = false);
	
	/**
	 * @brief Function for checking is value NULL or nullptr
	 * @param data The data received from the request
	 * @param fields The fields to be checked
	 * @param numFields The fields to be cheked for numbers
	 * @return The name of filed with validation errors
	*/
	std::vector<std::string> checkForNullValues(
		crow::query_string data,
		std::vector<std::string> fields,
		std::vector<std::string> numFields = {}
	);
private:
	/**
	 * @brief Function that returns lambda function for validation a field
	 * @param Field The field to be validated
	 * @return Lambda function based on the field
	*/
	ValidationHandler getValidationHandler(std::string field);

	/**
	 * @brief Checks if values is integer type
	 * @param str The string to be checked
	 * @return Is the value from integer type
	*/
	bool isValueNumber(std::string str);
};
