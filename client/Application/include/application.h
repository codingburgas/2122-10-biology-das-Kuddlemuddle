/*! @file application.h
*   @brief A header file for the application.
*/

#pragma once
//#include <back-end/accountSystem.h>
//#include <back-end/eventManager.h>
//#include <back-end/datetimeManager.h>
#include <logs.h>
#include <errorManager.h>
#include <windowsAPI.h>
#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <SceneManager.h>

/**
 * @brief Main struct for the applications
*/
struct Application
{
	/**
	 * @brief Scene manager
	*/
	SceneManager sceneManager;

	/**
	 * @brief Function for initialising
	 * @param sceneManager Scene manager
	*/
	void init(
		SceneManager& sceneManager
	);

	/**
	 * @brief Entry point for the application
	 * @param sceneManager Scene manager
	*/
	void run(
		SceneManager& sceneManager
	);
};