#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <functional>

typedef std::function<std::string()> SceneHandler;

struct Scene
{
	int id;
	std::string name;
	SceneHandler drawScene;
};

struct SceneManager
{
	std::vector<Scene> scenes;
	void LoadScenes();
	Scene* getSceneByName(std::string sceneName);
};
