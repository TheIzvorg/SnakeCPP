#include <iostream>;
#include <Windows.h>;
#include <conio.h>;
#include <ctime>;
#include <string>;

//� ������ ������ �� �� ���������� ������������ ���� STD ������ ��� ��� �������� � ������ � ����� ���� ����� std::cout

#pragma warning(disable:4996)

// ����������

void end();
void settings();
void run();
void printMap();
void initMap();
void move(int dx, int dy);
void update();
void changeDirection(char key);
void clearScreen();
void generateFood();
char getMapValue(int value);
void setColor(char symbol, int color);

// ��������� �����
const int mapwidth = 20;
const int mapheight = 20;
const int size = mapwidth * mapheight;

// ����� ������ ��������
int colorWalls = 7;
int colorSnake = 10;
int colorFood = 4;

// ���������� ������� �����
int map[size];

// ������ ����
int headxpos;
int headypos;
int direction;

// ���������� ��� ��� ������ ���� ����(���� �������� �� ���� � �� ������ �����)
int food = 3;

// ����������� ���� �� ����
bool running;

// �� ���� ��� ���� ���� ������� �� (���� ��������� ���������) 
int main()
{
	void* handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(handle, &structCursorInfo);
	structCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(handle, &structCursorInfo);
	int choise;
	setlocale(LC_ALL, "Russian");
	std::cout << "\t\t\t\t����� ���������� � ������� ����\n\t\t[1]������ ����\n\t\t[2]���������\n\t\t[3]�����\n\t\t[*]������� ������� ������: ";
	std::cin >> choise;
	switch (choise)
	{
		case 1: 
			system("CLS");
			run();
			break;
		case 2: 
			settings();
			break;
	default:
		return 0;
		break;
	}
}

void setColor(char symbol, int color) {
	HANDLE _color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(_color, color);
	std::cout << symbol << " ";
	SetConsoleTextAttribute(_color, 7);
}

void settings() {
	bool isStop = false;
	while (!isStop) {
		system("CLS");
		int choise;
		std::cout << "\t\t\t\t����� ���������� � ������� ����\n\t\t";
		std::cout << "[1]���� ������ (" << colorWalls << ") ";
		setColor('�',colorWalls); setColor('�', colorWalls); setColor('�', colorWalls); setColor('�', colorWalls);
		std::cout << "\n\t\t[2]���� ��� (" << colorFood << ") ";
		setColor('�', colorFood); setColor('�', colorFood); setColor('�', colorFood); setColor('�', colorFood);
		std::cout << "\n\t\t[3]���� ������ (" << colorSnake << ") ";
		setColor('�', colorSnake); setColor('�', colorSnake); setColor('�', colorSnake); setColor('�', colorSnake);
		std::cout << "\n\t\t[4]�����\n\t\t[*]������� ������� ������: ";
		std::cin >> choise;
		switch (choise) {
		case 1:
			std::cout << "\t\t[*]�������� ����� ���� (����� �����):";
			std::cin >> choise;
			colorWalls = choise;
			break;
		case 2:
			std::cout << "\t\t[*]�������� ����� ���� (����� �����):";
			std::cin >> choise;
			colorFood = choise;
			break;
		case 3:
			std::cout << "\t\t[*]�������� ����� ���� (����� �����):";
			std::cin >> choise;
			colorSnake = choise;
			break;
		default:
			isStop = true;
			system("CLS");
			main();
			break;
		}
	}
}

void end() {
	system("CLS");
	// ��������� � ���������� ����(����� ������� � ��������� �����!)
	std::cout << "\t\t!!!Game over!" << std::endl << "\t\tYour score is: " << food - 3;
	for (int x = 0; x < mapwidth; ++x) {
		for (int y = 0; y < mapheight; ++y) {
			if (map[x + y * mapwidth] != -1) {
				map[x + y * mapwidth] = 0;
			}
		}
	}
	food = 3;
	_sleep(2000);
	system("CLS");
	main();
	// �� ���� ��������� ������� ����� ��������� ���� 
	//std::cin.ignore();
}

// �������� ������� �����
void run()
{
	// ������������� �����
	initMap();
	running = true;
	while (running) {
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });
		// ���� ������ ������
		if (kbhit()) {
			changeDirection(getch());
		}
		//���������� �����
		update();
		//�������� ��� �������� ������ ���������� 
		//clearScreen();
		//����� ����� �� �����
		printMap();
		//������ �� 0.04 ������� ��� �� ���� ���������
		_sleep(40/(food*0.2));
	}
	end();
}

// ���������� ������� ����(���� ��� �������� ������ �� ���������� ���������)
void changeDirection(char key) {
	switch (key) {
	case 'w':
		if (direction != 2) direction = 0;
		break;
	case 'd':
		if (direction != 3) direction = 1;
		break;
	case 's':
		if (direction != 4) direction = 2;
		break;
	case 'a':
		if (direction != 5) direction = 3;
		break;
	}
}

// ���������� �������� ������
void move(int dx, int dy) {
	// ���������� ����� ��������� ������
	int newx = headxpos + dx;
	int newy = headypos + dy;

	// ������ ���� �� ���
	if (map[newx + newy * mapwidth] == -2) {
		food++;
		generateFood();
	}
	else if (map[newx + newy * mapwidth] != 0) {
		running = false;
	}

	// �������� ������ �� ����� �������
	headxpos = newx;
	headypos = newy;
	map[headxpos + headypos * mapwidth] = food + 1;
}

// ����� �������� ������(������? � ������ ��� �����)))0) ) 
void clearScreen() {
	system("cls");
}

// ��������� ���
void generateFood() {
	int x = 0;
	int y = 0;
	do {
		x = rand() % (mapwidth - 2) + 1;
		y = rand() % (mapheight - 2) + 1;

		//���� ������ ������, ��������� �� ���������
	} while (map[x + y * mapwidth] != 0);
	//���������� ���
	map[x + y * mapwidth] = -2;
}

// ���������� �����
void update() {
	switch (direction) {
	case 0: move(-1, 0);
		break;
	case 1: move(0, 1);
		break;
	case 2: move(1, 0);
		break;
	case 3: move(0, -1);
		break;
	}

	for (int i = 0; i < size; i++) {
		if (map[i] > 0) map[i]--;
	}
}

// ������������� �����
void initMap()
{
	// ���������� ������ ���� � ������
	headxpos = mapwidth / 2;
	headypos = mapheight / 2;
	map[headxpos + headypos * mapwidth] = 1;

	// ������� � ������ ������
	for (int x = 0; x < mapwidth; ++x) {
		map[x] = -1;
		map[x + (mapheight - 1) * mapwidth] = -1;
	}

	// ������ � ����� ������
	for (int y = 0; y < mapheight; y++) {
		map[0 + y * mapwidth] = -1;
		map[(mapwidth - 1) + y * mapwidth] = -1;
	}
	// ���������� ������� ������
	generateFood();
}

// ��������� �����
void printMap()
{
	for (int x = 0; x < mapwidth; ++x) {
		for (int y = 0; y < mapheight; ++y) {
			char symbol = getMapValue(map[x + y * mapwidth]);
			switch (symbol) {
			case 'X':
				setColor(symbol, colorWalls);
				break;
			case 'O':
				setColor(symbol, colorFood);
				break;
			case 'o':
				setColor(symbol, colorSnake);
				break;
			default:
				std::cout << symbol << " ";
				break;
			}
		}
		std::cout << std::endl;
	}
}

// ���������� ������ ��� ����������� �� �������� �����
char getMapValue(int value)
{
	if (value > 0) return 'o'; // ������ ����

	switch (value) {

	case -1: 
		return 'X'; // ������ �����
	case -2: 
		return 'O'; // ������ ������
	}
	return ' ';
}