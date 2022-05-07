/*! @file windowsAPI.h
*   @brief A header file for communicating with the windows API
*/

#pragma once
#include <Windows.h>

/**
 * @brief Function for setting the output position
 * @param x X coordinate
 * @param y Y coordinate
*/
void outputPosition(int x, int y);

/**
 * @brief Function for setting the output color
 * @param color The code of the color
*/
void setConsoleColorTo(int color);

/**
 * @brief Function for showing/hideing the cursor
 * @param shown is shown
*/
void setCursorVisibilityTo(bool shown);

/**
 * @brief Function for clearing the console
*/
void clearConsole();