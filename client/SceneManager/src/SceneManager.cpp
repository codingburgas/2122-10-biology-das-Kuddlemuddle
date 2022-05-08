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

void createInputField(int posy, std::string text, int size, int posx, int col, std::string input = "", int col2 = 7, int post = 14)
{
	setConsoleColorTo(col);
	outputPosition(posx, posy); 
	std::cout << char(201);

	for (int i = 0; i < post; i++)
	{
		std::cout << char(205);
	}

	std::cout << char(203);

	for (int i = 0; i < size - post; i++)
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

	for (int i = 0; i < post; i++)
	{
		std::cout << char(205);
	}

	std::cout << char(202);

	for (int i = 0; i < size - post; i++)
	{
		std::cout << char(205);
	}

	std::cout << char(188);
	setConsoleColorTo(6);
}

void createButton(int posy, std::string text, int size, int posx, int col, int col2 = 7)
{
	setConsoleColorTo(col); outputPosition(posx, posy); std::cout << char(201);

	for (int i = 0; i < size; i++)
	{
		std::cout << char(205);
	}
	std::cout << char(187);

	outputPosition(posx, posy + 1); std::cout << char(186); setConsoleColorTo(col2); std::cout << text; setConsoleColorTo(col); std::cout << char(186) << std::endl;
	outputPosition(posx, posy + 2); std::cout << char(200);

	for (int i = 0; i < size; i++)
	{
		std::cout << char(205);
	}

	std::cout << char(188); setConsoleColorTo(7);
}

void drawOrganisationLogo()
{
	outputPosition(15, 7); std::cout << "WWWWWWWWWWWWWWWWWWWWWWMWWMMWMMWWWWWWWWWWWWWWWWWWWWW";
	outputPosition(15, 8); std::cout << "WWWWWWWWWWWWWWWWWWWWWNkdddddddKWWWWWWWWWWWWWWWWWWWW";
	outputPosition(15, 9); std::cout << "WWWWWWWWWWWWWWWWWWWWW0'       :OKNWWWWWWWWWWWWWWWWW";
	outputPosition(15, 10); std::cout << "WWWWWWWWWWWWWWWWWWWWW0'         ;KWWWWWWWWWWWWWWWWW";
	outputPosition(15, 11); std::cout << "WWWWWWWWWWWWWWWWWWWWW0' ;kOo.   ,KWWWWWWWWWWWWWWWWW";
	outputPosition(15, 12); std::cout << "WWWWWWWWWWWWWWWWWWWWWO. .cokddddkNWWWWWWWWWWWWWWWWW";
	outputPosition(15, 13); std::cout << "WWWWWWWWWWWWWWWWWWXkl'     .,lkXWWWWWWWWWWWWWWWWWWW";
	outputPosition(15, 14); std::cout << "WWWWWWWWWWWWWWN0d:.            .:d0NWWWWWWWWWWWWWWW";
	outputPosition(15, 15); std::cout << "WWWWWWWWWWWXkl,.                  .,lkXWWWWWWWWWWWW";
	outputPosition(15, 16); std::cout << "WWWWWWWN0d:.                          .:d0NWWWWWWWW";
	outputPosition(15, 17); std::cout << "WWWWWWNo.                                .oNWWWWWWW";
	outputPosition(15, 18); std::cout << "WWWWWWNkl,    .c:.    ;lcll;    .:c.    ,lkNWWWWWWW";
	outputPosition(15, 19); std::cout << "WWWWWWWWWx.   ;XK,   .OWWWMO.   ,KX;   .xWWWWWWWWWW";
	outputPosition(15, 20); std::cout << "WWWWWWWWWx.   ;XK,   .OWWWWO.   ,KX;   .xWWWWWWWWWW";
	outputPosition(15, 21); std::cout << "WWWWWWWWWx.   ;XK,   .OWWWWO.   ,KX;   .xWWWWWWWWWW";
	outputPosition(15, 22); std::cout << "WWWWWWWWWx.   ;XK,   .OWWWWO.   ,KX;   .xWWWWWWWWWW";
	outputPosition(15, 23); std::cout << "WWWWWWWWWx.   ;XK,   .OWWWWO.   ,KX;   .xWWWWWWWWWW";
	outputPosition(15, 24); std::cout << "WWWWWWWWWx.   ;XK,   .OWWWWO.   ,KX;   .xWWWWWWWWWW";
	outputPosition(15, 25); std::cout << "WWWWWWWK0l    ,kx.   .o0OO0o.   .xk'    l0KWWWWWWWW";
	outputPosition(15, 26); std::cout << "WWWWWNK:               .                  :KNWWWWWW";
	outputPosition(15, 27); std::cout << "WWWWO;.                                    .;0WWWWW";
	outputPosition(15, 28); std::cout << "WWWWXxodoododoodoodoooodooodoodoooodoodoododxXWWWWW";
	outputPosition(15, 29); std::cout << "WWWWWWMWMWWMWWMWMMWMWWMWWMMWMWWMWWMWWMWMWWMWWWWWWWW";
}

void viewOrganisationAdmin(std::string JWTToken)
{
	clearConsole();
	char key; // Key to be entered
	int counter = 0, counter2 = 0;
	int ckey = 0;

	APIHandler apiHandler;

	auto orgsInfo = apiHandler.getAllOrgs(JWTToken);

	if (!orgsInfo[0].errors.empty())
	{
		std::cout << "There was problem with the server. Please try again latter!";
	}

	while (true)
	{
		int posy = 5;

		outputPosition(2, 3); setConsoleColorTo(6); std::cout << "O R G A N I S A T I O N S"; setConsoleColorTo(7);

		for (size_t i = 0; i < orgsInfo.size(); i++)
		{
			outputPosition(4, posy); std::cout << "-->";

			if (i == counter) 
			{
				setConsoleColorTo(6); outputPosition(9, posy); std::cout << orgsInfo[i].name;
				outputPosition(60, posy);

				if (counter2 == 0) 
				{
					setConsoleColorTo(2); std::cout << "Update"; setConsoleColorTo(7); std::cout << " | Delete";
				}
				else 
				{
					setConsoleColorTo(7); std::cout << "Update | "; setConsoleColorTo(4); std::cout << "Delete";
				}
			}
			else 
			{
				outputPosition(9, posy); std::cout << orgsInfo[i].name;
				outputPosition(60, posy); setConsoleColorTo(8); std::cout << "Update | Delete";
			}


			posy += 2;
			setConsoleColorTo(7);
		}

		key = _getch();

		if (key == 72 && (counter >= 1 && counter <= orgsInfo.size())) // 72/75 is the ASCII code for the up arrow
		{
			counter--;
		}

		if (key == 80 && (counter >= 0 && counter < orgsInfo.size() - 1)) // 80/77 is the ASCII code for the up arrow
		{
			counter++;
		}

		if (key == 75 && (counter2 == 1)) // 72/75 is the ASCII code for the up arrow
		{
			counter2--;
		}

		if (key == 77 && (counter2 == 0)) // 80/77 is the ASCII code for the up arrow
		{
			counter2++;
		}


	}
}

void joinOrganisation(int orgId, std::string JWTToken)
{
	clearConsole();
	char key = ' ';
	int iPut = 0;
	std::string info[5] = { "", "" };

	do
	{
		int colors[2] = { 7, 7 };
		colors[iPut] = 6;
		setConsoleColorTo(6);
		printLogo(26);
		drawOrganisationLogo();
		outputPosition(75, 9); std::cout << "You are not part of this organisation.";
		outputPosition(75, 10); std::cout << "Pleas enter the organisation password to continue!";
		createInputField(12, " Password   ", 40, 75, colors[1], info[0]);

		if (iPut == 1)
		{
			createButton(16, "   Join now   ", 14, 75, 2);
		}
		else {
			createButton(16, "   Join now   ", 14, 75, 7);
		}

		key = _getch();

		if (key == '\r')
		{
			iPut++;
			key = ' ';
		}

		else
		{
			if (iPut < 1)
			{
				if (key == '\b')
				{
					info[iPut] = info[iPut].substr(0, info[iPut].size() - 1);
				}

				else
				{
					info[iPut] += key;
				}
			}
		}

	} while (key != '\r' && iPut <= 1);

	APIHandler apiHandler;

	std::string recordSet = apiHandler.joinOrg(orgId, info[0], JWTToken);

	if (recordSet.empty())
	{
		outputPosition(15, 31); std::cout << "You are now part of this oraganisation! Press any key to continue!";

		(void)_getch();

		return;
	}

	outputPosition(15, 31); std::cout << recordSet;

	(void)_getch();

	return;
}

void createOrganisation(std::string JWTToken)
{
	clearConsole();
	char key = ' ';
	int iPut = 0;
	std::string info[5] = { "", "" };

	do
	{
		int colors[3] = { 7, 7 };
		colors[iPut] = 6;
		setConsoleColorTo(6);
		printLogo(26);
		drawOrganisationLogo();
		createInputField(8, " Organisation name", 46, 75, colors[0], info[0], 7, 20);
		createInputField(12, " Password         ", 46, 75, colors[1], info[1], 7, 20);


		if (iPut == 2)
		{
			createButton(16, "  Create now  ", 14, 75, 2);
		}
		else {
			createButton(16, "  Create now  ", 14, 75, 7);
		}

		key = _getch();

		if (key == 27)
		{
			return;
		}

		if (key == '\r')
		{
			iPut++;
			key = ' ';
		}
		else
		{
			if (iPut < 2)
			{
				if (key == '\b')
				{
					info[iPut] = info[iPut].substr(0, info[iPut].size() - 1);
				}

				else
				{
					info[iPut] += key;
				}
			}
		}

	} while (key != '\r' && iPut <= 2);

	APIHandler apiHandler;

	std::string recordSet = apiHandler.createOrg({ info[0], info[1] }, JWTToken);

	if (recordSet.empty())
	{
		outputPosition(15, 31); std::cout << "The organisation was created successfully! Press any key to continue!";

		(void)_getch();

		return;
	}

	outputPosition(15, 31); std::cout << recordSet;

	(void)_getch();

	return;
}

std::string viewOrganisationUser(std::string JWTToken)
{
	char key; // Key to be entered
	int counter = 0, posy;

	APIHandler apiHandler;

	auto orgsInfo = apiHandler.getAllOrgs(JWTToken);

	if (!orgsInfo[0].errors.empty())
	{
		std::cout << "There was problem with the server. Please try again latter!";
	}

	while (true)
	{
		outputPosition(6, 6); setConsoleColorTo(6); std::cout << "O R G A N I S A T I O N S"; setConsoleColorTo(7);
		posy = 8;

		for (int i = 0; i < orgsInfo.size(); i++)
		{
			outputPosition(8, posy); std::cout << "-->";
			i == counter ? setConsoleColorTo(6) : setConsoleColorTo(7);
			outputPosition(13, posy); std::cout << orgsInfo[i].name;
			setConsoleColorTo(7);

			posy += 2;
		}

		outputPosition(8, posy); std::cout << "-->";
		orgsInfo.size() == counter ? setConsoleColorTo(6) : setConsoleColorTo(7);
		outputPosition(13, posy); std::cout << "Create Organisation";
		setConsoleColorTo(7);

		key = _getch();

		if (key == 72 && (counter >= 1 && counter <= orgsInfo.size())) // 72 is the ASCII code for the up arrow
		{
			counter--;
		}

		if (key == 80 && (counter >= 0 && counter < orgsInfo.size())) // 80 is the ASCII code for the down arrow
		{
			counter++;
		}

		if (key == 27)
		{
			clearConsole();
			return "";
		}

		if (key == '\r' && counter == orgsInfo.size())
		{
			createOrganisation(JWTToken);
			clearConsole();
			return "";
		}
		
		if (key == '\r')
		{
			// try to get org info
			if (apiHandler.doUserHaveAccessToOrg(orgsInfo[counter].name, JWTToken))
			{
				return "NavigationBar";
			}
			else
			{
				joinOrganisation(orgsInfo[counter].id, JWTToken);
				clearConsole();
				return "";
			}
		}
	}
}

std::string accountPage(User user, std::string JWTToken)
{
	outputPosition(40, 6); setConsoleColorTo(6); std::cout << "A C C O U N T     S E T T I N G S"; setConsoleColorTo(7);
	outputPosition(43, 8); std::cout << "First name  ";
	outputPosition(58, 8); std::cout << user.fname;
	outputPosition(44, 10); std::cout << "Last name  ";
	outputPosition(58, 10); std::cout << user.lname;
	outputPosition(37, 12); std::cout << "Account Username  ";
	outputPosition(58, 12); std::cout << user.username;
	outputPosition(40, 14); std::cout << "Email address  ";
	outputPosition(58, 14); std::cout << user.email;
	outputPosition(45, 16); std::cout << "Role  ";
	
	if (user.role == "0")
	{
		outputPosition(58, 16); std::cout << "User";
	}
	else
	{
		outputPosition(58, 16); std::cout << "Admin";
	}

	char key; // Key to be entered
	int SetColor[2] = { 2, 7 };
	int counter = 0;
	int ckey = 0;
	APIHandler apiHandler;

	while (true)
	{
		// Button for history notebook section

		outputPosition(42, 18); setConsoleColorTo(SetColor[0]); std::cout << "U P D A T E";
		
		if (counter == 0)
		{
			outputPosition(54, 18); setConsoleColorTo(7); std::cout << "<--";
		}
		if (counter == 1)
		{
			outputPosition(54, 18); setConsoleColorTo(7); std::cout << "-->";
		}

		outputPosition(58, 18); setConsoleColorTo(SetColor[1]); std::cout << "D E L E T E";

		key = _getch();

		if (key == 75 && (counter == 1)) // 72 is the ASCII code for the up arrow
		{
			counter--;
		}

		if (key == 77 && (counter == 0)) // 80 is the ASCII code for the up arrow
		{
			counter++;
		}

		if (key == '\r' && (counter == 1)) // 72 is the ASCII code for the up arrow
		{
			counter--;
		}

		if (key == '\r' && (counter == 0)) // 80 is the ASCII code for the up arrow
		{
			std::string recordSet = apiHandler.deleteUser("@me", JWTToken);

			if (recordSet.empty())
			{
				return "Login";
			}
			else
			{
				outputPosition(25, 20); setConsoleColorTo(7); std::cout << recordSet;
				
				(void)_getch();
				
				return "";
			}
		}

		if (key == 27)
		{
			clearConsole();
			return "";
		}

		SetColor[0] = 7;
		SetColor[1] = 7;

		if (counter == 0) { SetColor[0] = 2; }
		if (counter == 1) { SetColor[1] = 4; }
	}

}

std::string adminPage(std::string JWTToken)
{
	// Key to be entered
	char key; 
	int indexCounter = 0;
	int selectedCounter = 0;

	APIHandler apiHandler;

	auto orgsInfo = apiHandler.getAllOrgs(JWTToken);

	if (!orgsInfo[0].errors.empty())
	{
		std::cout << "There was problem with the server. Please try again latter!";
	}

	auto usersInfo = apiHandler.getAllUsers(JWTToken);

	while (true)
	{
		if (selectedCounter == 0)
		{
			createButton(5, "        O R G A N I Z A T I O N S        ", 41, 15, 6);
			createButton(5, "            A L L   U S E R S            ", 41, 70, 8, 8);
		}

		else
		{
			createButton(5, "        O R G A N I Z A T I O N S        ", 41, 15, 8, 8);
			createButton(5, "            A L L   U S E R S            ", 41, 70, 6);
		}

		for (int i = 0; i < orgsInfo.size(); i++)
		{
			selectedCounter == 0 ? indexCounter == i ? setConsoleColorTo(6) : setConsoleColorTo(7) : setConsoleColorTo(8);
			outputPosition(15, 8 + i); std::cout << char(186) << " " << orgsInfo[i].name;
		}

		for (int i = 0; i < usersInfo.size(); i++)
		{
			selectedCounter == 1 ? indexCounter == i ? setConsoleColorTo(6) : setConsoleColorTo(7) : setConsoleColorTo(8);
			outputPosition(70, 8 + i); std::cout << char(186) << " " << usersInfo[i].username;
		}

		key = _getch();

		if (key == 72 && (indexCounter >= 1 && indexCounter <= orgsInfo.size() - 1))
		{
			indexCounter--;
		}

		if (key == 80 && (indexCounter >= 0 && indexCounter < usersInfo.size() - 1))
		{
			indexCounter++;
		}

		if (key == '\r' && (selectedCounter == 1))
		{
			std::string recordSet = apiHandler.deleteUser(usersInfo[indexCounter].username, JWTToken);

			if (recordSet.empty())
			{
				clearConsole();
				return "";
			}

			std::cout << recordSet;
		}

		if (key == 27)
		{
			clearConsole();
			return "";
		}

		if (key == 75 && (selectedCounter == 1))
		{
			selectedCounter--;
		}

		if (key == 77 && (selectedCounter == 0)) // 80/77 is the ASCII code for the up arrow
		{
			selectedCounter++;
		}
	}
}

void SceneManager::LoadScenes()
{
	SceneContex* sceneContext = new SceneContex();

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
				clearConsole();
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

					createInputField(8, " Email/Username ", 44, 65, colors[0], info[0], 7, 18);

					outputPosition(15, 9); std::cout << "MMMMMMMMMNOd:'.            .':dONMMMMMMMMM" << std::endl;
					outputPosition(15, 10); std::cout << "MMMMMMW0o'                      'o0WMMMMMM" << std::endl;
					outputPosition(15, 11); std::cout << "MMMMW0c.                          .c0WMMMM" << std::endl;
					outputPosition(15, 12); std::cout << "MMMNd.                              .dNMMM" << std::endl;

					createInputField(12, " Password       ", 44, 65, colors[1], info[1], 7, 18);

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
					else if (inputKey == '\t')
					{
						return "Register";
					}
					else if (inputKey == '\b')
					{
						info[iPut] = info[iPut].substr(0, info[iPut].size() - 1);
					}
					else
					{
						info[iPut] += inputKey;
					}
				} while (inputKey != '\r' && iPut <= 1);

				LoginData loginData = { info[0], info[1] };
				
				APIHandler apiHandler;

				std::string recordSet = apiHandler.loginHandler(loginData, sceneContext);

				if (recordSet.empty())
				{
					outputPosition(15, 31); std::cout << "The account was created successfully! Press any key to continue!";

					(void)_getch();

					return "NavigationBar";
				}

				outputPosition(15, 31); std::cout << recordSet;

				(void)_getch();

				return "Login";
			}
		}
	);
	
	scenes.push_back(
		{
			3,
			"NavigationBar",
			[&]() -> std::string
			{
				clearConsole();
				char inputKey; // Key to be entered
				int SetColor[5] = { 2, 7, 7, 7, 7 };
				int counter = 1;
				int ckey = 0;
				
				// Fetch the data from the server

				APIHandler apiHandler;
				User userData;

				sceneContext->isAuth = true;
				sceneContext->JWTToken = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXUyJ9.eyJleHAiOjE2NTIwNDk0MDYsImlhdCI6MTY1MTk2MzAwNiwiaXNBZG1pbiI6MCwic3ViIjoiMyJ9.tCmIx03JTOKclF93MztFkcTEg4ZCNt16V1pwtNVzCmE";

				std::string recordSet = apiHandler.getUserInfo("@me", sceneContext, userData);

				if (!recordSet.empty())
				{
					outputPosition(0, 1); std::cout << recordSet;

					(void)_getch();

					return "Login";
				}

				while (true)
				{
					// Button for history notebook section
					outputPosition(0, 1); std::cout << "     Kuddlemuddle  ";

					setConsoleColorTo(SetColor[0]);
					createButton(1, "       MY PROFILE       ", 24, 5, SetColor[0]);
					setConsoleColorTo(7);

					// Button for timeline section
					setConsoleColorTo(SetColor[1]);

					if (userData.role == "0")
					{
						createButton(1, "      ORGANIZATIONS     ", 24, 30, SetColor[1]);
					}
					else
					{
						createButton(1, "      ADMIN PAGE        ", 24, 30, SetColor[1]);
					}
					setConsoleColorTo(7);

					setConsoleColorTo(SetColor[2]);
					createButton(1, "        LOG OUT         ", 24, 100, SetColor[2]);
					setConsoleColorTo(7);

					for (int i = 0; i < 130; i++)
					{
						outputPosition(i, 4); std::cout << char(205);
					}

					inputKey = _getch();

					if (inputKey == 75 && (counter >= 2 && counter <= 3)) // 72 is the ASCII code for the up arrow
					{
						counter--;
					}

					if (inputKey == 77 && (counter >= 1 && counter <= 2)) // 80 is the ASCII code for the up arrow
					{
						counter++;
					}

					if (inputKey == '\r' && ckey % 2 == 0) // enter key
					{
						if (counter == 1)
						{
							recordSet = accountPage(userData, sceneContext->JWTToken);

							if (!recordSet.empty())
							{
								if (recordSet == "Login")
								{
									sceneContext->isAuth = false;
									sceneContext->JWTToken = "";
								}

								return recordSet;
							}
						}

						if (counter == 2)
						{
							if (userData.role == "1")
							{
								recordSet = adminPage(sceneContext->JWTToken);

								if (!recordSet.empty())
								{
									if (recordSet == "Login")
									{
										sceneContext->isAuth = false;
										sceneContext->JWTToken = "";
									}

									return recordSet;
								}
							}
							else
							{
								recordSet = viewOrganisationUser(sceneContext->JWTToken);

								if (!recordSet.empty())
								{
									if (recordSet == "Login")
									{
										sceneContext->isAuth = false;
										sceneContext->JWTToken = "";
									}

									return recordSet;
								}
							}
						}

						if (counter == 3)
						{
							sceneContext->isAuth = false;
							sceneContext->JWTToken = "";
							
							return "Login";
						}

						if (counter == 4)
						{
							break;
						}

					}

					SetColor[0] = 7;
					SetColor[1] = 7;
					SetColor[2] = 7;
					SetColor[3] = 7;

					if (inputKey == '\t')
					{
						ckey++;
					}

					if (ckey % 2 == 0)
					{
						if (counter == 1) { SetColor[0] = 2; }
						if (counter == 2) { SetColor[1] = 2; }
						if (counter == 3) { SetColor[2] = 2; }
						if (counter == 4) { SetColor[3] = 12; }
					}

				}

				return "NavigationBar";
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

