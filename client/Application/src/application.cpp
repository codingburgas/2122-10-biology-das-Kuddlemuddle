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
	// Hide the cursor
	setCursorVisibilityTo(false);

	sceneManager.LoadScenes();
}

void Application::run(
	SceneManager& sceneManager
)
{
	std::string nextSceneName = "Register";
	
	while (true)
	{
		Scene* registerScene = sceneManager.getSceneByName(nextSceneName);

		nextSceneName = registerScene->drawScene();
	}
}
