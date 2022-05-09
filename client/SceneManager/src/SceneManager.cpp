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

void manageAccount(RegisterData userData, std::string JWTToken, SceneContex* ctx)
{
	clearConsole();
	char key;
	outputPosition(5, 4); std::cout << "M A N A G E   A C C O U N T ";

	while (true)
	{
		createInputField(6, " First Name ", 40, 5, 6, userData.fname);
		createInputField(10, " Last Name  ", 40, 5, 7, userData.lname);
		createInputField(14, " Username   ", 40, 5, 7, userData.username);
		createInputField(18, " Email      ", 40, 5, 7, userData.email);
		createInputField(22, " Password   ", 40, 5, 7, userData.password);
		createButton(26, "    SUBMIT    ", 14, 5, 2);

		key = _getch();

		if (key == '\r')
		{
			break;
		}

		key == '\b' ? userData.fname = userData.fname.substr(0, userData.fname.size() - 1) : userData.fname += key;
	}

	while (true)
	{
		createInputField(6, " First Name ", 40, 5, 7, userData.fname);
		createInputField(10, " Last Name  ", 40, 5, 6, userData.lname);
		createInputField(14, " Username   ", 40, 5, 7, userData.username);
		createInputField(18, " Email      ", 40, 5, 7, userData.email);
		createInputField(22, " Password   ", 40, 5, 7, userData.password);
		createButton(26, "    SUBMIT    ", 14, 5, 2);

		key = _getch();

		if (key == '\r')
		{
			break;
		}

		key == '\b' ? userData.lname = userData.lname.substr(0, userData.lname.size() - 1) : userData.lname += key;
	}

	while (true)
	{
		createInputField(6, " First Name ", 40, 5, 7, userData.fname);
		createInputField(10, " Last Name  ", 40, 5, 7, userData.lname);
		createInputField(14, " Username   ", 40, 5, 6, userData.username);
		createInputField(18, " Email      ", 40, 5, 7, userData.email);
		createInputField(22, " Password   ", 40, 5, 7, userData.password);
		createButton(26, "    SUBMIT    ", 14, 5, 2);

		key = _getch();

		if (key == '\r')
		{
			break;
		}

		key == '\b' ? userData.username = userData.username.substr(0, userData.username.size() - 1) : userData.username += key;
	}

	while (true)
	{
		createInputField(6, " First Name ", 40, 5, 7, userData.fname);
		createInputField(10, " Last Name  ", 40, 5, 7, userData.lname);
		createInputField(14, " Username   ", 40, 5, 7, userData.username);
		createInputField(18, " Email      ", 40, 5, 6, userData.email);
		createInputField(22, " Password   ", 40, 5, 7, userData.password);
		createButton(26, "    SUBMIT    ", 14, 5, 2);

		key = _getch();

		if (key == '\r')
		{
			break;
		}

		key == '\b' ? userData.email = userData.email.substr(0, userData.email.size() - 1) : userData.email += key;
	}

	while (true)
	{
		createInputField(6, " First Name ", 40, 5, 7, userData.fname);
		createInputField(10, " Last Name  ", 40, 5, 7, userData.lname);
		createInputField(14, " Username   ", 40, 5, 7, userData.username);
		createInputField(18, " Email      ", 40, 5, 7, userData.email);
		createInputField(22, " Password   ", 40, 5, 6, userData.password);
		createButton(26, "    SUBMIT    ", 14, 5, 2);

		key = _getch();

		if (key == '\r')
		{
			break;
		}

		key == '\b' ? userData.password = userData.password.substr(0, userData.password.size() - 1) : userData.password += key;
	}

	APIHandler apiHandler;

	auto recordSet = apiHandler.updateUser(userData, JWTToken);

	if (recordSet.empty())
	{
		outputPosition(40, 28); std::cout << "The user info was updated successfully! Press any key to continue!";

		ctx->user.username = userData.username;

		(void)_getch();

		return;
	}

	outputPosition(40, 28); std::cout << recordSet;

	(void)_getch();

	return;
}

void joinOrganisation(int orgId, std::string JWTToken)
{
	clearConsole();
	char key = ' ';
	int iPut = 0;
	std::string info[5] = { "", "" };

	do
	{
		int setConsoleColorTos[2] = { 7, 7 };
		setConsoleColorTos[iPut] = 6;
		setConsoleColorTo(6);
		printLogo(26);
		drawOrganisationLogo();
		outputPosition(75, 9); std::cout << "You are not part of this organisation.";
		outputPosition(75, 10); std::cout << "Pleas enter the organisation password to continue!";
		createInputField(12, " Password   ", 40, 75, setConsoleColorTos[1], info[0]);

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

		if (key == 27)
		{
			clearConsole();
			return;
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

std::string viewOrganisation(std::string JWTToken, SceneContex* sceneContext)
{
	char key; // Key to be entered
	int counter = 0, posy, counter2 = 0;

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
			if (orgsInfo[i].isAdmin)
			{
				outputPosition(4, posy); std::cout << "-->";

				if (i == counter)
				{
					setConsoleColorTo(6); outputPosition(9, posy); std::cout << orgsInfo[i].name;
					outputPosition(60, posy);

					if (counter2 == 0)
					{
						setConsoleColorTo(2); std::cout << "Enter"; setConsoleColorTo(7); std::cout << " | Delete";
					}
					else
					{
						setConsoleColorTo(7); std::cout << "Enter | "; setConsoleColorTo(4); std::cout << "Delete";
					}
				}
				else
				{
					outputPosition(9, posy); std::cout << orgsInfo[i].name;
					outputPosition(60, posy); setConsoleColorTo(8); std::cout << "Enter | Delete";
				}
				setConsoleColorTo(7);
			}
			else
			{
				outputPosition(4, posy); std::cout << "-->";
				i == counter ? setConsoleColorTo(6) : setConsoleColorTo(7);
				outputPosition(9, posy); std::cout << orgsInfo[i].name;
				setConsoleColorTo(7);
			}

			posy += 2;
		}

		outputPosition(4, posy); std::cout << "-->";
		orgsInfo.size() == counter ? setConsoleColorTo(6) : setConsoleColorTo(7);
		outputPosition(9, posy); std::cout << "Create Organisation";
		setConsoleColorTo(7);

		key = _getch();

		if (key == 72 && (counter >= 1 && counter <= orgsInfo.size())) // 72 is the ASCII code for the up arrow
		{
			counter--;
			if (!orgsInfo[counter].isAdmin)
			{
				counter2 = 0;
			}
		}

		if (key == 80 && (counter >= 0 && counter < orgsInfo.size())) // 80 is the ASCII code for the down arrow
		{
			counter++;
			if (!orgsInfo[counter].isAdmin)
			{
				counter2 = 0;
			}
		}

		if (key == 75 && (counter2 == 1)) // 72/75 is the ASCII code for the up arrow
		{
			counter2--;
			if (!orgsInfo[counter].isAdmin)
			{
				counter2 = 0;
			}
		}

		if (key == 77 && (counter2 == 0)) // 80/77 is the ASCII code for the up arrow
		{
			counter2++;
			if (!orgsInfo[counter].isAdmin)
			{
				counter2 = 0;
			}
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
		
		if (key == '\r' && counter2 == 0)
		{
			auto orgInfo = apiHandler.getOrg(orgsInfo[counter].name, JWTToken);
			// try to get org info
			if (orgInfo.errors.empty())
			{
				sceneContext->orgInfo = orgInfo;
				
				int userRole = 0;

				apiHandler.getUserInfo("@me", sceneContext, sceneContext->user);

				for (auto& user : orgInfo.users)
				{
					if (user.id == std::stoi(sceneContext->user.id))
					{
						userRole = user.role;
					}
				}

				switch (userRole)
				{
				case 1:
					return "ViewOrgAsTeacher";
					break;
				case 2:
					return "ViewOrgAsAdmin";
					break;
				case 0:
				default:
					return "ViewOrgAsUser";
					break;
				}
			}
			else
			{
				joinOrganisation(orgsInfo[counter].id, JWTToken);
				clearConsole();
				return "";
			}
		}

		if (key == '\r' && counter2 == 1)
		{
			auto recordSet = apiHandler.deleteOrg(orgsInfo[counter].name, JWTToken);

			outputPosition(4, posy + 2);

			if (recordSet.empty())
			{
				std::cout << "Organisation deleted successfully! Press any key to continue...";
				(void)_getch();
				clearConsole();
				return "";
			}

			std::cout << recordSet;
		}
	}
}

void createCourse(std::string JWTToken, int orgId)
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
		createInputField(8, " Course name", 40, 75, colors[0], info[0]);
		createInputField(12, " Password   ", 40, 75, colors[1], info[1]);

		if (iPut == 2)
		{
			createButton(16, "  Create now  ", 14, 75, 2);
		}
		else {
			createButton(16, "  Create now  ", 14, 75, 7);
		}

		key = _getch();

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

	std::string recordSet = apiHandler.createCourse({ info[0], info[1] }, orgId, JWTToken);

	if (recordSet.empty())
	{
		outputPosition(15, 31); std::cout << "The course was created successfully! Press any key to continue!";

		(void)_getch();

		return;
	}

	outputPosition(15, 31); std::cout << recordSet;

	(void)_getch();

	return;

}

void joinCourse(int courseId, std::string JWTToken)
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
		outputPosition(75, 9); std::cout << "You are not part of this course.";
		outputPosition(75, 10); std::cout << "Pleas enter the course password to continue!";
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

	std::string recordSet = apiHandler.joinCourse(courseId, info[0], JWTToken);

	if (recordSet.empty())
	{
		outputPosition(15, 31); std::cout << "You are now part of this course! Press any key to continue!";

		(void)_getch();

		return;
	}

	outputPosition(15, 31); std::cout << recordSet;

	(void)_getch();

	return;
}

void manageCourse(std::string courseName, int courseId, std::string JWTToken, SceneContex* ctx)
{
	clearConsole();
	char key;
	outputPosition(5, 4); std::cout << "M A N A G E   C O U R S E ";

	std::string password;

	while (true)
	{
		createInputField(6, " Course name", 40, 5, 6, courseName);
		createInputField(10, " Password   ", 40, 5, 7, password);
		createButton(14, "    SUBMIT    ", 14, 5, 2);

		key = _getch();

		if (key == '\r')
		{
			break;
		}

		key == '\b' ? courseName = courseName.substr(0, courseName.size() - 1) : courseName += key;
	}

	while (true)
	{
		createInputField(6, " Course name", 40, 5, 7, courseName);
		createInputField(10, " Password   ", 40, 5, 6, password);
		createButton(14, "    SUBMIT    ", 14, 5, 2);

		key = _getch();

		if (key == '\r')
		{
			break;
		}

		key == '\b' ? password = password.substr(0, password.size() - 1) : password += key;
	}

	APIHandler apiHandler;

	std::string recordSet = apiHandler.updateCourse({ courseName , password }, courseId, JWTToken);

	if (recordSet.empty())
	{
		outputPosition(15, 31); std::cout << "The course info was updated successfully! Press any key to continue!";

		ctx->courseInfo.name = courseName;

		(void)_getch();

		return;
	}

	outputPosition(15, 31); std::cout << recordSet;

	(void)_getch();
}

std::string accountPage(User user, std::string JWTToken, SceneContex* ctx)
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

		if (key == '\r' && (counter == 0)) // 72 is the ASCII code for the up arrow
		{
			manageAccount({user.fname, user.lname, user.username, user.email}, JWTToken, ctx);
			clearConsole();
			return "NavigationBar";
		}

		if (key == '\r' && (counter == 1)) // 80 is the ASCII code for the up arrow
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

		if (selectedCounter == 0)
		{
			if (key == 72 && (indexCounter >= 1 && indexCounter <= orgsInfo.size() + 1))
			{
				indexCounter--;
			}

			if (key == 80 && (indexCounter >= 0 && indexCounter < orgsInfo.size() - 1))
			{
				indexCounter++;
			}
		}
		else
		{
			if (key == 72 && (indexCounter >= 1 && indexCounter <= usersInfo.size() + 1))
			{
				indexCounter--;
			}

			if (key == 80 && (indexCounter >= 0 && indexCounter < usersInfo.size() - 1))
			{
				indexCounter++;
			}
		}

		if (key == '\r' && (selectedCounter == 1))
		{
			std::string recordSet = apiHandler.deleteUser(usersInfo[indexCounter].username, JWTToken);

			outputPosition(15, orgsInfo.size() + 10);
			
			if (recordSet.empty())
			{
				std::cout << "User deleted successfully! Press any key to continue...";
				(void)_getch();
				clearConsole();
				return "";
			}

			std::cout << recordSet;
		}

		if (key == '\r' && (selectedCounter == 0))
		{
			std::string recordSet = apiHandler.deleteOrg(orgsInfo[indexCounter].name, JWTToken);

			outputPosition(15, orgsInfo.size() + 10);

			if (recordSet.empty())
			{
				std::cout << "Organisation deleted successfully! Press any key to continue...";
				(void)_getch();
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
			if (indexCounter > orgsInfo.size() - 1)
			{
				indexCounter = orgsInfo.size() - 1;
			}
		}

		if (key == 77 && (selectedCounter == 0)) // 80/77 is the ASCII code for the up arrow
		{
			selectedCounter++;
			if (indexCounter > usersInfo.size() - 1)
			{
				indexCounter = usersInfo.size() - 1;
			}
		}
	}
}

void addTeacherToCourse(int courseId, std::string JWTToken, SceneContex* ctx)
{
	clearConsole();
	char key; // Key to be entered
	int counter = 0;

	APIHandler apiHandler;

	//CourseInfo courseInfo = apiHandler.getCourse(courseId, JWTToken);
	/*
	if (!courseInfo.errors.empty())
	{
		std::cout << "There was problem with the server. Please try again latter!";

		(void)_getch();
		return;
	}
	*/

	std::vector<User> usernames;

	for (int i = 0; i < ctx->orgInfo.users.size(); i++)
	{
		if (ctx->orgInfo.users[i].role == 1)
		{
			User user;
			apiHandler.getUserInfoById(std::to_string(ctx->orgInfo.users[i].id), ctx, user);

			usernames.push_back({ user.id, user.username });
		}
	}

	while (true)
	{
		// Button for history notebook section
		int posy = 5;
		outputPosition(2, 3); setConsoleColorTo(6); std::cout << "M A N A G E   C O U R S E "; setConsoleColorTo(7);
		
		for (int i = 0; i < usernames.size(); i++)
		{
			outputPosition(4, posy); std::cout << "-->";
			if (i == counter)
			{
				setConsoleColorTo(6); outputPosition(9, posy); std::cout << usernames[i].fname;
				outputPosition(60, posy);
			}
			else
			{
				outputPosition(9, posy); std::cout << usernames[i].fname;
			}


			posy += 2;
			setConsoleColorTo(7);
		}

		key = _getch();

		if (key == 27)
		{
			return;
		}

		if (key == '\r')
		{
			if (usernames.size() == 0)
			{
				return;
			}

			auto recordSet = apiHandler.addTeacherToCourse(courseId, std::stoi(usernames[counter].id), JWTToken);

			outputPosition(4, posy + 2);

			if (recordSet.empty())
			{
				std::cout << "Teacher added to course successfully! Press any key to continue...";
				(void)_getch();
				clearConsole();
				return;
			}

			std::cout << recordSet;
			(void)_getch();
			clearConsole();
		}
	}
}

void manageCourseOptions(std::string courseName, int courseId, std::string JWTToken, SceneContex* ctx)
{
	system("cls");
	char key; // Key to be entered
	int optionCounter = 0;
	std::vector<std::string> options = {
		"Update course info",
		"Update course teachers"
	};

	while (true)
	{
		int posy = 5;

		outputPosition(2, 3); setConsoleColorTo(6); std::cout << "O R G A N I S A T I O N   C O U R S E S"; setConsoleColorTo(7);

		for (int i = 0; i < options.size(); i++)
		{
			outputPosition(4, posy); std::cout << "-->";
			if (i == optionCounter)
			{
				setConsoleColorTo(6); outputPosition(9, posy); std::cout << options[i];
				outputPosition(60, posy);
			}
			else
			{
				outputPosition(9, posy); std::cout << options[i];
			}


			posy += 2;
			setConsoleColorTo(7);
		}

		key = _getch();
		
		if (key == '\r' && optionCounter == 0)
		{
			manageCourse(courseName, courseId, JWTToken, ctx);
			return;
		}

		if (key == '\r' && optionCounter == 1)
		{
			addTeacherToCourse(courseId, JWTToken, ctx);
			return;
		}

		if (key == 72 && (optionCounter >= 1 && optionCounter <= options.size())) // 72/75 is the ASCII code for the up arrow
		{
			optionCounter--;
		}

		if (key == 80 && (optionCounter >= 0 && optionCounter < options.size() - 1)) // 80/77 is the ASCII code for the up arrow
		{
			optionCounter++;
		}
	}
}

void addTopicsInCourse(int courseId, std::string JWTToken)
{
	clearConsole();

	char key; // Key to be entered
	int counter = 0, counter2 = 0;

	int posy = 5;
	std::string newTopic = "";

	while (true)
	{
		createInputField(posy + 3, "  TOPIC NAME", 40, 4, 7, newTopic);
		key = _getch();

		if (key == '\r')
		{
			break;
		}

		else
		{
			if (key == '\b')
			{
				newTopic = newTopic.substr(0, newTopic.size() - 1);
			}

			else
			{
				newTopic += key;
			}

		}
	}

	APIHandler apiHandler;
	std::string recordSet = apiHandler.createTopic(newTopic, courseId, JWTToken);
	
	if (recordSet.empty())
	{
		outputPosition(15, 31); std::cout << "The topic was created successfully! Press any key to continue!";

		(void)_getch();

		return;
	}

	outputPosition(15, 31); std::cout << recordSet;

	(void)_getch();

	return;
}

void updateTopicsInCourse(std::string topicName, int topicId, std::string JWTToken)
{
	clearConsole();

	char key; // Key to be entered
	int counter = 0, counter2 = 0;

	int posy = 5;

	while (true)
	{
		createInputField(posy + 3, "  TOPIC NAME", 40, 4, 7, topicName);
		key = _getch();

		if (key == '\r')
		{
			break;
		}

		else
		{
			if (key == '\b')
			{
				topicName = topicName.substr(0, topicName.size() - 1);
			}

			else
			{
				topicName += key;
			}

		}
	}

	APIHandler apiHandler;
	std::string recordSet = apiHandler.updateTopic(topicName, topicId, JWTToken);

	if (recordSet.empty())
	{
		outputPosition(15, 31); std::cout << "The topic was updated successfully! Press any key to continue!";

		(void)_getch();

		return;
	}

	outputPosition(15, 31); std::cout << recordSet;

	(void)_getch();

	return;
}

void createLesson(int topicId, std::string JWTToken)
{
	clearConsole();

	char key; // Key to be entered
	int counter = 0, counter2 = 0;
	std::string newLesson = "";
	std::string lessonData = "";
	int posy = 5;

	while (true)
	{
		createInputField(posy + 3, " LESSON NAME", 40, 4, 7, newLesson);
		key = _getch();

		if (key == '\r')
		{
			break;
		}
		else
		{
			if (key == '\b')
			{
				newLesson = newLesson.substr(0, newLesson.size() - 1);
			}

			else
			{
				newLesson += key;
			}

		}
	}

	outputPosition(4, posy + 7); std::cout << "LESSON DATA:" << std::endl;
	outputPosition(4, posy + 8); std::getline(std::cin, lessonData);


	APIHandler apiHandler;
	std::string recordSet = apiHandler.createLesson(newLesson, lessonData, topicId, JWTToken);

	if (recordSet.empty())
	{
		outputPosition(15, 31); std::cout << "The lesson was created successfully! Press any key to continue!";

		(void)_getch();

		return;
	}

	outputPosition(15, 31); std::cout << recordSet;

	(void)_getch();

	return;
}

void updateLesson(std::string lessonName, int lessonId, std::string JWTToken)
{
	clearConsole();

	char key; // Key to be entered
	int counter = 0, counter2 = 0;
	std::string lessonData = "";
	int posy = 5;

	while (true)
	{
		createInputField(posy + 3, " LESSON NAME", 40, 4, 7, lessonName);
		key = _getch();

		if (key == '\r')
		{
			break;
		}
		else
		{
			if (key == '\b')
			{
				lessonName = lessonName.substr(0, lessonName.size() - 1);
			}

			else
			{
				lessonName += key;
			}

		}
	}

	outputPosition(4, posy + 7); std::cout << "LESSON DATA:" << std::endl;
	outputPosition(4, posy + 8); std::getline(std::cin, lessonData);


	APIHandler apiHandler;
	std::string recordSet = apiHandler.updateLesson(lessonName, lessonData, lessonId, JWTToken);

	if (recordSet.empty())
	{
		outputPosition(15, 31); std::cout << "The lesson was created successfully! Press any key to continue!";

		(void)_getch();

		return;
	}

	outputPosition(15, 31); std::cout << recordSet;

	(void)_getch();

	return;
}

void createQuiz(int topicId, std::string JWTToken)
{

	clearConsole();

	char key; // Key to be entered
	int counter = 0, counter2 = 0;

	int posy = 5;
	std::string newQuizName = "";

	while (true)
	{
		createInputField(posy + 3, "  QUIZ NAME ", 40, 4, 7, newQuizName);
		key = _getch();

		if (key == '\r')
		{
			break;
		}

		else
		{
			if (key == '\b')
			{
				newQuizName = newQuizName.substr(0, newQuizName.size() - 1);
			}

			else
			{
				newQuizName += key;
			}

		}
	}

	APIHandler apiHandler;
	std::string recordSet = apiHandler.createQuiz(newQuizName, topicId, JWTToken);

	if (recordSet.empty())
	{
		outputPosition(15, 31); std::cout << "The quiz was created successfully! Press any key to continue!";

		(void)_getch();

		return;
	}

	outputPosition(15, 31); std::cout << recordSet;

	(void)_getch();

	return;
}

void displayLesson(int lessonId, std::string JWTToken)
{
	clearConsole();
	APIHandler apiHandler;
	LessonInfo lessonInfo = apiHandler.getLesson(lessonId, JWTToken);

	outputPosition(2, 6); setConsoleColorTo(6); std::cout << "L E S S O N: "; std::cout << lessonInfo.name; setConsoleColorTo(7);

	outputPosition(2, 9); std::cout << lessonInfo.data;

	(void)_getch();

	return;
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
				int setConsoleColorTos[5] = { 7,7,7,7,7 };

				setConsoleColorTo(6);
			
				do
				{
					setConsoleColorTos[iPut] = 6;

					if (firstShow)
					{
						printLogo(30);
						outputPosition(15, 7); std::cout << "MMMMMMMMMMMMMNX0kXXXXXXk0XNMMMMMMMMMMMMMMM" << std::endl;
						outputPosition(15, 8); std::cout << "MMMMMMMMMMMMMNX0kXXXXXXk0XNMMMMMMMMMMMMMMM" << std::endl;
					}

					createInputField(8, " First name ", 40, 65, setConsoleColorTos[0], info[0]);

					if (firstShow)
					{
						outputPosition(15, 9); std::cout << "MMMMMMMMMNOd:'.            .':dONMMMMMMMMM" << std::endl;
						outputPosition(15, 10); std::cout << "MMMMMMW0o'                      'o0WMMMMMM" << std::endl;
						outputPosition(15, 11); std::cout << "MMMMW0c.                          .c0WMMMM" << std::endl;
						outputPosition(15, 12); std::cout << "MMMNd.                              .dNMMM" << std::endl;
					}

					createInputField(12, " Last name  ", 40, 65, setConsoleColorTos[1], info[1]);

					if (firstShow)
					{
						outputPosition(15, 13); std::cout << "MMXl             ,oxkkxl'             lXMM" << std::endl;
						outputPosition(15, 14); std::cout << "MNo            .dNMMMMMMXc             oNM" << std::endl;
						outputPosition(15, 15); std::cout << "MO.            ;XMMMMMMMMO.            .OM" << std::endl;
						outputPosition(15, 16); std::cout << "Wl             .kWMMMMMMNo.             lW" << std::endl;
					}

					createInputField(16, " Username   ", 40, 65, setConsoleColorTos[2], info[2]);

					if (firstShow)
					{
						outputPosition(15, 17); std::cout << "N:              .cx000Od;.              :N" << std::endl;
						outputPosition(15, 18); std::cout << "Nc             .;:lodooc:,.             cN" << std::endl;
						outputPosition(15, 19); std::cout << "Wd.          'dKWMMMMMMMMN0l.          .dW" << std::endl;
						outputPosition(15, 20); std::cout << "MK,         '0MMMMMMMMMMMMMWx.         ,KM" << std::endl;
					}

					createInputField(20, " Email      ", 40, 65, setConsoleColorTos[3], info[3]);

					if (firstShow)
					{
						outputPosition(15, 21); std::cout << "MWk.        ;0WMMMMMMMMMMMMWk.        .kWM" << std::endl;
						outputPosition(15, 22); std::cout << "MMWk'       .,::::::::::::::,.       'kWMM" << std::endl;
						outputPosition(15, 23); std::cout << "MMMWKc.                            .cKMMMM" << std::endl;
						outputPosition(15, 24); std::cout << "MMMMMNO:.                        .:ONMMMMM" << std::endl;
					}

					createInputField(24, " Password   ", 40, 65, setConsoleColorTos[4], info[4]);

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
				int setConsoleColorTos[2] = { 7,7 };

				setConsoleColorTo(6);
				do {
					printLogo(30);

					setConsoleColorTos[iPut] = 6;

					outputPosition(15, 7); std::cout << "MMMMMMMMMMMMMNX0kXXXXXXk0XNMMMMMMMMMMMMMMM" << std::endl;
					outputPosition(15, 8); std::cout << "MMMMMMMMMMMMMNX0kXXXXXXk0XNMMMMMMMMMMMMMMM" << std::endl;

					createInputField(8, " Email/Username ", 44, 65, setConsoleColorTos[0], info[0], 7, 18);

					outputPosition(15, 9); std::cout << "MMMMMMMMMNOd:'.            .':dONMMMMMMMMM" << std::endl;
					outputPosition(15, 10); std::cout << "MMMMMMW0o'                      'o0WMMMMMM" << std::endl;
					outputPosition(15, 11); std::cout << "MMMMW0c.                          .c0WMMMM" << std::endl;
					outputPosition(15, 12); std::cout << "MMMNd.                              .dNMMM" << std::endl;

					createInputField(12, " Password       ", 44, 65, setConsoleColorTos[1], info[1], 7, 18);

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
					outputPosition(15, 31); std::cout << "You are now logged in! Press any key to continue!";

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
				sceneContext->JWTToken = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXUyJ9.eyJleHAiOjE2NTIxNDMzNTksImlhdCI6MTY1MjA1Njk1OSwiaXNBZG1pbiI6MCwic3ViIjoiMiJ9.FZuDtASJb79hfGlMOy2rgW95AHmEQ9FsDWCbXVaHno8";

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
							recordSet = accountPage(userData, sceneContext->JWTToken, sceneContext);

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
								recordSet = viewOrganisation(sceneContext->JWTToken, sceneContext);

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

	scenes.push_back(
		{
			4,
			"ViewOrgAsUser",
			[&]()
			{
				clearConsole();
				char key; // Key to be entered
				int orgsCounter = 0;
				int optionCounter = 0;

				APIHandler apiHandler;
				
				OrgInfo orgInfo = apiHandler.getOrg(sceneContext->orgInfo.name, sceneContext->JWTToken);

				if (!orgInfo.errors.empty())
				{
					std::cout << "There was problem with the server. Please try again latter!";

					sceneContext->orgInfo = {};

					(void)_getch();
					return "NavigationBar";
				}

				sceneContext->orgInfo = orgInfo;

				while (true)
				{
					int posy = 5;

					outputPosition(2, 3); setConsoleColorTo(6); std::cout << "O R G A N I S A T I O N   C O U R S E S"; setConsoleColorTo(7);

					for (int i = 0; i < orgInfo.courses.size(); i++)
					{
						outputPosition(4, posy); std::cout << "-->";

						if (i == orgsCounter)
						{
							setConsoleColorTo(6); outputPosition(9, posy); std::cout << orgInfo.courses[i].name;
							outputPosition(60, posy);
						}
						else
						{
							outputPosition(9, posy); std::cout << orgInfo.courses[i].name;
						}


						posy += 2;
						setConsoleColorTo(7);
					}

					key = _getch();

					if (key == 27)
					{
						clearConsole();
						return "NavigationBar";
					}

					if (key == '\r')
					{
						auto courseInfo = apiHandler.getCourse(orgInfo.courses[orgsCounter].id, sceneContext->JWTToken);
						// try to get course info
						if (courseInfo.errors.empty())
						{
							sceneContext->courseInfo = courseInfo;

							int userRole = 0;

							apiHandler.getUserInfo("@me", sceneContext, sceneContext->user);

							for (auto& user : courseInfo.users)
							{
								if (user.id == std::stoi(sceneContext->user.id))
								{
									userRole = user.role;
								}
							}

							switch (userRole)
							{
							case 1:
							case 2:
								return "ViewCourseAsAuth";
								break;
							case 0:
							default:
								return "ViewCourseAsUser";
								break;
							}
						}
						else
						{
							joinCourse(orgInfo.courses[orgsCounter].id, sceneContext->JWTToken);
							clearConsole();
							return "ViewOrgAsUser";
						}
					}

					if (key == 72 && (orgsCounter >= 1 && orgsCounter <= orgInfo.courses.size())) // 72/75 is the ASCII code for the up arrow
					{
						orgsCounter--;
					}

					if (key == 80 && (orgsCounter >= 0 && orgsCounter < orgInfo.courses.size() - 1)) // 80/77 is the ASCII code for the up arrow
					{
						orgsCounter++;
					}
				}

				return "ViewOrgAsUser";
			}
		}
	);

	scenes.push_back(
		{
			5,
			"ViewOrgAsAdmin",
			[&]()
			{
				clearConsole();
				char key; // Key to be entered
				int counter = 0, counter2 = 0;

				APIHandler apiHandler;

				OrgInfo orgInfo = apiHandler.getOrg(sceneContext->orgInfo.name, sceneContext->JWTToken);

				if (!orgInfo.errors.empty())
				{
					std::cout << "There was problem with the server. Please try again latter!";

					sceneContext->orgInfo = {};

					(void)_getch();
					return "NavigationBar";
				}

				sceneContext->orgInfo = orgInfo;

				while (true)
				{
					int posy = 5;
					outputPosition(2, 3); setConsoleColorTo(6); std::cout << "O R G A N I S A T I O N   C O U R S E S"; setConsoleColorTo(7);
					for (int i = 0; i < orgInfo.courses.size(); i++)
					{
						outputPosition(4, posy); std::cout << "-->";
						if (i == counter)
						{
							setConsoleColorTo(6); outputPosition(9, posy); std::cout << orgInfo.courses[i].name;
							outputPosition(60, posy);
							if (counter2 == 0)
							{
								setConsoleColorTo(2); std::cout << "Enter"; setConsoleColorTo(7); std::cout << " | Settings | Delete";
							}
							else if (counter2 == 1)
							{
								setConsoleColorTo(7); std::cout << "Enter | "; setConsoleColorTo(9); std::cout << "Settings"; setConsoleColorTo(7); std::cout << " | Delete";
							}
							else
							{
								setConsoleColorTo(7); std::cout << "Enter | Settings | "; setConsoleColorTo(4); std::cout << "Delete";
							}
						}
						else
						{
							outputPosition(9, posy); std::cout << orgInfo.courses[i].name;
							outputPosition(60, posy); setConsoleColorTo(8); std::cout << "Enter | Settings | Delete";
						}


						posy += 2;
						setConsoleColorTo(7);
					}

					outputPosition(4, posy); std::cout << "-->";
					orgInfo.courses.size() == counter ? setConsoleColorTo(6) : setConsoleColorTo(7);
					outputPosition(9, posy); std::cout << "Create Course";
					setConsoleColorTo(7);

					key = _getch();

					if (key == 27)
					{
						clearConsole();
						return "NavigationBar";
					}

					if (key == '\t')
					{
						clearConsole();
						return "updateOrganisation";
					}

					if (key == '\r' && counter == orgInfo.courses.size())
					{
						createCourse(sceneContext->JWTToken, orgInfo.id);
						return "ViewOrgAsAdmin";
					}

					if (key == '\r' && counter2 == 0)
					{
						auto courseInfo = apiHandler.getCourse(orgInfo.courses[counter].id, sceneContext->JWTToken);
						// try to get course info
						if (courseInfo.errors.empty())
						{
							sceneContext->courseInfo = courseInfo;
							return "ViewCourseAsAuth";
						}
						else
						{
							joinCourse(orgInfo.courses[counter].id, sceneContext->JWTToken);
							clearConsole();
							return "ViewOrgAsAdmin";
						}
					}

					if (key == '\r' && counter2 == 1)
					{
						auto courseInfo = apiHandler.getCourse(orgInfo.courses[counter].id, sceneContext->JWTToken);
						
						manageCourseOptions(courseInfo.name, orgInfo.courses[counter].id, sceneContext->JWTToken, sceneContext);
						clearConsole();
						return "ViewOrgAsAdmin";
					}

					if (key == '\r' && counter2 == 2)
					{
						auto recordSet = apiHandler.deleteCourse(orgInfo.courses[counter].id, sceneContext->JWTToken);

						outputPosition(4, posy + 2);

						if (recordSet.empty())
						{
							std::cout << "Course deleted successfully! Press any key to continue...";
							(void)_getch();
							clearConsole();
							return "ViewOrgAsAdmin";
						}

						std::cout << recordSet;
						(void)_getch();
					}

					if (key == 72 && (counter >= 1 && counter <= orgInfo.courses.size()))
					{
						counter--;
					}

					if (key == 80 && (counter >= 0 && counter < orgInfo.courses.size()))
					{
						counter++;
					}

					if (key == 75 && (counter2 == 1) || key == 75 && (counter2 == 2)) // 72/75 is the ASCII code for the up arrow
					{
						counter2--;
					}

					if (key == 77 && (counter2 == 0) || key == 77 && (counter2 == 1)) // 80/77 is the ASCII code for the up arrow
					{
						counter2++;
					}
				}

				return "ViewOrgAsAdmin";
			}
		}
	);

	scenes.push_back(
		{
			7,
			"ViewOrgAsTeacher",
			[&]()
			{
				system("cls");
				char key; // Key to be entered
				int counter = 0, counter2 = 0;

				APIHandler apiHandler;

				OrgInfo orgInfo = apiHandler.getOrg(sceneContext->orgInfo.name, sceneContext->JWTToken);

				if (!orgInfo.errors.empty())
				{
					std::cout << "There was problem with the server. Please try again latter!";

					sceneContext->orgInfo = {};

					(void)_getch();
					return "NavigationBar";
				}

				sceneContext->orgInfo = orgInfo;

				OrgUser userRole;

				for (auto& user: orgInfo.users)
				{
					if (std::to_string(user.id) == sceneContext->user.id)
					{
						userRole = user;
						break;
					}
				}

				while (true)
				{
					int posy = 5;
					outputPosition(2, 3); setConsoleColorTo(6); std::cout << "O R G A N I S A T I O N   C O U R S E S"; setConsoleColorTo(7);
					for (int i = 0; i < orgInfo.courses.size(); i++)
					{
						outputPosition(4, posy); std::cout << "-->";
						if (i == counter)
						{
							sceneContext->user.id;
							setConsoleColorTo(6); outputPosition(9, posy); std::cout << orgInfo.courses[i].name;
							outputPosition(60, posy);
							
							if (std::find(userRole.userCoursesId.begin(), userRole.userCoursesId.end(), orgInfo.courses[i].id) != userRole.userCoursesId.end())
							{
								if (counter2 == 0)
								{
									setConsoleColorTo(2); std::cout << "Enter"; setConsoleColorTo(7); std::cout << " | Settings";
								}
								else
								{
									setConsoleColorTo(7); std::cout << "Enter | "; setConsoleColorTo(4); std::cout << "Settings";
								}
							}
							else
							{
								setConsoleColorTo(6); outputPosition(9, posy); std::cout << orgInfo.courses[i].name;
								outputPosition(60, posy);
							}
						}
						else
						{
							if (std::find(userRole.userCoursesId.begin(), userRole.userCoursesId.end(), orgInfo.courses[i].id) != userRole.userCoursesId.end())
							{
								outputPosition(9, posy); std::cout << orgInfo.courses[i].name;
								outputPosition(60, posy); setConsoleColorTo(8); std::cout << "Enter | Settings";
							}
							else
							{
								outputPosition(9, posy); std::cout << orgInfo.courses[i].name;
							}
						}


						posy += 2;
						setConsoleColorTo(7);
					}

					key = _getch();

					if (key == 27)
					{
						clearConsole();
						return "NavigationBar";
					}
					
					if (key == '\r' && counter2 == 0)
					{
						auto courseInfo = apiHandler.getCourse(orgInfo.courses[counter].id, sceneContext->JWTToken);
						// try to get course info
						if (courseInfo.errors.empty())
						{
							sceneContext->courseInfo = courseInfo;

							int userRole = 0;

							apiHandler.getUserInfo("@me", sceneContext, sceneContext->user);

							for (auto& user : courseInfo.users)
							{
								if (user.id == std::stoi(sceneContext->user.id))
								{
									userRole = user.role;
								}
							}

							switch (userRole)
							{
							case 1:
							case 2:
								return "ViewCourseAsAuth";
								break;
							case 0:
							default:
								return "ViewCourseAsUser";
								break;
							}
						}
						else
						{
							joinCourse(orgInfo.courses[counter].id, sceneContext->JWTToken);
							clearConsole();
							return "ViewCourseAsAuth";
						}
					}

					if (key == '\r' && counter2 == 1)
					{
						manageCourse(orgInfo.courses[counter].name, orgInfo.courses[counter].id, sceneContext->JWTToken, sceneContext);
						return "ViewOrgAsTeacher";
					}

					if (key == 72 && (counter >= 1 && counter <= orgInfo.courses.size())) // 72 is the ASCII code for the up arrow
					{
						counter--;
					}

					if (key == 80 && (counter >= 0 && counter < orgInfo.courses.size())) // 80 is the ASCII code for the down arrow
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

				return "ViewOrgAsTeacher";
			}
		}
	);

	scenes.push_back(
		{
			8,
			"updateOrganisation",
			[&]() -> std::string
			{
				system("cls");
				char key = ' ';
				int iPut = 0;

				APIHandler apiHandler;

				OrgInfo orgInfo = apiHandler.getOrg(sceneContext->orgInfo.name, sceneContext->JWTToken);

				if (!orgInfo.errors.empty())
				{
					std::cout << "There was problem with the server. Please try again latter!";

					sceneContext->orgInfo = {};

					(void)_getch();
					return "NavigationBar";
				}

				sceneContext->orgInfo = orgInfo;

				std::string info[5] = { orgInfo.name, ""};
				
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
						createButton(16, "    Update    ", 14, 75, 2);
					}
					else {
						createButton(16, "    Update    ", 14, 75, 7);
					}

					key = _getch();

					if (key == 27)
					{
						return "NavigationBar";
					}

					if (key == '\t')
					{
						return "ManageRolesInOrg";
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


				std::string recordSet = apiHandler.updateOrg({ info[0], info[1] }, orgInfo.name, sceneContext->JWTToken);

				if (recordSet.empty())
				{
					outputPosition(15, 31); std::cout << "The organisation info was updated successfully! Press any key to continue!";

					sceneContext->orgInfo.name = info[0];

					(void)_getch();

					return "updateOrganisation";
				}

				outputPosition(15, 31); std::cout << recordSet;

				(void)_getch();

				return "updateOrganisation";
			}
		}
	);

	scenes.push_back(
		{
			9,
			"ManageRolesInOrg",
			[&]() -> std::string
			{
				clearConsole();
				char key; // Key to be entered
				int counter = 0, counter2 = 0;

				APIHandler apiHandler;

				OrgInfo orgInfo = apiHandler.getOrg(sceneContext->orgInfo.name, sceneContext->JWTToken);

				if (!orgInfo.errors.empty())
				{
					std::cout << "There was problem with the server. Please try again latter!";

					sceneContext->orgInfo = {};

					(void)_getch();
					return "NavigationBar";
				}

				sceneContext->orgInfo = orgInfo;

				std::vector<std::string> usernames;

				for (int i = 0; i < orgInfo.users.size(); i++)
				{
					User user;
					apiHandler.getUserInfoById(std::to_string(orgInfo.users[i].id), sceneContext, user);

					usernames.push_back(user.username);
				}

				while (true)
				{
					// Button for history notebook section
					int posy = 5;
					outputPosition(2, 3); setConsoleColorTo(6); std::cout << "O R G A N I Z A T I O N   U S E R S"; setConsoleColorTo(7);
					for (int i = 0; i < usernames.size(); i++)
					{
						outputPosition(4, posy); std::cout << "-->";

						if (i == counter)
						{
							setConsoleColorTo(6); outputPosition(9, posy); std::cout << usernames[i];
							outputPosition(60, posy);
							if (counter2 == 0)
							{
								setConsoleColorTo(2); std::cout << "Student"; setConsoleColorTo(7); std::cout << " | Teacher | Admin";
							}
							else if (counter2 == 1)
							{
								setConsoleColorTo(7); std::cout << "Student | "; setConsoleColorTo(9); std::cout << "Teacher"; setConsoleColorTo(7); std::cout << " | Admin";
							}
							else
							{
								setConsoleColorTo(7); std::cout << "Student | Teacher | "; setConsoleColorTo(4); std::cout << "Admin";
							}
						}
						else
						{
							outputPosition(9, posy); std::cout << usernames[i];
							outputPosition(60, posy); setConsoleColorTo(8); std::cout << "Student | Teacher | Admin";
						}

						posy += 2;
						setConsoleColorTo(7);
					}

					key = _getch();

					if (key == 27)
					{
						return "NavigationBar";
					}

					if (key == '\t')
					{
						return "ViewOrgAsAdmin";
					}

					if (key == '\r' && counter2 == 0)
					{
						auto recordSet = apiHandler.updateRoleInOrg(orgInfo.id, orgInfo.users[counter].id, 0,sceneContext->JWTToken);

						outputPosition(4, posy + 2);

						if (recordSet.empty())
						{
							std::cout << "User role updated successfully! Press any key to continue...";
							(void)_getch();
							clearConsole();
							return "NavigationBar";
						}

						std::cout << recordSet;
					}

					if (key == '\r' && counter2 == 1)
					{
						auto recordSet = apiHandler.updateRoleInOrg(orgInfo.id, orgInfo.users[counter].id, 1, sceneContext->JWTToken);

						outputPosition(4, posy + 2);

						if (recordSet.empty())
						{
							std::cout << "User role updated successfully! Press any key to continue...";
							(void)_getch();
							clearConsole();
							return "NavigationBar";
						}

						std::cout << recordSet;
					}

					if (key == '\r' && counter2 == 2)
					{
						auto recordSet = apiHandler.updateRoleInOrg(orgInfo.id, orgInfo.users[counter].id, 2, sceneContext->JWTToken);

						outputPosition(4, posy + 2);

						if (recordSet.empty())
						{
							std::cout << "User role updated successfully! Press any key to continue...";
							(void)_getch();
							clearConsole();
							return "NavigationBar";
						}

						std::cout << recordSet;
					}

					if (key == 72 && (counter >= 1 && counter <= usernames.size())) // 72/75 is the ASCII code for the up arrow
					{
						counter--;
					}

					if (key == 80 && (counter >= 0 && counter < usernames.size() - 1)) // 80/77 is the ASCII code for the up arrow
					{
						counter++;
					}

					if (key == 75 && (counter2 == 1) || key == 75 && (counter2 == 2)) // 72/75 is the ASCII code for the up arrow
					{
						counter2--;
					}

					if (key == 77 && (counter2 == 0) || key == 77 && (counter2 == 1)) // 80/77 is the ASCII code for the up arrow
					{
						counter2++;
					}

				}
			}
		}
	);

	scenes.push_back(
		{
			10,
			"ViewCourseAsUser",
			[&]()
			{
				clearConsole();
				char key; // Key to be entered
				int coursesCounter = 0;
				int optionCounter = 0;

				APIHandler apiHandler;

				CourseInfo courseInfo = apiHandler.getCourse(sceneContext->courseInfo.id, sceneContext->JWTToken);

				if (!courseInfo.errors.empty())
				{
					std::cout << "There was problem with the server. Please try again latter!";

					sceneContext->courseInfo = {};

					(void)_getch();
					return "NavigationBar";
				}

				sceneContext->courseInfo = courseInfo;

				while (true)
				{
					int posy = 5;

					outputPosition(2, 3); setConsoleColorTo(6); std::cout << "C O U R S E   T O P I C S"; setConsoleColorTo(7);

					for (int i = 0; i < courseInfo.topics.size(); i++)
					{
						outputPosition(4, posy); std::cout << "-->";

						if (i == coursesCounter)
						{
							setConsoleColorTo(6); outputPosition(9, posy); std::cout << courseInfo.topics[i].name;
							outputPosition(60, posy);
						}
						else
						{
							outputPosition(9, posy); std::cout << courseInfo.topics[i].name;
						}


						posy += 2;
						setConsoleColorTo(7);
					}

					key = _getch();

					if (key == 27)
					{
						clearConsole();
						return "ViewOrgAsUser";
					}

					if (key == '\r')
					{
						sceneContext->topicInfo.id = courseInfo.topics[coursesCounter].id;
						return "ViewTopicAsUser";
					}

					if (key == 72 && (coursesCounter >= 1 && coursesCounter <= courseInfo.topics.size())) // 72/75 is the ASCII code for the up arrow
					{
						coursesCounter--;
					}

					if (key == 80 && (coursesCounter >= 0 && coursesCounter < courseInfo.topics.size() - 1)) // 80/77 is the ASCII code for the up arrow
					{
						coursesCounter++;
					}
				}

				return "ViewOrgAsUser";
			}
		}
	);

	scenes.push_back(
		{
			11,
			"ViewCourseAsAuth",
			[&]()
			{
				clearConsole();
				char key; // Key to be entered
				int counter = 0, counter2 = 0;

				APIHandler apiHandler;

				CourseInfo courseInfo = apiHandler.getCourse(sceneContext->courseInfo.id, sceneContext->JWTToken);

				if (!courseInfo.errors.empty())
				{
					std::cout << "There was problem with the server. Please try again latter!";

					sceneContext->orgInfo = {};

					(void)_getch();
					return "NavigationBar";
				}

				sceneContext->courseInfo = courseInfo;

				while (true)
				{
					int posy = 5;

					outputPosition(2, 3); setConsoleColorTo(6); std::cout << "C O U R S E   T O P I C S"; setConsoleColorTo(7);

					for (int i = 0; i < courseInfo.topics.size(); i++)
					{
						outputPosition(4, posy); std::cout << "-->";
						if (i == counter)
						{
							setConsoleColorTo(6); outputPosition(9, posy); std::cout << courseInfo.topics[i].name;
							outputPosition(60, posy);
							if (counter2 == 0)
							{
								setConsoleColorTo(2); std::cout << "Enter"; setConsoleColorTo(7); std::cout << " | Settings | Delete";
							}
							else if (counter2 == 1)
							{
								setConsoleColorTo(7); std::cout << "Enter | "; setConsoleColorTo(9); std::cout << "Settings"; setConsoleColorTo(7); std::cout << " | Delete";
							}
							else
							{
								setConsoleColorTo(7); std::cout << "Enter | Settings | "; setConsoleColorTo(4); std::cout << "Delete";
							}
						}
						else
						{
							outputPosition(9, posy); std::cout << courseInfo.topics[i].name;
							outputPosition(60, posy); setConsoleColorTo(8); std::cout << "Enter | Settings | Delete";
						}


						posy += 2;
						setConsoleColorTo(7);
					}

					outputPosition(4, posy); std::cout << "-->";
					courseInfo.topics.size() == counter ? setConsoleColorTo(6) : setConsoleColorTo(7);
					outputPosition(9, posy); std::cout << "Create Topic";
					setConsoleColorTo(7);

					key = _getch();

					if (key == 27)
					{
						clearConsole();
						return "NavigationBar";
					}

					if (key == '\r' && counter == courseInfo.topics.size())
					{
						addTopicsInCourse(courseInfo.id, sceneContext->JWTToken);
						return "NavigationBar";
					}

					if (key == '\r' && counter2 == 0)
					{
						sceneContext->topicInfo.id = courseInfo.topics[counter].id;
						return "ViewTopicAsAuth";
					}
					
					if (key == '\r' && counter2 == 1)
					{
						updateTopicsInCourse(courseInfo.topics[counter].name, courseInfo.topics[counter].id, sceneContext->JWTToken);
						return "NavigationBar";
					}

					if (key == '\r' && counter2 == 2)
					{
						std::string recordSet = apiHandler.deleteTopic(courseInfo.topics[counter].id, sceneContext->JWTToken);

						outputPosition(4, posy + 2);

						if (recordSet.empty())
						{
							std::cout << "Topic deleted successfully! Press any key to continue...";
							(void)_getch();
							clearConsole();
							return "NavigationBar";
						}

						std::cout << recordSet;
						(void)_getch();

						return "NavigationBar";
					}
					
					if (key == 72 && (counter >= 1 && counter <= courseInfo.topics.size()))
					{
						counter--;
					}

					if (key == 80 && (counter >= 0 && counter < courseInfo.topics.size()))
					{
						counter++;
					}

					if (key == 75 && (counter2 == 1) || key == 75 && (counter2 == 2)) // 72/75 is the ASCII code for the up arrow
					{
						counter2--;
					}

					if (key == 77 && (counter2 == 0) || key == 77 && (counter2 == 1)) // 80/77 is the ASCII code for the up arrow
					{
						counter2++;
					}
				}

				return "ViewCourseAsAuth";
			}
		}
	);

	scenes.push_back(
		{
			12,
			"ViewTopicAsUser",
			[&]()
			{
				clearConsole();
				char key; // Key to be entered
				int orgsCounter = 0;
				int optionCounter = 0;

				APIHandler apiHandler;

				TopicInfo topicsInfo = apiHandler.getTopic(sceneContext->topicInfo.id, sceneContext->JWTToken);

				if (!topicsInfo.errors.empty())
				{
					std::cout << "There was problem with the server. Please try again latter!";

					sceneContext->courseInfo = {};

					(void)_getch();
					return "NavigationBar";
				}

				sceneContext->topicInfo = topicsInfo;

				while (true)
				{
					int posy = 5;

					outputPosition(2, 3); setConsoleColorTo(6); std::cout << "C O U R S E   T O P I C S"; setConsoleColorTo(7);

					for (int i = 0; i < topicsInfo.lessons.size(); i++)
					{
						outputPosition(4, posy); std::cout << "-->";

						if (i == orgsCounter)
						{
							setConsoleColorTo(6); outputPosition(9, posy); std::cout << topicsInfo.lessons[i].name;
							outputPosition(60, posy);
						}
						else
						{
							outputPosition(9, posy); std::cout << topicsInfo.lessons[i].name;
						}


						posy += 2;
						setConsoleColorTo(7);
					}

					outputPosition(2, topicsInfo.lessons.size() + 7); setConsoleColorTo(6); std::cout << "C O U R S E   Q U I Z Z E S"; setConsoleColorTo(7);
					posy += 2;

					for (int i = 0; i < topicsInfo.quizzes.size(); i++)
					{
						outputPosition(4, posy); std::cout << "-->";

						if (i + topicsInfo.lessons.size() == orgsCounter)
						{
							setConsoleColorTo(6); outputPosition(9, posy); std::cout << topicsInfo.quizzes[i].name;
							outputPosition(60, posy);
						}
						else
						{
							outputPosition(9, posy); std::cout << topicsInfo.quizzes[i].name;
						}


						posy += 2;
						setConsoleColorTo(7);
					}

					key = _getch();

					if (key == 27)
					{
						clearConsole();
						return "ViewCourseAsUser";
					}

					if (key == '\r')
					{
						/*
						auto courseInfo = apiHandler.getCourse(courseInfo.courses[orgsCounter].id, sceneContext->JWTToken);
						// try to get course info
						if (courseInfo.errors.empty())
						{
							sceneContext->courseInfo = courseInfo;

							int userRole = 0;

							apiHandler.getUserInfo("@me", sceneContext, sceneContext->user);

							for (auto& user : courseInfo.users)
							{
								if (user.id == std::stoi(sceneContext->user.id))
								{
									userRole = user.role;
								}
							}

							switch (userRole)
							{
							case 1:
								return "ViewOrgAsTeacher";
								break;
							case 2:
								return "ViewOrgAsAdmin";
								break;
							case 0:
							default:
								return "ViewOrgAsUser";
								break;
							}
						}
						else
						{
							joinCourse(courseInfo.courses[orgsCounter].id, sceneContext->JWTToken);
							clearConsole();
							return "ViewOrgAsUser";
						}
						*/
					}

					if (key == 72 && (orgsCounter >= 1 && orgsCounter <= topicsInfo.lessons.size() + topicsInfo.quizzes.size())) // 72/75 is the ASCII code for the up arrow
					{
						orgsCounter--;
					}

					if (key == 80 && (orgsCounter >= 0 && orgsCounter < topicsInfo.lessons.size() + topicsInfo.quizzes.size() - 1)) // 80/77 is the ASCII code for the up arrow
					{
						orgsCounter++;
					}
				}

				return "ViewOrgAsUser";
			}
		}
	);

	scenes.push_back(
		{
			13,
			"ViewTopicAsAuth",
			[&]()
			{
				clearConsole();
				char key; // Key to be entered
				int counter = 0, counter2 = 0;

				APIHandler apiHandler;

				TopicInfo topicInfo = apiHandler.getTopic(sceneContext->topicInfo.id, sceneContext->JWTToken);

				if (!topicInfo.errors.empty())
				{
					std::cout << "There was problem with the server. Please try again latter!";

					sceneContext->orgInfo = {};

					(void)_getch();
					return "NavigationBar";
				}

				sceneContext->topicInfo = topicInfo;

				while (true)
				{
					int posy = 5;

					outputPosition(2, 3); setConsoleColorTo(6); std::cout << "T O P I C  T O P I C S"; setConsoleColorTo(7);

					for (int i = 0; i < topicInfo.lessons.size(); i++)
					{
						outputPosition(4, posy); std::cout << "-->";
						if (i == counter)
						{
							setConsoleColorTo(6); outputPosition(9, posy); std::cout << topicInfo.lessons[i].name;
							outputPosition(60, posy);
							if (counter2 == 0)
							{
								setConsoleColorTo(2); std::cout << "Enter"; setConsoleColorTo(7); std::cout << " | Settings | Delete";
							}
							else if (counter2 == 1)
							{
								setConsoleColorTo(7); std::cout << "Enter | "; setConsoleColorTo(9); std::cout << "Settings"; setConsoleColorTo(7); std::cout << " | Delete";
							}
							else
							{
								setConsoleColorTo(7); std::cout << "Enter | Settings | "; setConsoleColorTo(4); std::cout << "Delete";
							}
						}
						else
						{
							outputPosition(9, posy); std::cout << topicInfo.lessons[i].name;
							outputPosition(60, posy); setConsoleColorTo(8); std::cout << "Enter | Settings | Delete";
						}


						posy += 2;
						setConsoleColorTo(7);
					}

					outputPosition(4, posy); std::cout << "-->";
					topicInfo.lessons.size() == counter ? setConsoleColorTo(6) : setConsoleColorTo(7);
					outputPosition(9, posy); std::cout << "Create Lesson";
					setConsoleColorTo(7);

					outputPosition(2, topicInfo.lessons.size() + 9); setConsoleColorTo(6); std::cout << "T O P I C   Q U I Z Z E S"; setConsoleColorTo(7);
					posy += 4;

					for (int i = 0; i < topicInfo.quizzes.size(); i++)
					{
						outputPosition(4, posy); std::cout << "-->";
						if (i + topicInfo.lessons.size() + 1 == counter)
						{
							setConsoleColorTo(6); outputPosition(9, posy); std::cout << topicInfo.quizzes[i].name;
							outputPosition(60, posy);
							if (counter2 == 0)
							{
								setConsoleColorTo(2); std::cout << "Enter"; setConsoleColorTo(7); std::cout << " | Settings | Delete";
							}
							else if (counter2 == 1)
							{
								setConsoleColorTo(7); std::cout << "Enter | "; setConsoleColorTo(9); std::cout << "Settings"; setConsoleColorTo(7); std::cout << " | Delete";
							}
							else
							{
								setConsoleColorTo(7); std::cout << "Enter | Settings | "; setConsoleColorTo(4); std::cout << "Delete";
							}
						}
						else
						{
							outputPosition(9, posy); std::cout << topicInfo.quizzes[i].name;
							outputPosition(60, posy); setConsoleColorTo(8); std::cout << "Enter | Settings | Delete";
						}

						posy += 2;
						setConsoleColorTo(7);
					}

					outputPosition(4, posy); std::cout << "-->";
					topicInfo.lessons.size() + topicInfo.quizzes.size() + 1 == counter ? setConsoleColorTo(6) : setConsoleColorTo(7);
					outputPosition(9, posy); std::cout << "Create Quiz";
					setConsoleColorTo(7);

					key = _getch();

					if (key == 27)
					{
						clearConsole();
						return "ViewCourseAsAuth";
					}

					if (key == '\r' && counter == topicInfo.lessons.size())
					{
						createLesson(topicInfo.id, sceneContext->JWTToken);
						return "ViewTopicAsAuth";
					}

					if (key == '\r' && counter == topicInfo.lessons.size() + topicInfo.quizzes.size() + 1)
					{
						createQuiz(topicInfo.id, sceneContext->JWTToken);
						return "ViewTopicAsAuth";
					}

					if (key == '\r' && counter2 == 0 && counter < topicInfo.lessons.size())
					{
						displayLesson(topicInfo.lessons[counter].id, sceneContext->JWTToken);
						return "ViewTopicAsAuth";
					}
					if (key == '\r' && counter2 == 1 && counter < topicInfo.lessons.size())
					{
						updateLesson(topicInfo.lessons[counter].name, topicInfo.lessons[counter].id, sceneContext->JWTToken);
						return "ViewTopicAsAuth";
					}

					if (key == '\r' && counter2 == 2 && counter < topicInfo.lessons.size())
					{
						std::string recordSet = apiHandler.deleteLesson(topicInfo.lessons[counter].id, sceneContext->JWTToken);

						outputPosition(4, posy + 2);

						if (recordSet.empty())
						{
							std::cout << "Lesson deleted successfully! Press any key to continue...";
							(void)_getch();
							clearConsole();
							return "ViewTopicAsAuth";
						}

						std::cout << recordSet;
						(void)_getch();

						return "ViewCourseAsAuth";
					}

					if (key == 72 && (counter >= 1 && counter <= topicInfo.lessons.size() + topicInfo.quizzes.size() + 1))
					{
						counter--;
					}

					if (key == 80 && (counter >= 0 && counter < topicInfo.lessons.size() + topicInfo.quizzes.size() + 1))
					{
						counter++;
					}

					if (key == 75 && (counter2 == 1) || key == 75 && (counter2 == 2)) // 72/75 is the ASCII code for the up arrow
					{
						counter2--;
					}

					if (key == 77 && (counter2 == 0) || key == 77 && (counter2 == 1)) // 80/77 is the ASCII code for the up arrow
					{
						counter2++;
					}
				}

				return "ViewCourseAsAuth";
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

