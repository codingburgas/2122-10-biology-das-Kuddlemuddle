/*! @file application.cpp
*   @brief A source file for the application.
*/
#include <application.h>
#include <windows.h>
#include <conio.h>

void Application::init(
	SceneManager& sceneManager
)
{
	setCursorVisibilityTo(false);

	sceneManager.LoadScenes();
}

void Application::run(
	SceneManager& sceneManager
)
{
	// Based on that load the first scene
	std::string nextSceneName = "NavigationBar";
	
	while (true)
	{
		Scene* registerScene = sceneManager.getSceneByName(nextSceneName);

		nextSceneName = registerScene->drawScene();
	}
}
