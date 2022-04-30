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

int menuRegister() //Main menu with three options
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

int main()
{
	menuRegister();
	menuLogin();
}