#include <iostream>
#include <APIHandler.h>
#include <conio.h>
#include <windows.h>

using namespace std;

bool consoleOpened = false; //Checks if console is opened now and extracts data from files

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
	cout << text << "  "; color(col); cout << char(186) << " ";  color(7); cout << input;
	for (size_t i = 0; i < 25 - input.size(); i++)
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

	while (true)
	{
		// Button for history notebook section
		gotoxy(0, 1); cout << "     Kuddlemuddle  ";

		color(SetColor[0]);
		drawButton(1, "         QUIZZES        ", 24, 5, SetColor[0]);
		color(7);


		// Button for timeline section
		color(SetColor[1]);
		drawButton(1, "         LESSONS        ", 24, 30, SetColor[1]);
		color(7);


		// Button for Search Engine
		color(SetColor[2]);
		drawButton(1, "           OTHERS       ", 24, 55, SetColor[2]);
		color(7);

		color(SetColor[3]);
		drawButton(1, "        Log out         ", 24, 100, SetColor[3]);
		color(7);

		for (int i = 0; i < 130; i++)
		{
			gotoxy(i, 4); cout << char(205);
		}

		key = _getch();

		if (key == 75 && (counter >= 2 && counter <= 4)) // 72 is the ASCII code for the up arrow
		{
			counter--;
		}

		if (key == 77 && (counter >= 1 && counter <= 3)) // 80 is the ASCII code for the up arrow
		{
			counter++;
		}

		if (key == '\\\\r' && ckey % 2 == 0) // enter key
		{
			if (counter == 1)
			{
				system("CLS");
				tr("Genetics");
			}

			if (counter == 2)
			{
				system("CLS");
				tr("BioSphere");
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

		if (key == '\\\\t')
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
				info[iPut] = info[iPut].c_str()
			}
			info[iPut] += key;
		}


	} while (key != '\r' && iPut <= 4);

	RegisterData accaunt = { info[0], info[1], info[2], info[3], info[4] };

	APIHandler newprof;
	newprof.registerHandler(accaunt);

}