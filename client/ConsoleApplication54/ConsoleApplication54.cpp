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

void drawBox(int posy, string text, int size, int posx, int col, string input = "")
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
		drawBox(8, " First name ", 40, 65, colors[0], info[0]);
		gotoxy(15, 9); cout << "MMMMMMMMMNOd:'.            .':dONMMMMMMMMM" << endl;
		gotoxy(15, 10); cout << "MMMMMMW0o'                      'o0WMMMMMM" << endl;
		gotoxy(15, 11); cout << "MMMMW0c.                          .c0WMMMM" << endl;
		gotoxy(15, 12); cout << "MMMNd.                              .dNMMM" << endl;
		drawBox(12, " Last name  ", 40, 65, colors[1], info[1]);
		gotoxy(15, 13); cout << "MMXl             ,oxkkxl'             lXMM" << endl;
		gotoxy(15, 14); cout << "MNo            .dNMMMMMMXc             oNM" << endl;
		gotoxy(15, 15); cout << "MO.            ;XMMMMMMMMO.            .OM" << endl;
		gotoxy(15, 16); cout << "Wl             .kWMMMMMMNo.             lW" << endl;
		drawBox(16, " Username   ", 40, 65, colors[2], info[2]);
		gotoxy(15, 17); cout << "N:              .cx000Od;.              :N" << endl;
		gotoxy(15, 18); cout << "Nc             .;:lodooc:,.             cN" << endl;
		gotoxy(15, 19); cout << "Wd.          'dKWMMMMMMMMN0l.          .dW" << endl;
		gotoxy(15, 20); cout << "MK,         '0MMMMMMMMMMMMMWx.         ,KM" << endl;
		drawBox(20, " Email      ", 40, 65, colors[3], info[3]);
		gotoxy(15, 21); cout << "MWk.        ;0WMMMMMMMMMMMMWk.        .kWM" << endl;
		gotoxy(15, 22); cout << "MMWk'       .,::::::::::::::,.       'kWMM" << endl;
		gotoxy(15, 23); cout << "MMMWKc.                            .cKMMMM" << endl;
		gotoxy(15, 24); cout << "MMMMMNO:.                        .:ONMMMMM" << endl;
		drawBox(24, " Password   ", 40, 65, colors[4], info[4]);
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

	RegisterData account = { info[0], info[1], info[2], info[3], info[4] };

	APIHandler newprof;
	newprof.registerHandler(account);
	return account;
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
		drawBox(8, " Email      ", 40, 65, colors[0], info[0]);
		gotoxy(15, 9); cout << "MMMMMMMMMNOd:'.            .':dONMMMMMMMMM" << endl;
		gotoxy(15, 10); cout << "MMMMMMW0o'                      'o0WMMMMMM" << endl;
		gotoxy(15, 11); cout << "MMMMW0c.                          .c0WMMMM" << endl;
		gotoxy(15, 12); cout << "MMMNd.                              .dNMMM" << endl;
		drawBox(12, " Password   ", 40, 65, colors[1], info[1]);
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

	LoginData account = { info[0], info[1] };
}

void accountPage(RegisterData acc)
{
	gotoxy(40, 6); color(6); cout << "A C C O U N T     S E T T I N G S"; color(7);
	gotoxy(43, 8); cout << "First name  ";
	gotoxy(58, 8); cout << acc.fname;
	gotoxy(44, 10); cout << "Last name  ";
	gotoxy(58, 10); cout << acc.lname;
	gotoxy(37, 12); cout << "Account Username  ";
	gotoxy(58, 12); cout << acc.username;
	gotoxy(40, 14); cout << "Email address  ";
	gotoxy(58, 14); cout << acc.email;
	gotoxy(45, 16); cout << "Password  ";
	gotoxy(58, 16); cout << acc.password;

	char key; // Key to be entered
	int SetColor[2] = { 2, 7 };
	int counter = 0;
	int ckey = 0;

	while (true)
	{
		// Button for history notebook section

		gotoxy(42, 18); color(SetColor[0]); cout << "U P D A T E";
		if (counter == 0) {
			gotoxy(54, 18); color(7); cout << "<--";
		}
		if (counter == 1) {
			gotoxy(54, 18); color(7); cout << "-->";
		}
		gotoxy(58, 18); color(SetColor[1]); cout << "D E L E T E";
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

void printOrganisationAdmin(string acc[], int orgSize)
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

void drawButton(int posy, string text, int size, int posx, int col)
{
	color(col); gotoxy(posx, posy); cout << char(201);

	for (int i = 0; i < size; i++)
	{
		cout << char(205);
	}

	cout << char(187);
	gotoxy(posx, posy + 1); cout << char(186); color(7); cout << text; color(col); cout << char(186) << endl;
	gotoxy(posx, posy + 2); cout << char(200);

	for (int i = 0; i < size; i++)
	{
		cout << char(205);
	}

	cout << char(188); color(7);

}

void displayQuestion()
{
	string info = "", question = "Who is the person who loves vscpi the most";
	char key;

	do {
		drawButton(7, "     QUESTION    ", 17, 6, 6);
		gotoxy(26, 8); color(7); cout << question;
		drawBox(10, "  TYPE HERE ", 40, 6, 7, info);

		drawButton(13, "      SUBMIT     ", 17, 6, 2);
		key = _getch();

		if (key == '\b') {
			info = info.substr(0, info.size() - 1);
		}
		else {
			info += key;
		}

	} while (key != '\r');
}

void printOrganisationUser(string acc[], int orgSize)
{
	/*APIHandler apiHandler;
	apiHandler.getAllOrgs("eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXUyJ9.eyJleHAiOjE2NTEzOTQ4MTMsImlhdCI6MTY1MTMwODQxMywiaXNBZG1pbiI6MCwic3ViIjoiMSJ9.lFWn7AUKArrNMiqggBmnkOEHMt7sJFdshvN2JeCCtjw"); */
	system("cls");


	char key; // Key to be entered
	int counter = 0;

	while (true)
	{
		// Button for history notebook section
		int posy = 5;
		gotoxy(2, 3); color(6); cout << "O R G A N I S A T I O N S"; color(7);
		for (int i = 0; i < orgSize; i++) {
			gotoxy(4, posy); cout << "-->";
			if (i == counter) {
				color(6); gotoxy(9, posy); cout << acc[i];
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
		gotoxy(2, 9 + counter); color(6);  cout << char(254); color(7);
		gotoxy(9 + counter2, 7); color(6);  cout << char(254); color(7);

		for (int i = 0; i < 36; i += 7)
		{
			gotoxy(5 + i, 9); cout << char(179) << " ";

		}
		for (int i = 0; i < 26; i += 7)
		{
			gotoxy(15 + i, 9); cout << parents[row];
			row++;
		}
		for (int i = 0; i < 8; i += 2)
		{
			gotoxy(8, 11 + i); cout << parents[row];
			row++;
		}
		drawPunnettRows(10, 195, 196, 197, 180);

		for (int i = 0; i < 36; i += 7)
		{
			gotoxy(5 + i, 11); cout << char(179);
		}

		drawPunnettRows(12, 195, 196, 197, 180);

	}
void createQuestion()
{
	string question = "", answer;
	char key;
	do {

		drawButton(5, "   QUESTION   ", 14, 5, 6);
		drawBox(8, " TYPE HERE  ", 40, 5, 7, question);

		drawButton(12, "    ANSWER    ", 14, 5, 8, 8);
		drawBox(15, " TYPE HERE  ", 40, 5, 8, " ", 8);
		drawButton(19, "    SUBMIT    ", 14, 5, 2);
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
		drawButton(5, "   QUESTION   ", 14, 5, 8, 8);
		drawBox(8, " TYPE HERE  ", 40, 5, 8, question, 8);
		drawButton(12, "    ANSWER    ", 14, 5, 6);
		drawBox(15, " TYPE HERE  ", 40, 5, 7, answer);
		drawButton(19, "    SUBMIT    ", 14, 5, 2, subcol);
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

void displayNumber(int num, int posx, int posy)
{
	if (num == 1)
	{
		gotoxy(posx, posy + 2); cout << " __  ";
		gotoxy(posx, posy + 3); cout << "/_ | ";
		gotoxy(posx, posy + 4); cout << " | |";
		gotoxy(posx, posy + 5); cout << " | |";
		gotoxy(posx, posy + 6); cout << " | |";
		gotoxy(posx, posy + 7); cout << " |_| ";
	}
	if (num == 2)
	{
		gotoxy(posx, posy + 2); cout << " ___  ";
		gotoxy(posx, posy + 3); cout << "|__  \\  ";
		gotoxy(posx, posy + 4); cout << "   ) | ";
		gotoxy(posx, posy + 5); cout << "  / /  ";
		gotoxy(posx, posy + 6); cout << " / /_  ";
		gotoxy(posx, posy + 7); cout << "|____| ";
	}
	if (num == 3)
	{
		gotoxy(posx, posy + 2); cout << " ____";
		gotoxy(posx, posy + 3); cout << "|___ \\ ";
		gotoxy(posx, posy + 4); cout << "  __) | ";
		gotoxy(posx, posy + 5); cout << " |__ <  ";
		gotoxy(posx, posy + 6); cout << " ___) | ";
		gotoxy(posx, posy + 7); cout << "|____/  ";
	}
	if (num == 4)
	{
		gotoxy(posx, posy + 2); cout << " _  _   ";
		gotoxy(posx, posy + 3); cout << "| || |   ";
		gotoxy(posx, posy + 4); cout << "| || |_  ";
		gotoxy(posx, posy + 5); cout << "|__   _| ";
		gotoxy(posx, posy + 6); cout << "   | |   ";
		gotoxy(posx, posy + 7); cout << "   |_|   ";
	}
	if (num == 5)
	{
		gotoxy(posx, posy + 2); cout << " _____  ";
		gotoxy(posx, posy + 3); cout << "| ____| ";
		gotoxy(posx, posy + 4); cout << "| |__   ";
		gotoxy(posx, posy + 5); cout << "|___ \\  ";
		gotoxy(posx, posy + 6); cout << " ___) | ";
		gotoxy(posx, posy + 7); cout << "|____/  ";
	}
	if (num == 6) {
		gotoxy(posx, posy + 2); cout << "   __   ";
		gotoxy(posx, posy + 3); cout << "  / /   ";
		gotoxy(posx, posy + 4); cout << " / /_   ";
		gotoxy(posx, posy + 5); cout << "| '_ \\  ";
		gotoxy(posx, posy + 6); cout << "| (_) | ";
		gotoxy(posx, posy + 7); cout << " \\___/  ";
	}
	if (num == 7) {
		gotoxy(posx, posy + 2); cout << " ______  ";
		gotoxy(posx, posy + 3); cout << "|____  | ";
		gotoxy(posx, posy + 4); cout << "    / /  ";
		gotoxy(posx, posy + 5); cout << "   / /   ";
		gotoxy(posx, posy + 6); cout << "  / /    ";
		gotoxy(posx, posy + 7); cout << " /_/ ";
	}
	if (num == 8)
	{
		gotoxy(posx, posy + 2); cout << "  ___   ";
		gotoxy(posx, posy + 3); cout << " / _ \\  ";
		gotoxy(posx, posy + 4); cout << "| (_) | ";
		gotoxy(posx, posy + 5); cout << " > _ <  ";
		gotoxy(posx, posy + 6); cout << "| (_) | ";
		gotoxy(posx, posy + 7); cout << " \\___/ ";

	}

	if (num == 9)
	{
		gotoxy(posx, posy + 2); cout << "  ___  ";
		gotoxy(posx, posy + 3); cout << " / _ \\ ";
		gotoxy(posx, posy + 4); cout << "| (_) | ";
		gotoxy(posx, posy + 5); cout << " \\__, | ";
		gotoxy(posx, posy + 6); cout << "   / /  ";
		gotoxy(posx, posy + 7); cout << "  /_/  ";
	}

	if (num == 0) {
		gotoxy(posx, posy + 2); cout << "  ___   ";
		gotoxy(posx, posy + 3); cout << " / _ \\  ";
		gotoxy(posx, posy + 4); cout << "| | | | ";
		gotoxy(posx, posy + 5); cout << "| | | | ";
		gotoxy(posx, posy + 6); cout << "| |_| | ";
		gotoxy(posx, posy + 7); cout << " \\___/  ";
	}
}

void displayGrade(int points)
{
	gotoxy(40, 2); cout << char(218);
	for (int i = 0; i < 50; i++) {
		cout << char(196);
	}
	gotoxy(91, 2); cout << char(191);

	gotoxy(54, 5);  cout << "Y O U R   S C O R E   I S";

	for (int i = 0; i < 25; i++)
	{
		gotoxy(40, 3 + i);  cout << char(179);
		gotoxy(91, 3 + i);  cout << char(179);
	}

	color(3); displayNumber(points / 10, 58, 4);
	displayNumber(points % 10, 67, 4); color(7);
	gotoxy(62, 13);  cout << "O U T  O F";


	color(8); displayNumber(1, 55, 12);
	displayNumber(0, 62, 12);
	displayNumber(0, 71, 12); color(7);

	gotoxy(54, 21);  cout << "Y O U R "; color(3); cout << " S T A T U S "; color(7); cout << " I S";
	if (points < 40) {
		gotoxy(65, 23); color(4); cout << "B A D";
	}
	else if (points <= 70) {
		gotoxy(61, 23); color(6); cout << "A V E R A G E";
	}
	else if (points <= 100) {
		gotoxy(59, 23); color(2); cout << "E X C E L E N T";
	}
	color(7);
	gotoxy(40, 26); cout << char(192);
	for (int i = 0; i < 50; i++)
	{
		cout << char(196);
	}
	gotoxy(91, 26); cout << char(217);

}

void panetTable() {

}

void tr(string st)
{
	gotoxy(80, 10);  cout << "this is page bout " << st;
}

int Menu() //Main menu with three options
{
	char key; // Key to be entered
	int SetColor[5] = { 2, 7, 7, 7, 7 };
	int counter = 1;
	int ckey = 0;
	RegisterData acc = { "Vesko", "Stoyanov", "vesaka77", "veselincho_2@cod.bg", "vesilesi819" };
	while (true)
	{
		// Button for history notebook section
		gotoxy(0, 1); cout << "     Kuddlemuddle  ";

		color(SetColor[0]);
		drawButton(1, "       MY PROFILE       ", 24, 5, SetColor[0]);
		color(7);


		// Button for timeline section
		color(SetColor[1]);
		drawButton(1, "      ORGANIZATIONS     ", 24, 30, SetColor[1]);
		color(7);

		color(SetColor[2]);
		drawButton(1, "        LOG OUT         ", 24, 100, SetColor[2]);
		color(7);

		for (int i = 0; i < 130; i++)
		{
			gotoxy(i, 4); cout << char(205);
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
				printOrganisationAdmin(acc, 5);
			}

			if (counter == 3)
			{
				system("CLS");
				tr("Sells");
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

	return 0;
}

int main()
{
	/*RegisterData acc = menuRegister();
	menuLogin();
	accountPage(acc);*/
	/*string acc[5] = { "Vocational school of Programing", "United states of America",
		"Kethering und scisorss", "Code block for Specialists", "Cooking restaurnat um France"};*/

	/*Menu();*/
	//displayQuestion();
	//displayGrade(34);

}
