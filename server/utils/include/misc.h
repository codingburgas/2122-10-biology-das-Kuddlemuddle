/*! @file misc.h
*   @brief A header file for miscellaneous functions.
*/

#pragma once
#include <string>
#include <vector>
#include <algorithm>

/**
 * @brief Function to make a string into a lower case
 * @param input String
 * @return The string in lower case
*/
std::string toLowerCase(std::string input);

/**
 * @brief Function for generating all possible combinations when mixing two parents
 * @param P1 Parent 1 (In format AABB)
 * @param P2 Parent 2 (In format AABB)
 * @return All possible combinations
*/
std::vector<std::string> convertP1andP2toAnswers(std::string P1, std::string P2);