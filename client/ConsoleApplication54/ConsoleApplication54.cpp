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

void drawButton(int posy, string text, int size, int posx, int col, string input = "")
{
	color(col);
	gotoxy(posx, posy); cout << char(201);

	for (int i = 0; i < 14; i++)
	{
		cout << char(205);
	}
	cout << char(203);
	for (int i = 0; i < size - 14; i++)
	{
		cout << char(205);
	}
	cout << char(187);

	gotoxy(posx, posy + 1); cout << char(186); color(7);
	cout << text << "  "; color(col); cout << char(186) << " ";
	color(7);
	size_t inputLength = 0;
	if (input.size() > 24) {
		cout << input.substr(input.size() - 24, input.size());
		inputLength = 24;
	}
	else {
		cout << input;
		inputLength = input.size();
	}

	for (size_t i = 0; i < 25 - inputLength; i++)
	{
		cout << " ";
	}
	color(col); cout << char(186) << endl;
	gotoxy(posx, posy + 2); cout << char(200);

	for (int i = 0; i < 14; i++)
	{
		cout << char(205);
	}
	cout << char(202);
	for (int i = 0; i < size - 14; i++)
	{
		cout << char(205);
	}

	cout << char(188);
	color(6);
}

RegisterData menuRegister() //Main menu with three options
{
	char key = ' ';
	int iPut = 0;
	string info[5] = { "", "", "", "", "" };

	color(6);
	do {

		int colors[5] = { 7,7,7,7,7 };
		colors[iPut] = 6;
		gotoxy(30, 0); cout << "     _             _  __         _     _ _                          _     _ _      " << endl;
		gotoxy(30, 1); cout << "  __| | __ _ ___  | |/ /   _  __| | __| | | ___ _ __ ___  _   _  __| | __| | | ___ " << endl;
		gotoxy(30, 2); cout << " / _` |/ _` / __| | ' / | | |/ _` |/ _` | |/ _ \\ '_ ` _ \\| | | |/ _` |/ _` | |/ _ \\" << endl;
		gotoxy(30, 3); cout << "| (_| | (_| \\__ \\ | . \\ |_| | (_| | (_| | |  __/ | | | | | |_| | (_| | (_| | |  __/" << endl;
		gotoxy(30, 4); cout << " \\__,_|\\__,_|___/ |_|\\_\\__,_|\\__,_|\\__,_|_|\\___|_| |_| |_|\\__,_|\\__,_|\\__,_|_|\\___|" << endl;
		gotoxy(15, 7); cout << "MMMMMMMMMMMMMNX0kXXXXXXk0XNMMMMMMMMMMMMMMM" << endl;
		gotoxy(15, 8); cout << "MMMMMMMMMMMMMNX0kXXXXXXk0XNMMMMMMMMMMMMMMM" << endl;
		drawButton(8, " First name ", 40, 65, colors[0], info[0]);
		gotoxy(15, 9); cout << "MMMMMMMMMNOd:'.            .':dONMMMMMMMMM" << endl;
		gotoxy(15, 10); cout << "MMMMMMW0o'                      'o0WMMMMMM" << endl;
		gotoxy(15, 11); cout << "MMMMW0c.                          .c0WMMMM" << endl;
		gotoxy(15, 12); cout << "MMMNd.                              .dNMMM" << endl;
		drawButton(12, " Last name  ", 40, 65, colors[1], info[1]);
		gotoxy(15, 13); cout << "MMXl             ,oxkkxl'             lXMM" << endl;
		gotoxy(15, 14); cout << "MNo            .dNMMMMMMXc             oNM" << endl;
		gotoxy(15, 15); cout << "MO.            ;XMMMMMMMMO.            .OM" << endl;
		gotoxy(15, 16); cout << "Wl             .kWMMMMMMNo.             lW" << endl;
		drawButton(16, " Username   ", 40, 65, colors[2], info[2]);
		gotoxy(15, 17); cout << "N:              .cx000Od;.              :N" << endl;
		gotoxy(15, 18); cout << "Nc             .;:lodooc:,.             cN" << endl;
		gotoxy(15, 19); cout << "Wd.          'dKWMMMMMMMMN0l.          .dW" << endl;
		gotoxy(15, 20); cout << "MK,         '0MMMMMMMMMMMMMWx.         ,KM" << endl;
		drawButton(20, " Email      ", 40, 65, colors[3], info[3]);
		gotoxy(15, 21); cout << "MWk.        ;0WMMMMMMMMMMMMWk.        .kWM" << endl;
		gotoxy(15, 22); cout << "MMWk'       .,::::::::::::::,.       'kWMM" << endl;
		gotoxy(15, 23); cout << "MMMWKc.                            .cKMMMM" << endl;
		gotoxy(15, 24); cout << "MMMMMNO:.                        .:ONMMMMM" << endl;
		drawButton(24, " Password   ", 40, 65, colors[4], info[4]);
		gotoxy(15, 25); cout << "MMMMMMMW0o;.                  .;o0WMMMMMMM" << endl;
		gotoxy(15, 26); cout << "MMMMMMMMMMWKkoc;,'......',;cokKWMMMMMMMMMM" << endl;
		gotoxy(15, 27); cout << "MMMMMMMMMMMMMMMWWNNXXNNWMMWMMMMMMMMMMMMMMM" << endl;
		gotoxy(15, 28); cout << "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM" << endl;
		color(6);

		key = _getch();

		if (key == '\r') {
			iPut++;
			key = ' ';
		}

		else {
			if (key == '\b') {
				info[iPut] = info[iPut].substr(0, info[iPut].size() - 1);
			}
			else {
				info[iPut] += key;
			}

		}

	} while (key != '\r' && iPut <= 4);

	RegisterData accaunt = { info[0], info[1], info[2], info[3], info[4] };
	
	APIHandler newprof;
	newprof.registerHandler(accaunt);
	return accaunt;
}


void menuLogin() 
{
	system("cls");
	char key = ' ';
	int iPut = 0;
	string info[5] = { "", "" };

	color(6);
	do {

		int colors[2] = { 7,7 };
		colors[iPut] = 6;
		gotoxy(30, 0); cout << "     _             _  __         _     _ _                          _     _ _      " << endl;
		gotoxy(30, 1); cout << "  __| | __ _ ___  | |/ /   _  __| | __| | | ___ _ __ ___  _   _  __| | __| | | ___ " << endl;
		gotoxy(30, 2); cout << " / _` |/ _` / __| | ' / | | |/ _` |/ _` | |/ _ \\ '_ ` _ \\| | | |/ _` |/ _` | |/ _ \\" << endl;
		gotoxy(30, 3); cout << "| (_| | (_| \\__ \\ | . \\ |_| | (_| | (_| | |  __/ | | | | | |_| | (_| | (_| | |  __/" << endl;
		gotoxy(30, 4); cout << " \\__,_|\\__,_|___/ |_|\\_\\__,_|\\__,_|\\__,_|_|\\___|_| |_| |_|\\__,_|\\__,_|\\__,_|_|\\___|" << endl;
		gotoxy(15, 7); cout << "MMMMMMMMMMMMMNX0kXXXXXXk0XNMMMMMMMMMMMMMMM" << endl;
		gotoxy(15, 8); cout << "MMMMMMMMMMMMMNX0kXXXXXXk0XNMMMMMMMMMMMMMMM" << endl;
		drawButton(8, " Email      ", 40, 65, colors[0], info[0]);
		gotoxy(15, 9); cout << "MMMMMMMMMNOd:'.            .':dONMMMMMMMMM" << endl;
		gotoxy(15, 10); cout << "MMMMMMW0o'                      'o0WMMMMMM" << endl;
		gotoxy(15, 11); cout << "MMMMW0c.                          .c0WMMMM" << endl;
		gotoxy(15, 12); cout << "MMMNd.                              .dNMMM" << endl;
		drawButton(12, " Password   ", 40, 65, colors[1], info[1]);
		gotoxy(15, 13); cout << "MMXl             ,oxkkxl'             lXMM" << endl;
		gotoxy(15, 14); cout << "MNo            .dNMMMMMMXc             oNM" << endl;
		gotoxy(15, 15); cout << "MO.            ;XMMMMMMMMO.            .OM" << endl;
		gotoxy(15, 16); cout << "Wl             .kWMMMMMMNo.             lW" << endl;
		gotoxy(15, 17); cout << "N:              .cx000Od;.              :N" << endl;
		gotoxy(15, 18); cout << "Nc             .;:lodooc:,.             cN" << endl;
		gotoxy(15, 19); cout << "Wd.          'dKWMMMMMMMMN0l.          .dW" << endl;
		gotoxy(15, 20); cout << "MK,         '0MMMMMMMMMMMMMWx.         ,KM" << endl;
		gotoxy(15, 21); cout << "MWk.        ;0WMMMMMMMMMMMMWk.        .kWM" << endl;
		gotoxy(15, 22); cout << "MMWk'       .,::::::::::::::,.       'kWMM" << endl;
		gotoxy(15, 23); cout << "MMMWKc.                            .cKMMMM" << endl;
		gotoxy(15, 24); cout << "MMMMMNO:.                        .:ONMMMMM" << endl;
		gotoxy(15, 25); cout << "MMMMMMMW0o;.                  .;o0WMMMMMMM" << endl;
		gotoxy(15, 26); cout << "MMMMMMMMMMWKkoc;,'......',;cokKWMMMMMMMMMM" << endl;
		gotoxy(15, 27); cout << "MMMMMMMMMMMMMMMWWNNXXNNWMMWMMMMMMMMMMMMMMM" << endl;
		gotoxy(15, 28); cout << "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM" << endl;
		color(6);

		key = _getch();

		if (key == '\r') {
			iPut++;
			key = ' ';
		}

		else {
			if (key == '\b') {
				info[iPut] = info[iPut].substr(0, info[iPut].size() - 1);
			}
			else {
				info[iPut] += key;
			}

		}

	} while (key != '\r' && iPut <= 1);

	LoginData accaunt = { info[0], info[1]};
}


void accountPage(RegisterData acc) 
{
	system("cls");
	gotoxy(40, 3); color(6); cout << "A C C O U N T     S E T T I N G S"; color(7);
	gotoxy(43, 5); cout << "First name  ";
	gotoxy(58, 5); cout << acc.fname;
	gotoxy(44, 7); cout << "Last name  ";
	gotoxy(58, 7); cout << acc.lname;
	gotoxy(37, 9); cout << "Accaunt Username  ";
	gotoxy(58, 9); cout << acc.username;
	gotoxy(40, 11); cout << "Email address  ";
	gotoxy(58, 11); cout << acc.email;
	gotoxy(45, 13); cout << "Password  ";
	gotoxy(58, 13); cout << acc.password;

	char key; // Key to be entered
	int SetColor[2] = { 2, 7 };
	int counter = 0;
	int ckey = 0;

	while (true)
	{
		// Button for history notebook section
		
		gotoxy(46, 15); color(SetColor[0]); cout << "U P D A T E";
		if (counter == 0) {
			gotoxy(54, 15); color(7); cout << "<--";
		}
		if (counter == 1) {
			gotoxy(54, 15); color(7); cout << "-->";
		}
		gotoxy(58, 15); color(SetColor[1]); cout << "D E L E T E";
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

void printOrganisation(string acc[], int orgSize) 
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
		gotoxy(2, 3); color(6); cout << "O R G A N I S A T I O N S"; color(7);
		for (int i = 0; i < orgSize; i++) {
			gotoxy(4, posy); cout << "-->";
			if (i == counter) {
				color(6); gotoxy(9, posy); cout << acc[i];
				gotoxy(60, posy); 
				if (counter2 == 0) {
					color(2); cout << "Update"; color(7); cout << " | Delete";
				}
				else {
					color(7); cout << "Update | "; color(4); cout << "Delete";
				}
			}
			else {
				gotoxy(9, posy); cout << acc[i];
				gotoxy(60, posy); color(8); cout << "Update | Delete";
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
		
		if (key == 75 && (counter2 == 1 )) // 72/75 is the ASCII code for the up arrow
		{
			counter2--;
		}

		if (key == 77 && (counter2 == 0)) // 80/77 is the ASCII code for the up arrow
		{
			counter2++;
		}


	}
}

int main()
{
	/*RegisterData acc = menuRegister();
	menuLogin();
	accountPage(acc);*/
	string acc[5] = { "Vocational school of Programing", "United states of America",
		"Kethering und scisorss", "Code block for Specialists", "Cooking restaurnat um France"};
	//accountPage(acc);
	printOrganisation(acc, 5);
}
	