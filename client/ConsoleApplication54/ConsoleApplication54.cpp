#include <iostream>
#include <conio.h>
#include <windows.h>
#include <APIHandler.h>
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

void drawButton(int posy, string text, int size, int posx, int col)
{
	gotoxy(posx, posy); cout << char(201);

	for (int i = 0; i < size; i++)
	{
		cout << char(205);
	}

	cout << char(187);
	gotoxy(posx, posy + 1); cout << char(186); color(7); cout << text; color(col);cout << char(186) << endl;
	gotoxy(posx, posy + 2); cout << char(200);

	for (int i = 0; i < size; i++)
	{
		cout << char(205);
	}

	cout << char(188);
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

		if (key == '\r' && ckey % 2 == 0) // enter key
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
			key = '\t';
		}

		
		SetColor[0] = 7;
		SetColor[1] = 7;
		SetColor[2] = 7;
		SetColor[3] = 7;

		if (key == '\t') 
		{
			ckey++;
		}

		if(ckey %2 == 0) {
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
	/*Menu();*/
	RegisterData a;
	APIHandler zed;

	std::cin >> a.fname;
	std::cin >> a.lname;
	std::cin >> a.username;
	std::cin >> a.email;
	std::cin >> a.password;
	
	zed.registerHandler(a);


}