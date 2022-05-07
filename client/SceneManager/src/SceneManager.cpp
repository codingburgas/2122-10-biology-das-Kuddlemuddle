#include <SceneManager.h>
#include <windowsAPI.h>
#include <conio.h>
#include <APIHandler.h>

void printLogo(int posx) 
{
	outputPosition(posx, 0); std::cout << "     _             _  __         _     _ _                          _     _ _      " << std::endl;
	outputPosition(posx, 1); std::cout << "  __| | __ _ ___  | |/ /   _  __| | __| | | ___ _ __ ___  _   _  __| | __| | | ___ " << std::endl;
	outputPosition(posx, 2); std::cout << " / _` |/ _` / __| | ' / | | |/ _` |/ _` | |/ _ \\ '_ ` _ \\| | | |/ _` |/ _` | |/ _ \\" << std::endl;
	outputPosition(posx, 3); std::cout << "| (_| | (_| \\__ \\ | . \\ |_| | (_| | (_| | |  __/ | | | | | |_| | (_| | (_| | |  __/" << std::endl;
	outputPosition(posx, 4); std::cout << " \\__,_|\\__,_|___/ |_|\\_\\__,_|\\__,_|\\__,_|_|\\___|_| |_| |_|\\__,_|\\__,_|\\__,_|_|\\___|" << std::endl;
}

void createInputField(int posy, std::string text, int size, int posx, int col, std::string input = "", int col2 = 7)
{
	setConsoleColorTo(col);
	outputPosition(posx, posy); 
	std::cout << char(201);

	for (int i = 0; i < 14; i++)
	{
		std::cout << char(205);
	}

	std::cout << char(203);

	for (int i = 0; i < size - 14; i++)
	{
		std::cout << char(205);
	}

	std::cout << char(187);

	outputPosition(posx, posy + 1); std::cout << char(186);
	setConsoleColorTo(col2);

	std::cout << text << "  "; setConsoleColorTo(col); std::cout << char(186) << " ";
	setConsoleColorTo(col2);

	size_t inputLength = 0;

	if (input.size() > 24)
	{
		std::cout << input.substr(input.size() - 24, input.size());
		inputLength = 24;
	}

	else
	{
		std::cout << input;
		inputLength = input.size();
	}

	for (size_t i = 0; i < 25 - inputLength; i++)
	{
		std::cout << " ";
	}

	setConsoleColorTo(col); std::cout << char(186) << std::endl;
	outputPosition(posx, posy + 2); std::cout << char(200);

	for (int i = 0; i < 14; i++)
	{
		std::cout << char(205);
	}

	std::cout << char(202);

	for (int i = 0; i < size - 14; i++)
	{
		std::cout << char(205);
	}

	std::cout << char(188);
	setConsoleColorTo(6);
}

void SceneManager::LoadScenes()
{
	SceneManager* sceneManager = this;

	scenes.push_back(
		{
			1,
			"Register",
			[&]() 
			{
				clearConsole();
				char inputKey = ' ';
				int iPut = 0;
				std::string info[5] = { "", "", "", "", "" };
				bool firstShow = true;
				int colors[5] = { 7,7,7,7,7 };

				setConsoleColorTo(6);
			
				do
				{
					colors[iPut] = 6;

					if (firstShow)
					{
						printLogo(30);
						outputPosition(15, 7); std::cout << "MMMMMMMMMMMMMNX0kXXXXXXk0XNMMMMMMMMMMMMMMM" << std::endl;
						outputPosition(15, 8); std::cout << "MMMMMMMMMMMMMNX0kXXXXXXk0XNMMMMMMMMMMMMMMM" << std::endl;
					}

					createInputField(8, " First name ", 40, 65, colors[0], info[0]);

					if (firstShow)
					{
						outputPosition(15, 9); std::cout << "MMMMMMMMMNOd:'.            .':dONMMMMMMMMM" << std::endl;
						outputPosition(15, 10); std::cout << "MMMMMMW0o'                      'o0WMMMMMM" << std::endl;
						outputPosition(15, 11); std::cout << "MMMMW0c.                          .c0WMMMM" << std::endl;
						outputPosition(15, 12); std::cout << "MMMNd.                              .dNMMM" << std::endl;
					}

					createInputField(12, " Last name  ", 40, 65, colors[1], info[1]);

					if (firstShow)
					{
						outputPosition(15, 13); std::cout << "MMXl             ,oxkkxl'             lXMM" << std::endl;
						outputPosition(15, 14); std::cout << "MNo            .dNMMMMMMXc             oNM" << std::endl;
						outputPosition(15, 15); std::cout << "MO.            ;XMMMMMMMMO.            .OM" << std::endl;
						outputPosition(15, 16); std::cout << "Wl             .kWMMMMMMNo.             lW" << std::endl;
					}

					createInputField(16, " Username   ", 40, 65, colors[2], info[2]);

					if (firstShow)
					{
						outputPosition(15, 17); std::cout << "N:              .cx000Od;.              :N" << std::endl;
						outputPosition(15, 18); std::cout << "Nc             .;:lodooc:,.             cN" << std::endl;
						outputPosition(15, 19); std::cout << "Wd.          'dKWMMMMMMMMN0l.          .dW" << std::endl;
						outputPosition(15, 20); std::cout << "MK,         '0MMMMMMMMMMMMMWx.         ,KM" << std::endl;
					}

					createInputField(20, " Email      ", 40, 65, colors[3], info[3]);

					if (firstShow)
					{
						outputPosition(15, 21); std::cout << "MWk.        ;0WMMMMMMMMMMMMWk.        .kWM" << std::endl;
						outputPosition(15, 22); std::cout << "MMWk'       .,::::::::::::::,.       'kWMM" << std::endl;
						outputPosition(15, 23); std::cout << "MMMWKc.                            .cKMMMM" << std::endl;
						outputPosition(15, 24); std::cout << "MMMMMNO:.                        .:ONMMMMM" << std::endl;
					}

					createInputField(24, " Password   ", 40, 65, colors[4], info[4]);

					if (firstShow)
					{
						outputPosition(15, 25); std::cout << "MMMMMMMW0o;.                  .;o0WMMMMMMM" << std::endl;
						outputPosition(15, 26); std::cout << "MMMMMMMMMMWKkoc;,'......',;cokKWMMMMMMMMMM" << std::endl;
						outputPosition(15, 27); std::cout << "MMMMMMMMMMMMMMMWWNNXXNNWMMWMMMMMMMMMMMMMMM" << std::endl;
						outputPosition(15, 28); std::cout << "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM" << std::endl;
						setConsoleColorTo(6);
					}

					firstShow = false;

					inputKey = _getch();

					if (inputKey == '\r')
					{
						iPut++;
						inputKey = ' ';
					}
					else if (inputKey == '\t')
					{
						return "Login";
						//sceneManager->getSceneByName("Login")->drawScene();
					}
					else if (inputKey == '\b')
					{
						info[iPut] = info[iPut].substr(0, info[iPut].size() - 1);
					}
					else
					{
						info[iPut] += inputKey;
					}
				} while (inputKey != '\r' && iPut <= 4);

				RegisterData registerData = { info[0], info[1], info[2], info[3], info[4] };

				APIHandler apiHandler;
			
				std::string recordSet = apiHandler.registerHandler(registerData);

				if (recordSet.empty())
				{
					outputPosition(15, 31); std::cout << "The account was created successfully! Press any key to continue!";
				
					(void)_getch();
				
					return "Login";
				}
			
				outputPosition(15, 31); std::cout << recordSet;

				(void)_getch();
				
				return "Register";
			}
		}
	);

	scenes.push_back(
		{
			2,
			"Login",
			[&]()
			{
				system("cls");
				char inputKey = ' ';
				int iPut = 0;
				std::string info[2] = { "", "" };
				int colors[2] = { 7,7 };

				setConsoleColorTo(6);
				do {
					printLogo(30);

					colors[iPut] = 6;

					outputPosition(15, 7); std::cout << "MMMMMMMMMMMMMNX0kXXXXXXk0XNMMMMMMMMMMMMMMM" << std::endl;
					outputPosition(15, 8); std::cout << "MMMMMMMMMMMMMNX0kXXXXXXk0XNMMMMMMMMMMMMMMM" << std::endl;

					createInputField(8, " Email      ", 40, 65, colors[0], info[0]);

					outputPosition(15, 9); std::cout << "MMMMMMMMMNOd:'.            .':dONMMMMMMMMM" << std::endl;
					outputPosition(15, 10); std::cout << "MMMMMMW0o'                      'o0WMMMMMM" << std::endl;
					outputPosition(15, 11); std::cout << "MMMMW0c.                          .c0WMMMM" << std::endl;
					outputPosition(15, 12); std::cout << "MMMNd.                              .dNMMM" << std::endl;

					createInputField(12, " Password   ", 40, 65, colors[1], info[1]);

					outputPosition(15, 13); std::cout << "MMXl             ,oxkkxl'             lXMM" << std::endl;
					outputPosition(15, 14); std::cout << "MNo            .dNMMMMMMXc             oNM" << std::endl;
					outputPosition(15, 15); std::cout << "MO.            ;XMMMMMMMMO.            .OM" << std::endl;
					outputPosition(15, 16); std::cout << "Wl             .kWMMMMMMNo.             lW" << std::endl;
					outputPosition(15, 17); std::cout << "N:              .cx000Od;.              :N" << std::endl;
					outputPosition(15, 18); std::cout << "Nc             .;:lodooc:,.             cN" << std::endl;
					outputPosition(15, 19); std::cout << "Wd.          'dKWMMMMMMMMN0l.          .dW" << std::endl;
					outputPosition(15, 20); std::cout << "MK,         '0MMMMMMMMMMMMMWx.         ,KM" << std::endl;
					outputPosition(15, 21); std::cout << "MWk.        ;0WMMMMMMMMMMMMWk.        .kWM" << std::endl;
					outputPosition(15, 22); std::cout << "MMWk'       .,::::::::::::::,.       'kWMM" << std::endl;
					outputPosition(15, 23); std::cout << "MMMWKc.                            .cKMMMM" << std::endl;
					outputPosition(15, 24); std::cout << "MMMMMNO:.                        .:ONMMMMM" << std::endl;
					outputPosition(15, 25); std::cout << "MMMMMMMW0o;.                  .;o0WMMMMMMM" << std::endl;
					outputPosition(15, 26); std::cout << "MMMMMMMMMMWKkoc;,'......',;cokKWMMMMMMMMMM" << std::endl;
					outputPosition(15, 27); std::cout << "MMMMMMMMMMMMMMMWWNNXXNNWMMWMMMMMMMMMMMMMMM" << std::endl;
					outputPosition(15, 28); std::cout << "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM" << std::endl;

					setConsoleColorTo(6);

					inputKey = _getch();

					if (inputKey == '\r')
					{
						iPut++;
						inputKey = ' ';
					}

					if (inputKey == '\t')
					{
						return "Register";
					}

					else
					{
						if (inputKey == '\b')
						{
							info[iPut] = info[iPut].substr(0, info[iPut].size() - 1);
						}
						else
						{
							info[iPut] += inputKey;
						}
					}

				} while (inputKey != '\r' && iPut <= 1);
			}
		}
	);
}

Scene* SceneManager::getSceneByName(std::string sceneName)
{
	for (auto& scene: scenes)
	{
		if (scene.name == sceneName)
		{
			return &scene;
		}
	}

	return nullptr;
}

