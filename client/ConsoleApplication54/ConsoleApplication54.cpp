#include <iostream>
#include <APIHandler.h>
#include <conio.h>
#include <windows.h>
using namespace std;

void color(int color) //Sets color of the text
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void gotoxy(int x, int y)  //Get the coordinates inside the console
{
	COORD coord;
	coord.X = x; // x-axis
	coord.Y = y; // y-axis
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void createInputField(int posy, string text, int size, int posx, int col, string input = "", int col2 = 7)
{
	color(col);
	gotoxy(posx, posy); std::cout << char(201);

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

	gotoxy(posx, posy + 1); std::cout << char(186); 
	color(col2);

	std::cout << text << "  "; color(col); std::cout << char(186) << " ";
	color(col2);

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

	color(col); std::cout << char(186) << std::endl;
	gotoxy(posx, posy + 2); std::cout << char(200);

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
	color(6);
}
void createButton(int posy, string text, int size, int posx, int col, int col2 = 7)
{
	color(col); gotoxy(posx, posy); std::cout << char(201);

	for (int i = 0; i < size; i++)
	{
		std::cout << char(205);
	}
	std::cout << char(187);

	gotoxy(posx, posy + 1); std::cout << char(186); color(col2); std::cout << text; color(col); std::cout << char(186) << std::endl;
	gotoxy(posx, posy + 2); std::cout << char(200);

	for (int i = 0; i < size; i++)
	{
		std::cout << char(205);
	}

	std::cout << char(188); color(7);
}
void printLogo(int posx) {
	gotoxy(posx, 0); std::cout << "     _             _  __         _     _ _                          _     _ _      " << std::endl;
	gotoxy(posx, 1); std::cout << "  __| | __ _ ___  | |/ /   _  __| | __| | | ___ _ __ ___  _   _  __| | __| | | ___ " << std::endl;
	gotoxy(posx, 2); std::cout << " / _` |/ _` / __| | ' / | | |/ _` |/ _` | |/ _ \\ '_ ` _ \\| | | |/ _` |/ _` | |/ _ \\" << std::endl;
	gotoxy(posx, 3); std::cout << "| (_| | (_| \\__ \\ | . \\ |_| | (_| | (_| | |  __/ | | | | | |_| | (_| | (_| | |  __/" << std::endl;
	gotoxy(posx, 4); std::cout << " \\__,_|\\__,_|___/ |_|\\_\\__,_|\\__,_|\\__,_|_|\\___|_| |_| |_|\\__,_|\\__,_|\\__,_|_|\\___|" << std::endl;
}

void loginPage()
{
	system("cls");
	char key = ' ';
	int iPut = 0;
	string info[2] = { "", "" };

	color(6);
	do {
		printLogo(30);
		int colors[2] = { 7,7 };
		colors[iPut] = 6;
		gotoxy(15, 7); std::cout << "MMMMMMMMMMMMMNX0kXXXXXXk0XNMMMMMMMMMMMMMMM" << std::endl;
		gotoxy(15, 8); std::cout << "MMMMMMMMMMMMMNX0kXXXXXXk0XNMMMMMMMMMMMMMMM" << std::endl;
		createInputField(8, " Email      ", 40, 65, colors[0], info[0]);
		gotoxy(15, 9); std::cout << "MMMMMMMMMNOd:'.            .':dONMMMMMMMMM" << std::endl;
		gotoxy(15, 10); std::cout << "MMMMMMW0o'                      'o0WMMMMMM" << std::endl;
		gotoxy(15, 11); std::cout << "MMMMW0c.                          .c0WMMMM" << std::endl;
		gotoxy(15, 12); std::cout << "MMMNd.                              .dNMMM" << std::endl;
		createInputField(12, " Password   ", 40, 65, colors[1], info[1]);
		gotoxy(15, 13); std::cout << "MMXl             ,oxkkxl'             lXMM" << std::endl;
		gotoxy(15, 14); std::cout << "MNo            .dNMMMMMMXc             oNM" << std::endl;
		gotoxy(15, 15); std::cout << "MO.            ;XMMMMMMMMO.            .OM" << std::endl;
		gotoxy(15, 16); std::cout << "Wl             .kWMMMMMMNo.             lW" << std::endl;
		gotoxy(15, 17); std::cout << "N:              .cx000Od;.              :N" << std::endl;
		gotoxy(15, 18); std::cout << "Nc             .;:lodooc:,.             cN" << std::endl;
		gotoxy(15, 19); std::cout << "Wd.          'dKWMMMMMMMMN0l.          .dW" << std::endl;
		gotoxy(15, 20); std::cout << "MK,         '0MMMMMMMMMMMMMWx.         ,KM" << std::endl;
		gotoxy(15, 21); std::cout << "MWk.        ;0WMMMMMMMMMMMMWk.        .kWM" << std::endl;
		gotoxy(15, 22); std::cout << "MMWk'       .,::::::::::::::,.       'kWMM" << std::endl;
		gotoxy(15, 23); std::cout << "MMMWKc.                            .cKMMMM" << std::endl;
		gotoxy(15, 24); std::cout << "MMMMMNO:.                        .:ONMMMMM" << std::endl;
		gotoxy(15, 25); std::cout << "MMMMMMMW0o;.                  .;o0WMMMMMMM" << std::endl;
		gotoxy(15, 26); std::cout << "MMMMMMMMMMWKkoc;,'......',;cokKWMMMMMMMMMM" << std::endl;
		gotoxy(15, 27); std::cout << "MMMMMMMMMMMMMMMWWNNXXNNWMMWMMMMMMMMMMMMMMM" << std::endl;
		gotoxy(15, 28); std::cout << "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM" << std::endl;
		color(6);

		key = _getch();

		if (key == '\r') 
		{
			iPut++;
			key = ' ';
		}

		else 
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

	} while (key != '\r' && iPut <= 1);

	LoginData account = { info[0], info[1] };
}

void registerPage()
{
	char key = ' ';
	int iPut = 0;
	string info[5] = { "", "", "", "", "" };
	bool firstShow = true;
	color(6);
	int colors[5] = { 7,7,7,7,7 };
	
	do 
	{
		colors[iPut] = 6;
		if (firstShow)
		{
			printLogo(30);
			gotoxy(15, 7); std::cout << "MMMMMMMMMMMMMNX0kXXXXXXk0XNMMMMMMMMMMMMMMM" << std::endl;
			gotoxy(15, 8); std::cout << "MMMMMMMMMMMMMNX0kXXXXXXk0XNMMMMMMMMMMMMMMM" << std::endl;
		}
		
		createInputField(8, " First name ", 40, 65, colors[0], info[0]);
		
		if (firstShow)
		{
			gotoxy(15, 9); std::cout << "MMMMMMMMMNOd:'.            .':dONMMMMMMMMM" << std::endl;
			gotoxy(15, 10); std::cout << "MMMMMMW0o'                      'o0WMMMMMM" << std::endl;
			gotoxy(15, 11); std::cout << "MMMMW0c.                          .c0WMMMM" << std::endl;
			gotoxy(15, 12); std::cout << "MMMNd.                              .dNMMM" << std::endl;
		}
			
		createInputField(12, " Last name  ", 40, 65, colors[1], info[1]);

		if (firstShow)
		{
			gotoxy(15, 13); std::cout << "MMXl             ,oxkkxl'             lXMM" << std::endl;
			gotoxy(15, 14); std::cout << "MNo            .dNMMMMMMXc             oNM" << std::endl;
			gotoxy(15, 15); std::cout << "MO.            ;XMMMMMMMMO.            .OM" << std::endl;
			gotoxy(15, 16); std::cout << "Wl             .kWMMMMMMNo.             lW" << std::endl;
		}
		
		createInputField(16, " Username   ", 40, 65, colors[2], info[2]);
		
		if (firstShow)
		{
			gotoxy(15, 17); std::cout << "N:              .cx000Od;.              :N" << std::endl;
			gotoxy(15, 18); std::cout << "Nc             .;:lodooc:,.             cN" << std::endl;
			gotoxy(15, 19); std::cout << "Wd.          'dKWMMMMMMMMN0l.          .dW" << std::endl;
			gotoxy(15, 20); std::cout << "MK,         '0MMMMMMMMMMMMMWx.         ,KM" << std::endl;
		}
		
		createInputField(20, " Email      ", 40, 65, colors[3], info[3]);

		if (firstShow)
		{
			gotoxy(15, 21); std::cout << "MWk.        ;0WMMMMMMMMMMMMWk.        .kWM" << std::endl;
			gotoxy(15, 22); std::cout << "MMWk'       .,::::::::::::::,.       'kWMM" << std::endl;
			gotoxy(15, 23); std::cout << "MMMWKc.                            .cKMMMM" << std::endl;
			gotoxy(15, 24); std::cout << "MMMMMNO:.                        .:ONMMMMM" << std::endl;
		}

		createInputField(24, " Password   ", 40, 65, colors[4], info[4]);
		
		if (firstShow)
		{
			gotoxy(15, 25); std::cout << "MMMMMMMW0o;.                  .;o0WMMMMMMM" << std::endl;
			gotoxy(15, 26); std::cout << "MMMMMMMMMMWKkoc;,'......',;cokKWMMMMMMMMMM" << std::endl;
			gotoxy(15, 27); std::cout << "MMMMMMMMMMMMMMMWWNNXXNNWMMWMMMMMMMMMMMMMMM" << std::endl;
			gotoxy(15, 28); std::cout << "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM" << std::endl;
			color(6);
		}

		firstShow = false;

		key = _getch();

		if (key == '\r') {
			iPut++;
			key = ' ';
		}

		else 
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

	} while (key != '\r' && iPut <= 4);

	RegisterData account = { info[0], info[1], info[2], info[3], info[4] };

	APIHandler newprof;
	newprof.registerHandler(account);
}

void accountPage(RegisterData acc)
{
	gotoxy(40, 6); color(6); std::cout << "A C C O U N T     S E T T I N G S"; color(7);
	gotoxy(43, 8); std::cout << "First name  ";
	gotoxy(58, 8); std::cout << acc.fname;
	gotoxy(44, 10); std::cout << "Last name  ";
	gotoxy(58, 10); std::cout << acc.lname;
	gotoxy(37, 12); std::cout << "Account Username  ";
	gotoxy(58, 12); std::cout << acc.username;
	gotoxy(40, 14); std::cout << "Email address  ";
	gotoxy(58, 14); std::cout << acc.email;
	gotoxy(45, 16); std::cout << "Password  ";
	gotoxy(58, 16); std::cout << acc.password;

	char key; // Key to be entered
	int SetColor[2] = { 2, 7 };
	int counter = 0;
	int ckey = 0;

	while (true)
	{
		// Button for history notebook section

		gotoxy(42, 18); color(SetColor[0]); std::cout << "U P D A T E";
		if (counter == 0) 
		{
			gotoxy(54, 18); color(7); std::cout << "<--";
		}
		if (counter == 1) 
		{
			gotoxy(54, 18); color(7); std::cout << "-->";
		}
		gotoxy(58, 18); color(SetColor[1]); std::cout << "D E L E T E";

		key = _getch();

		if (key == 75 && (counter == 1)) // 72 is the ASCII code for the up arrow
		{
			counter--;
		}

		if (key == 77 && (counter == 0)) // 80 is the ASCII code for the up arrow
		{
			counter++;
		}

		SetColor[0] = 7;
		SetColor[1] = 7;

		if (counter == 0) { SetColor[0] = 2; }
		if (counter == 1) { SetColor[1] = 4; }


	}

}
void adminPage(vector<string> systemOrg, vector<string> systemUser)
{
	char key; // Key to be entered
	int counter = 0, counter2 = 0;

	while (true)
	{
		if (counter2 == 0)
		{
			createButton(5, "        O R G A N I Z A T I O N S        ", 41, 15, 6);
			createButton(5, "            A L L   U S E R S            ", 41, 70, 8, 8);
		}

		else
		{
			createButton(5, "        O R G A N I Z A T I O N S        ", 41, 15, 8, 8);
			createButton(5, "            A L L   U S E R S            ", 41, 70, 6);
		}

		for (int i = 0; i < systemOrg.size(); i++)
		{
			counter2 == 0 ? counter == i ? color(6) : color(7) : color(8);
			gotoxy(15, 8 + i); std::cout << char(186) << " " << systemOrg[i];
		}

		for (int i = 0; i < systemUser.size(); i++)
		{
			counter2 == 1 ? counter == i ? color(6) : color(7) : color(8);
			gotoxy(70, 8 + i); std::cout << char(186) << " " << systemUser[i];
		}

		key = _getch();

		if (key == 72 && (counter >= 1 && counter <= systemOrg.size()))
		{
			counter--;
		}

		if (key == 80 && (counter >= 0 && counter < systemOrg.size() - 1))
		{
			counter++;
		}

		if (key == 75 && (counter2 == 1))
		{
			counter2--;
		}

		if (key == 77 && (counter2 == 0)) // 80/77 is the ASCII code for the up arrow
		{
			counter2++;
		}
	}
}

void drawOrganisationLogo()
{
	gotoxy(15, 7); std::cout << "WWWWWWWWWWWWWWWWWWWWWWMWWMMWMMWWWWWWWWWWWWWWWWWWWWW";
	gotoxy(15, 8); std::cout << "WWWWWWWWWWWWWWWWWWWWWNkdddddddKWWWWWWWWWWWWWWWWWWWW";
	gotoxy(15, 9); std::cout << "WWWWWWWWWWWWWWWWWWWWW0'       :OKNWWWWWWWWWWWWWWWWW";
	gotoxy(15, 10); std::cout << "WWWWWWWWWWWWWWWWWWWWW0'         ;KWWWWWWWWWWWWWWWWW";
	gotoxy(15, 11); std::cout << "WWWWWWWWWWWWWWWWWWWWW0' ;kOo.   ,KWWWWWWWWWWWWWWWWW";
	gotoxy(15, 12); std::cout << "WWWWWWWWWWWWWWWWWWWWWO. .cokddddkNWWWWWWWWWWWWWWWWW";
	gotoxy(15, 13); std::cout << "WWWWWWWWWWWWWWWWWWXkl'     .,lkXWWWWWWWWWWWWWWWWWWW";
	gotoxy(15, 14); std::cout << "WWWWWWWWWWWWWWN0d:.            .:d0NWWWWWWWWWWWWWWW";
	gotoxy(15, 15); std::cout << "WWWWWWWWWWWXkl,.                  .,lkXWWWWWWWWWWWW";
	gotoxy(15, 16); std::cout << "WWWWWWWN0d:.                          .:d0NWWWWWWWW";
	gotoxy(15, 17); std::cout << "WWWWWWNo.                                .oNWWWWWWW";
	gotoxy(15, 18); std::cout << "WWWWWWNkl,    .c:.    ;lcll;    .:c.    ,lkNWWWWWWW";
	gotoxy(15, 19); std::cout << "WWWWWWWWWx.   ;XK,   .OWWWMO.   ,KX;   .xWWWWWWWWWW";
	gotoxy(15, 20); std::cout << "WWWWWWWWWx.   ;XK,   .OWWWWO.   ,KX;   .xWWWWWWWWWW";
	gotoxy(15, 21); std::cout << "WWWWWWWWWx.   ;XK,   .OWWWWO.   ,KX;   .xWWWWWWWWWW";
	gotoxy(15, 22); std::cout << "WWWWWWWWWx.   ;XK,   .OWWWWO.   ,KX;   .xWWWWWWWWWW";
	gotoxy(15, 23); std::cout << "WWWWWWWWWx.   ;XK,   .OWWWWO.   ,KX;   .xWWWWWWWWWW";
	gotoxy(15, 24); std::cout << "WWWWWWWWWx.   ;XK,   .OWWWWO.   ,KX;   .xWWWWWWWWWW";
	gotoxy(15, 25); std::cout << "WWWWWWWK0l    ,kx.   .o0OO0o.   .xk'    l0KWWWWWWWW";
	gotoxy(15, 26); std::cout << "WWWWWNK:               .                  :KNWWWWWW";
	gotoxy(15, 27); std::cout << "WWWWO;.                                    .;0WWWWW";
	gotoxy(15, 28); std::cout << "WWWWXxodoododoodoodoooodooodoodoooodoodoododxXWWWWW";
	gotoxy(15, 29); std::cout << "WWWWWWMWMWWMWWMWMMWMWWMWWMMWMWWMWWMWWMWMWWMWWWWWWWW";
}
void viewOrganisationAdmin(string acc[], int orgSize)
{
	/*APIHandler apiHandler;
	apiHandler.getAllOrgs("eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXUyJ9.eyJleHAiOjE2NTEzOTQ4MTMsImlhdCI6MTY1MTMwODQxMywiaXNBZG1pbiI6MCwic3ViIjoiMSJ9.lFWn7AUKArrNMiqggBmnkOEHMt7sJFdshvN2JeCCtjw"); */
	system("cls");
	char key; // Key to be entered
	int counter = 0, counter2 = 0;
	int ckey = 0;

	while (true)
	{
		// Button for history notebook section
		int posy = 5;
		gotoxy(2, 3); color(6); std::cout << "O R G A N I S A T I O N S"; color(7);
		for (int i = 0; i < orgSize; i++) {
			gotoxy(4, posy); std::cout << "-->";
			if (i == counter) {
				color(6); gotoxy(9, posy); std::cout << acc[i];
				gotoxy(60, posy);
				if (counter2 == 0) {
					color(2); std::cout << "Enter"; color(7); std::cout << " | Delete";
				}
				else {
					color(7); std::cout << "Enter | "; color(4); std::cout << "Delete";
				}
			}
			else {
				gotoxy(9, posy); std::cout << acc[i];
				gotoxy(60, posy); color(8); std::cout << "Enter | Delete";
			}


			posy += 2;
			color(7);
		}

		key = _getch();

		if (key == 72 && (counter >= 1 && counter <= orgSize)) // 72/75 is the ASCII code for the up arrow
		{
			counter--;
		}

		if (key == 80 && (counter >= 0 && counter < orgSize - 1)) // 80/77 is the ASCII code for the up arrow
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
void viewOrganisation(string acc[], int orgSize)
{
	system("cls");

	char key; // Key to be entered
	int counter = 0, posy;

	while (true)
	{
		gotoxy(2, 3); color(6); std::cout << "O R G A N I S A T I O N S"; color(7);
		posy = 5;

		for (int i = 0; i < orgSize; i++)
		{
			gotoxy(4, posy); std::cout << "-->";
			i == counter ? color(6) : color(7);
			gotoxy(9, posy); std::cout << acc[i];

			posy += 2;
		}

		key = _getch();

		if (key == 72 && (counter >= 1 && counter <= orgSize)) // 72 is the ASCII code for the up arrow
		{
			counter--;
		}

		if (key == 80 && (counter >= 0 && counter < orgSize - 1)) // 80 is the ASCII code for the down arrow
		{
			counter++;
		}

	}
}
void createOrganisation()
{
	system("cls");
	char key = ' ';
	int iPut = 0;
	string info[5] = { "", "" };

	do
	{
		int colors[3] = { 7, 7 };
		colors[iPut] = 6;
		color(6);
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
}
void joinOrganisation()
{
	system("cls");
	char key = ' ';
	int iPut = 0;
	string info[5] = { "", "" };

	do
	{
		int colors[2] = { 7, 7 };
		colors[iPut] = 6;
		color(6);
		printLogo(26);
		drawOrganisationLogo();
		gotoxy(75, 9); cout << "You are not part of this organisation.";
		gotoxy(75, 10); cout << "Pleas enter the organisation password to continue!";
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
}

void updateOrganisation(string orgName, string orgPass)
{
	system("cls");
	char key = ' ';
	int iPut = 0;
	string info[5] = { orgName, orgPass };

	do
	{
		int colors[3] = { 7, 7 };
		colors[iPut] = 6;
		color(6);
		printLogo(26);
		drawOrganisationLogo();
		createInputField(8, " Course name", 40, 75, colors[0], info[0]);
		createInputField(12, " Password   ", 40, 75, colors[1], info[1]);

		if (iPut == 2)
		{
			createButton(16, "    Update    ", 14, 75, 2);
		}
		else {
			createButton(16, "    Update    ", 14, 75, 7);
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
}

void manageUserRoles(string acc[], int orgSize)
{
	system("cls");
	char key; // Key to be entered
	int counter = 0, counter2 = 0;

	while (true)
	{
		// Button for history notebook section
		int posy = 5;
		gotoxy(2, 3); color(6); std::cout << "O R G A N I Z A T I O N   U S E R S"; color(7);
		for (int i = 0; i < orgSize; i++)
		{
			gotoxy(4, posy); std::cout << "-->";

			if (i == counter)
			{
				color(6); gotoxy(9, posy); std::cout << acc[i];
				gotoxy(60, posy);

				if (counter2 == 0)
				{
					color(3); std::cout << "Teacher"; color(7); std::cout << " | Admin";
				}

				else
				{
					color(7); std::cout << "Teacher | "; color(3); std::cout << "Admin";
				}
			}

			else
			{
				gotoxy(9, posy); std::cout << acc[i];
				gotoxy(60, posy); color(8); std::cout << "Teacher | Admin";
			}


			posy += 2;
			color(7);
		}

		key = _getch();

		if (key == 72 && (counter >= 1 && counter <= orgSize)) // 72/75 is the ASCII code for the up arrow
		{
			counter--;
		}

		if (key == 80 && (counter >= 0 && counter < orgSize - 1)) // 80/77 is the ASCII code for the up arrow
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

void viewCoursesInOrganisationAdmin(string acc[], int orgSize)
{
	system("cls");
	char key; // Key to be entered
	int counter = 0, counter2 = 0;

	while (true)
	{
		int posy = 5;
		gotoxy(2, 3); color(6); std::cout << "O R G A N I S A T I O N   C O U R S E S"; color(7);
		for (int i = 0; i < orgSize; i++)
		{
			gotoxy(4, posy); std::cout << "-->";
			if (i == counter)
			{
				color(6); gotoxy(9, posy); std::cout << acc[i];
				gotoxy(60, posy);
				if (counter2 == 0)
				{
					color(2); std::cout << "Enter"; color(7); std::cout << " | Settings | Delete";
				}
				else if (counter2 == 1)
				{
					color(7); std::cout << "Enter | "; color(9); std::cout << "Settings"; color(7); std::cout << " | Delete";
				}
				else
				{
					color(7); std::cout << "Enter | Settings | "; color(4); std::cout << "Delete";
				}
			}
			else
			{
				gotoxy(9, posy); std::cout << acc[i];
				gotoxy(60, posy); color(8); std::cout << "Enter | Settings | Delete";
			}


			posy += 2;
			color(7);
		}

		key = _getch();

		if (key == 72 && (counter >= 1 && counter <= orgSize)) // 72/75 is the ASCII code for the up arrow
		{
			counter--;
		}

		if (key == 80 && (counter >= 0 && counter < orgSize - 1)) // 80/77 is the ASCII code for the up arrow
		{
			counter++;
		}

		if (key == 75 && (counter2 == 1) || key == 75 && (counter2 == 2)) // 72/75 is the ASCII code for the up arrow
		{
			counter2--;
		}

		if (key == 77 && (counter2 == 0) || key == 77 && (counter2 == 1) ) // 80/77 is the ASCII code for the up arrow
		{
			counter2++;
		}
	}
}
void viewCoursesInOrganisationUser(string acc[], int orgSize)
{
	system("cls");
	char key; // Key to be entered
	int orgsCounter = 0;

	while (true)
	{
		int posy = 5;
		
		gotoxy(2, 3); color(6); std::cout << "O R G A N I S A T I O N   C O U R S E S"; color(7);

		for (int i = 0; i < orgSize; i++)
		{
			gotoxy(4, posy); std::cout << "-->";
			if (i == orgsCounter)
			{
				color(6); gotoxy(9, posy); std::cout << acc[i];
				gotoxy(60, posy);
			}
			else
			{
				gotoxy(9, posy); std::cout << acc[i];
			}


			posy += 2;
			color(7);
		}

		key = _getch();

		if (key == 72 && (orgsCounter >= 1 && orgsCounter <= orgSize)) // 72/75 is the ASCII code for the up arrow
		{
			orgsCounter--;
		}

		if (key == 80 && (orgsCounter >= 0 && orgsCounter < orgSize - 1)) // 80/77 is the ASCII code for the up arrow
		{
			orgsCounter++;
		}
	}
}
void viewCoursesInOrganisationTeacher(string acc[], int orgSize)
{
	system("cls");
	char key; // Key to be entered
	int counter = 0, counter2 = 0;


	while (true)
	{
		int posy = 5;
		gotoxy(2, 3); color(6); std::cout << "O R G A N I S A T I O N   C O U R S E S"; color(7);
		for (int i = 0; i < orgSize; i++)
		{
			gotoxy(4, posy); std::cout << "-->";
			if (i == counter)
			{
				color(6); gotoxy(9, posy); std::cout << acc[i];
				gotoxy(60, posy);
				if (counter2 == 0)
				{
					color(2); std::cout << "Settings"; color(7); std::cout << " | Delete";
				}
				else
				{
					color(7); std::cout << "Settings | "; color(4); std::cout << "Delete";
				}
			}
			else
			{
				gotoxy(9, posy); std::cout << acc[i];
				gotoxy(60, posy); color(8); std::cout << "Settings | Delete";
			}


			posy += 2;
			color(7);
		}

		key = _getch();

		if (key == 72 && (counter >= 1 && counter <= orgSize)) // 72/75 is the ASCII code for the up arrow
		{
			counter--;
		}

		if (key == 80 && (counter >= 0 && counter < orgSize - 1)) // 80/77 is the ASCII code for the up arrow
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

void createCourse()
{
	string question = "", answer, teacher;
	char key;
	gotoxy(5, 4); std::cout << "C R E A T E   C O U R S E ";
	do
	{
		createInputField(6, " Course name", 40, 5, 6, question);
		createInputField(10, " Password   ", 40, 5, 7, " ");
		createInputField(14, " Add Teacher", 40, 5, 7, " ");
		createButton(18, "    SUBMIT    ", 14, 5, 2);

		key = _getch();

		if (key == '\r')
		{
			break;
		}

		key == '\b' ? question = question.substr(0, question.size() - 1) : question += key;

	} while (true);

	int subcol = 7;
	do
	{
		createInputField(6, " Course name", 40, 5, 7, question);
		createInputField(10, " Password   ", 40, 5, 6, answer);
		createInputField(14, " Add Teacher", 40, 5, 7, " ");
		createButton(18, "    SUBMIT    ", 14, 5, 2);

		key = _getch();

		if (key == '\r')
		{
			break;
		}

		key == '\b' ? answer = answer.substr(0, answer.size() - 1) : answer += key;

		answer.size() > 0 ? subcol = 2 : subcol = 7;

	} while (true);

	do
	{
		createInputField(6, " Course name", 40, 5, 7, question);
		createInputField(10, " Password   ", 40, 5, 7, answer);
		createInputField(14, " Add Teacher", 40, 5, 6, teacher);
		createButton(18, "    SUBMIT    ", 14, 5, 2);

		key = _getch();

		if (key == '\r')
		{
			break;
		}

		key == '\b' ? teacher = teacher.substr(0, teacher.size() - 1) : teacher += key;

		answer.size() > 0 ? subcol = 2 : subcol = 7;

	} while (true);

}
void manageCourse(string courseName, string password)
{
	char key;
	gotoxy(5, 4); std::cout << "M A N A G E   C O U R S E ";

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
}

void viewTopicsInCourse(string acc[], int orgSize)
{
	system("cls");
	char key; // Key to be entered
	int counter = 0, counter2 = 0;


	while (true)
	{
		int posy = 5;
		gotoxy(2, 3); color(6); std::cout << "C O U R S E ' S   T O P I C S"; color(7);
		for (int i = 0; i < orgSize; i++)
		{
			gotoxy(4, posy); std::cout << "-->";
			if (i == counter)
			{
				color(6); gotoxy(9, posy); std::cout << acc[i];
			}
			else
			{
				gotoxy(9, posy); std::cout << acc[i];
			}

			posy += 2;
			color(7);
		}

		key = _getch();

		if (key == 72 && (counter >= 1 && counter <= orgSize)) // 72/75 is the ASCII code for the up arrow
		{
			counter--;
		}

		if (key == 80 && (counter >= 0 && counter < orgSize - 1)) // 80/77 is the ASCII code for the up arrow
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
void updateDeleteTopicsInCourse(string acc[], int orgSize)
{
	system("cls");
	char key; // Key to be entered
	int counter = 0, counter2 = 0;

	while (true)
	{
		int posy = 5;
		gotoxy(2, 3); color(6); std::cout << "C O U R S E ' S   T O P I C S"; color(7);
		for (int i = 0; i < orgSize; i++)
		{
			gotoxy(4, posy); std::cout << "-->";
			if (i == counter)
			{
				color(6); gotoxy(9, posy); std::cout << acc[i];
				gotoxy(60, posy);
				if (counter2 == 0)
				{
					color(2); std::cout << "Settings"; color(7); std::cout << " | Delete";
				}
				else
				{
					color(7); std::cout << "Settings | "; color(4); std::cout << "Delete";
				}
			}
			else
			{
				gotoxy(9, posy); std::cout << acc[i];
				gotoxy(60, posy); color(8); std::cout << "Settings | Delete";
			}


			posy += 2;
			color(7);
		}

		key = _getch();

		if (key == 72 && (counter >= 1 && counter <= orgSize)) // 72/75 is the ASCII code for the up arrow
		{
			counter--;
		}

		if (key == 80 && (counter >= 0 && counter < orgSize - 1)) // 80/77 is the ASCII code for the up arrow
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
void addTopicsInCourse(string acc[], int orgSize)
{
	char key; // Key to be entered
	int counter = 0, counter2 = 0;


	while (true)
	{
		system("cls");
		int posy = 5;
		gotoxy(2, 3); color(6); std::cout << "C O U R S E ' S   T O P I C S"; color(7);
		for (int i = 0; i < orgSize; i++)
		{
			gotoxy(4, posy); std::cout << "-->";
			if (i == counter)
			{
				color(6); gotoxy(9, posy); std::cout << acc[i];
			}
			else
			{
				gotoxy(9, posy); std::cout << acc[i];
			}

			posy += 2;
			color(7);
		}
		createButton(posy, " + NEW TOPIC  ", 14, 4, 2);

		key = _getch();

		if (key == 72 && (counter >= 1 && counter <= orgSize)) // 72/75 is the ASCII code for the up arrow
		{
			counter--;
		}

		if (key == 80 && (counter >= 0 && counter < orgSize - 1)) // 80/77 is the ASCII code for the up arrow
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

		if (key == '+') 
		{
			string newTopic = "";
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
			acc[orgSize] = newTopic;
			orgSize++;
		}
	}
}

void viewLessonsInTopic(string acc[], int orgSize)
{
	system("cls");
	char key; // Key to be entered
	int counter = 0, counter2 = 0;

	while (true)
	{
		int posy = 5;
		gotoxy(2, 3); color(6); std::cout << "L E S S O N S  I N   T H E   T O P I C"; color(7);
		for (int i = 0; i < orgSize; i++)
		{
			gotoxy(4, posy); std::cout << "-->";
			if (i == counter)
			{
				color(6); gotoxy(9, posy); std::cout << acc[i];
			}
			else
			{
				gotoxy(9, posy); std::cout << acc[i];
			}

			posy += 2;
			color(7);
		}

		key = _getch();

		if (key == 72 && (counter >= 1 && counter <= orgSize)) // 72/75 is the ASCII code for the up arrow
		{
			counter--;
		}

		if (key == 80 && (counter >= 0 && counter < orgSize - 1)) // 80/77 is the ASCII code for the up arrow
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
void updateDeleteLessons(string acc[], int orgSize)
{
	system("cls");
	char key; // Key to be entered
	int counter = 0, counter2 = 0;

	while (true)
	{
		int posy = 5;
		gotoxy(2, 3); color(6); std::cout << "L E S S O N S  I N   T H E   T O P I C"; color(7);
		for (int i = 0; i < orgSize; i++)
		{
			gotoxy(4, posy); std::cout << "-->";
			if (i == counter)
			{
				color(6); gotoxy(9, posy); std::cout << acc[i];
				gotoxy(60, posy);
				if (counter2 == 0)
				{
					color(2); std::cout << "Settings"; color(7); std::cout << " | Delete";
				}
				else
				{
					color(7); std::cout << "Settings | "; color(4); std::cout << "Delete";
				}
			}
			else
			{
				gotoxy(9, posy); std::cout << acc[i];
				gotoxy(60, posy); color(8); std::cout << "Settings | Delete";
			}


			posy += 2;
			color(7);
		}

		key = _getch();

		if (key == 72 && (counter >= 1 && counter <= orgSize)) // 72/75 is the ASCII code for the up arrow
		{
			counter--;
		}

		if (key == 80 && (counter >= 0 && counter < orgSize - 1)) // 80/77 is the ASCII code for the up arrow
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
void addLesson(string acc[], int orgSize)
{
	char key; // Key to be entered
	int counter = 0, counter2 = 0;


	while (true)
	{
		system("cls");
		int posy = 5;
		gotoxy(2, 3); color(6); std::cout << "L E S S O N S  I N   T H E   T O P I C"; color(7);
		for (int i = 0; i < orgSize; i++)
		{
			gotoxy(4, posy); std::cout << "-->";
			if (i == counter)
			{
				color(6); gotoxy(9, posy); std::cout << acc[i];
			}
			else
			{
				gotoxy(9, posy); std::cout << acc[i];
			}

			posy += 2;
			color(7);
		}
		createButton(posy, " + NEW TOPIC  ", 14, 4, 2);

		key = _getch();

		if (key == 72 && (counter >= 1 && counter <= orgSize)) // 72/75 is the ASCII code for the up arrow
		{
			counter--;
		}

		if (key == 80 && (counter >= 0 && counter < orgSize - 1)) // 80/77 is the ASCII code for the up arrow
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

		if (key == '+')
		{
			string newTopic = "";
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
			acc[orgSize] = newTopic;
			orgSize++;
		}
	}
}

void addQuizz(string acc[], int orgSize)
{
	char key; // Key to be entered
	int counter = 0, counter2 = 0;


	while (true)
	{
		system("cls");
		int posy = 5;
		gotoxy(2, 3); color(6); std::cout << "Q U I Z Z E S"; color(7);
		for (int i = 0; i < orgSize; i++)
		{
			gotoxy(4, posy); std::cout << "-->";
			if (i == counter)
			{
				color(6); gotoxy(9, posy); std::cout << acc[i];
			}
			else
			{
				gotoxy(9, posy); std::cout << acc[i];
			}

			posy += 2;
			color(7);
		}
		createButton(posy, " + NEW TOPIC  ", 14, 4, 2);

		key = _getch();

		if (key == 72 && (counter >= 1 && counter <= orgSize)) // 72/75 is the ASCII code for the up arrow
		{
			counter--;
		}

		if (key == 80 && (counter >= 0 && counter < orgSize - 1)) // 80/77 is the ASCII code for the up arrow
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

		if (key == '+')
		{
			string newTopic = "";
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
			acc[orgSize] = newTopic;
			orgSize++;
		}
	}
}
void updateDeleteQuizz(string acc[], int orgSize)
{
	system("cls");
	char key; // Key to be entered
	int counter = 0, counter2 = 0;

	while (true)
	{
		int posy = 5;
		gotoxy(2, 3); color(6); std::cout << "Q U I Z Z E S"; color(7);
		for (int i = 0; i < orgSize; i++)
		{
			gotoxy(4, posy); std::cout << "-->";
			if (i == counter)
			{
				color(6); gotoxy(9, posy); std::cout << acc[i];
				gotoxy(60, posy);
				if (counter2 == 0)
				{
					color(2); std::cout << "Settings"; color(7); std::cout << " | Delete";
				}
				else
				{
					color(7); std::cout << "Settings | "; color(4); std::cout << "Delete";
				}
			}
			else
			{
				gotoxy(9, posy); std::cout << acc[i];
				gotoxy(60, posy); color(8); std::cout << "Settings | Delete";
			}


			posy += 2;
			color(7);
		}

		key = _getch();

		if (key == 72 && (counter >= 1 && counter <= orgSize)) // 72/75 is the ASCII code for the up arrow
		{
			counter--;
		}

		if (key == 80 && (counter >= 0 && counter < orgSize - 1)) // 80/77 is the ASCII code for the up arrow
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

void addQuestionToQuizz()
{
	string question = "", answer;
	char key;
	do {

		createButton(5, "   QUESTION   ", 14, 5, 6);
		createInputField(8, " TYPE HERE  ", 40, 5, 7, question);

		createButton(12, "    ANSWER    ", 14, 5, 8, 8);
		createInputField(15, " TYPE HERE  ", 40, 5, 8, " ", 8);
		createButton(19, "    SUBMIT    ", 14, 5, 2);
		key = _getch();

		if (key == '\b')
		{
			question = question.substr(0, question.size() - 1);
		}

		else if (key == '\r')
		{
			break;
		}
		else
		{
			question += key;
		}



	} while (key != '\r');

	int subcol = 7;
	do {
		createButton(5, "   QUESTION   ", 14, 5, 8, 8);
		createInputField(8, " TYPE HERE  ", 40, 5, 8, question, 8);
		createButton(12, "    ANSWER    ", 14, 5, 6);
		createInputField(15, " TYPE HERE  ", 40, 5, 7, answer);
		createButton(19, "    SUBMIT    ", 14, 5, 2, subcol);
		key = _getch();

		if (key == '\b')
		{
			answer = answer.substr(0, answer.size() - 1);
		}

		else
		{
			answer += key;
		}

		answer.size() > 0 ? subcol = 2 : subcol = 7;

	} while (key != '\r');
}
void displayQuestion()
{
	string info = "", question = "Who is the person who loves vscpi the most";
	char key;

	do {
		createButton(7, "     QUESTION    ", 17, 6, 6);
		gotoxy(26, 8); color(7); std::cout << question;
		createInputField(10, "  TYPE HERE ", 40, 6, 7, info);

		createButton(13, "      SUBMIT     ", 17, 6, 2);
		key = _getch();

		if (key == '\b') {
			info = info.substr(0, info.size() - 1);
		}
		else {
			info += key;
		}

	} while (key != '\r');
}

void displayNumber(int num, int posx, int posy)
{
	if (num == 1)
	{
		gotoxy(posx, posy + 2); std::cout << " __  ";
		gotoxy(posx, posy + 3); std::cout << "/_ | ";
		gotoxy(posx, posy + 4); std::cout << " | |";
		gotoxy(posx, posy + 5); std::cout << " | |";
		gotoxy(posx, posy + 6); std::cout << " | |";
		gotoxy(posx, posy + 7); std::cout << " |_| ";
	}
	if (num == 2)
	{
		gotoxy(posx, posy + 2); std::cout << " ___  ";
		gotoxy(posx, posy + 3); std::cout << "|__  \\  ";
		gotoxy(posx, posy + 4); std::cout << "   ) | ";
		gotoxy(posx, posy + 5); std::cout << "  / /  ";
		gotoxy(posx, posy + 6); std::cout << " / /_  ";
		gotoxy(posx, posy + 7); std::cout << "|____| ";
	}
	if (num == 3)
	{
		gotoxy(posx, posy + 2); std::cout << " ____";
		gotoxy(posx, posy + 3); std::cout << "|___ \\ ";
		gotoxy(posx, posy + 4); std::cout << "  __) | ";
		gotoxy(posx, posy + 5); std::cout << " |__ <  ";
		gotoxy(posx, posy + 6); std::cout << " ___) | ";
		gotoxy(posx, posy + 7); std::cout << "|____/  ";
	}
	if (num == 4)
	{
		gotoxy(posx, posy + 2); std::cout << " _  _   ";
		gotoxy(posx, posy + 3); std::cout << "| || |   ";
		gotoxy(posx, posy + 4); std::cout << "| || |_  ";
		gotoxy(posx, posy + 5); std::cout << "|__   _| ";
		gotoxy(posx, posy + 6); std::cout << "   | |   ";
		gotoxy(posx, posy + 7); std::cout << "   |_|   ";
	}
	if (num == 5)
	{
		gotoxy(posx, posy + 2); std::cout << " _____  ";
		gotoxy(posx, posy + 3); std::cout << "| ____| ";
		gotoxy(posx, posy + 4); std::cout << "| |__   ";
		gotoxy(posx, posy + 5); std::cout << "|___ \\  ";
		gotoxy(posx, posy + 6); std::cout << " ___) | ";
		gotoxy(posx, posy + 7); std::cout << "|____/  ";
	}
	if (num == 6) {
		gotoxy(posx, posy + 2); std::cout << "   __   ";
		gotoxy(posx, posy + 3); std::cout << "  / /   ";
		gotoxy(posx, posy + 4); std::cout << " / /_   ";
		gotoxy(posx, posy + 5); std::cout << "| '_ \\  ";
		gotoxy(posx, posy + 6); std::cout << "| (_) | ";
		gotoxy(posx, posy + 7); std::cout << " \\___/  ";
	}
	if (num == 7) {
		gotoxy(posx, posy + 2); std::cout << " ______  ";
		gotoxy(posx, posy + 3); std::cout << "|____  | ";
		gotoxy(posx, posy + 4); std::cout << "    / /  ";
		gotoxy(posx, posy + 5); std::cout << "   / /   ";
		gotoxy(posx, posy + 6); std::cout << "  / /    ";
		gotoxy(posx, posy + 7); std::cout << " /_/ ";
	}
	if (num == 8)
	{
		gotoxy(posx, posy + 2); std::cout << "  ___   ";
		gotoxy(posx, posy + 3); std::cout << " / _ \\  ";
		gotoxy(posx, posy + 4); std::cout << "| (_) | ";
		gotoxy(posx, posy + 5); std::cout << " > _ <  ";
		gotoxy(posx, posy + 6); std::cout << "| (_) | ";
		gotoxy(posx, posy + 7); std::cout << " \\___/ ";

	}

	if (num == 9)
	{
		gotoxy(posx, posy + 2); std::cout << "  ___  ";
		gotoxy(posx, posy + 3); std::cout << " / _ \\ ";
		gotoxy(posx, posy + 4); std::cout << "| (_) | ";
		gotoxy(posx, posy + 5); std::cout << " \\__, | ";
		gotoxy(posx, posy + 6); std::cout << "   / /  ";
		gotoxy(posx, posy + 7); std::cout << "  /_/  ";
	}

	if (num == 0) {
		gotoxy(posx, posy + 2); std::cout << "  ___   ";
		gotoxy(posx, posy + 3); std::cout << " / _ \\  ";
		gotoxy(posx, posy + 4); std::cout << "| | | | ";
		gotoxy(posx, posy + 5); std::cout << "| | | | ";
		gotoxy(posx, posy + 6); std::cout << "| |_| | ";
		gotoxy(posx, posy + 7); std::cout << " \\___/  ";
	}
}
void displayGrade(int points)
{
	gotoxy(40, 2); std::cout << char(218);
	for (int i = 0; i < 50; i++) {
		std::cout << char(196);
	}
	gotoxy(91, 2); std::cout << char(191);

	gotoxy(54, 5);  std::cout << "Y O U R   S C O R E   I S";

	for (int i = 0; i < 25; i++)
	{
		gotoxy(40, 3 + i);  std::cout << char(179);
		gotoxy(91, 3 + i);  std::cout << char(179);
	}

	color(3); displayNumber(points / 10, 58, 4);
	displayNumber(points % 10, 67, 4); color(7);
	gotoxy(62, 13);  std::cout << "O U T  O F";


	color(8); displayNumber(1, 55, 12);
	displayNumber(0, 62, 12);
	displayNumber(0, 71, 12); color(7);

	gotoxy(54, 21);  std::cout << "Y O U R "; color(3); std::cout << " S T A T U S "; color(7); std::cout << " I S";
	if (points < 40) {
		gotoxy(65, 23); color(4); std::cout << "B A D";
	}
	else if (points <= 70) {
		gotoxy(61, 23); color(6); std::cout << "A V E R A G E";
	}
	else if (points <= 100) {
		gotoxy(59, 23); color(2); std::cout << "E X C E L E N T";
	}
	color(7);
	gotoxy(40, 26); std::cout << char(192);
	for (int i = 0; i < 50; i++)
	{
		std::cout << char(196);
	}
	gotoxy(91, 26); std::cout << char(217);

}

void drawPunnettRows(int y, int start, int middSymb, int middSymb2, int end)
{
	gotoxy(5, y); std::cout << char(start);
	for (int j = 0; j < 5; j++)
	{
		for (int i = 0; i < 6; i++)
		{
			std::cout << char(middSymb);
		}
		j == 4 ? std::cout << char(end) : std::cout << char(middSymb2);
	}

}
void drawPunnettSquare()
{
	char key;
	int row = 0;
	string parents[8] = { "AB", "Ab","aB","ab","AB", "Ab","aB","ab" };

	int counter2 = 0, counter = 0;
	do {
		system("cls");
		row = 0;
		drawPunnettRows(8, 218, 196, 194, 191);
		gotoxy(2, 9 + counter); color(6);  std::cout << char(254); color(7);
		gotoxy(9 + counter2, 7); color(6);  std::cout << char(254); color(7);

		for (int i = 0; i < 36; i += 7)
		{
			gotoxy(5 + i, 9); std::cout << char(179) << " ";

		}
		for (int i = 0; i < 26; i += 7)
		{
			gotoxy(15 + i, 9); std::cout << parents[row];
			row++;
		}
		for (int i = 0; i < 8; i += 2)
		{
			gotoxy(8, 11 + i); std::cout << parents[row];
			row++;
		}
		drawPunnettRows(10, 195, 196, 197, 180);

		for (int i = 0; i < 36; i += 7)
		{
			gotoxy(5 + i, 11); std::cout << char(179);
		}

		drawPunnettRows(12, 195, 196, 197, 180);

		for (int i = 0; i < 36; i += 7)
		{
			gotoxy(5 + i, 13); std::cout << char(179);
		}

		drawPunnettRows(14, 195, 196, 197, 180);

		for (int i = 0; i < 36; i += 7)
		{
			gotoxy(5 + i, 15); std::cout << char(179);
		}

		drawPunnettRows(16, 195, 196, 197, 180);

		for (int i = 0; i < 36; i += 7)
		{
			gotoxy(5 + i, 17); std::cout << char(179);
		}

		drawPunnettRows(18, 192, 196, 193, 217);

		key = _getch();

		if (key == 72 && (counter >= 1 && counter <= 8)) // 72/75 is the ASCII code for the up arrow
		{
			counter -= 2;
		}

		if (key == 80 && (counter >= 0 && counter < 7)) // 80/77 is the ASCII code for the up arrow
		{
			counter += 2;
		}

		if (key == 75 && (counter2 >= 1 && counter2 <= 28)) // 72/75 is the ASCII code for the up arrow
		{
			counter2 -= 7;
		}

		if (key == 77 && (counter2 >= 0 && counter2 < 26)) // 80/77 is the ASCII code for the up arrow
		{
			counter2 += 7;
		}

	} while (key != '\r');
}

void navigationBar() //Main menu with three options
{
	char key; // Key to be entered
	int SetColor[5] = { 2, 7, 7, 7, 7 };
	int counter = 1;
	int ckey = 0;
	RegisterData acc = { "Vesko", "Stoyanov", "vesaka77", "veselincho_2@cod.bg", "vesilesi819" };
	while (true)
	{
		// Button for history notebook section
		gotoxy(0, 1); std::cout << "     Kuddlemuddle  ";

		color(SetColor[0]);
		createButton(1, "       MY PROFILE       ", 24, 5, SetColor[0]);
		color(7);


		// Button for timeline section
		color(SetColor[1]);
		createButton(1, "      ORGANIZATIONS     ", 24, 30, SetColor[1]);
		color(7);

		color(SetColor[2]);
		createButton(1, "        LOG OUT         ", 24, 100, SetColor[2]);
		color(7);

		for (int i = 0; i < 130; i++)
		{
			gotoxy(i, 4); std::cout << char(205);
		}

		key = _getch();

		if (key == 75 && (counter >= 2 && counter <= 3)) // 72 is the ASCII code for the up arrow
		{
			counter--;
		}

		if (key == 77 && (counter >= 1 && counter <= 2)) // 80 is the ASCII code for the up arrow
		{
			counter++;
		}

		if (key == '\r' && ckey % 2 == 0) // enter key
		{
			if (counter == 1)
			{
				accountPage(acc);
			}

			if (counter == 2)
			{
				system("CLS");
				string acc[5] = { "Vocational school of Programing", "United states of America",
		"Kethering und scisorss", "Code block for Specialists", "Cooking restaurnat um France" };
				viewOrganisationAdmin(acc, 5);
			}

			if (counter == 3)
			{
				system("CLS");
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

		if (key == '\t')
		{
			ckey++;
		}

		if (ckey % 2 == 0) {
			if (counter == 1) { SetColor[0] = 2; }
			if (counter == 2) { SetColor[1] = 2; }
			if (counter == 3) { SetColor[2] = 2; }
			if (counter == 4) { SetColor[3] = 12; }
		}

	}

	return;
}

void manageAccount(RegisterData userData)
{
	char key;
	gotoxy(5, 4); std::cout << "M A N A G E   A C C O U N T ";

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

}


int main()
{
	RegisterData data = { "Veselin", "Stoynov", "Vesko", "VStoynov19@da.bg", "Vesko123" };
	string courses[2] = {
		"Biology",
		"Math"
	};
	//registerPage();
	addQuestionToQuizz();
}
