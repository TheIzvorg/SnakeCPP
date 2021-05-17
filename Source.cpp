#include <iostream>;
#include <Windows.h>;
#include <conio.h>;
#include <ctime>;
#include <string>;

//в данном случае мы не используем пространство имен STD потому что все ломается к чертям и вывод идет через std::cout

#pragma warning(disable:4996)

// Перегрузки

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

// Настройки карты
const int mapwidth = 20;
const int mapheight = 20;
const int size = mapwidth * mapheight;

// Цвета разных символов
int colorWalls = 7;
int colorSnake = 10;
int colorFood = 4;

// Объявление размера карты
int map[size];

// Голова змеи
int headxpos;
int headypos;
int direction;

// Количество еды или длинна тела змеи(один параметр на очки и на длинну тушки)
int food = 3;

// Отслеживает идет ли игра
bool running;

// ну мейн как мейн чего бубнить то (сюда впихнется интерфейс) 
int main()
{
	void* handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(handle, &structCursorInfo);
	structCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(handle, &structCursorInfo);
	int choise;
	setlocale(LC_ALL, "Russian");
	std::cout << "\t\t\t\tДОБРО ПОЖАЛОВАТЬ В ГЛАВНОЕ МЕНЮ\n\t\t[1]Начало игры\n\t\t[2]Настройки\n\t\t[3]Выход\n\t\t[*]Введите вариант ответа: ";
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
		std::cout << "\t\t\t\tДОБРО ПОЖАЛОВАТЬ В ГЛАВНОЕ МЕНЮ\n\t\t";
		std::cout << "[1]Цвет стенки (" << colorWalls << ") ";
		setColor('Ц',colorWalls); setColor('В', colorWalls); setColor('Е', colorWalls); setColor('Т', colorWalls);
		std::cout << "\n\t\t[2]Цвет еды (" << colorFood << ") ";
		setColor('Ц', colorFood); setColor('В', colorFood); setColor('Е', colorFood); setColor('Т', colorFood);
		std::cout << "\n\t\t[3]Цвет змейки (" << colorSnake << ") ";
		setColor('Ц', colorSnake); setColor('В', colorSnake); setColor('Е', colorSnake); setColor('Т', colorSnake);
		std::cout << "\n\t\t[4]Выход\n\t\t[*]Введите вариант ответа: ";
		std::cin >> choise;
		switch (choise) {
		case 1:
			std::cout << "\t\t[*]Выберите новый цвет (любое число):";
			std::cin >> choise;
			colorWalls = choise;
			break;
		case 2:
			std::cout << "\t\t[*]Выберите новый цвет (любое число):";
			std::cin >> choise;
			colorFood = choise;
			break;
		case 3:
			std::cout << "\t\t[*]Выберите новый цвет (любое число):";
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
	// Сообщение о завершении игры(Позже вынести в отдельный метод!)
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
	// Не дает закрыться консоли после окончания игры 
	//std::cin.ignore();
}

// Основные игровые фишки
void run()
{
	// Инициализация карты
	initMap();
	running = true;
	while (running) {
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });
		// Если кнопка нажата
		if (kbhit()) {
			changeDirection(getch());
		}
		//Обновление карты
		update();
		//Чистилка как чистилка ничего особенного 
		//clearScreen();
		//Вывод карты на экран
		printMap();
		//Таймер на 0.04 секунды что бы была плавность
		_sleep(40/(food*0.2));
	}
	end();
}

// Управление головой змеи(Пока что работает только на английской раскладке)
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

// Реализация движения головы
void move(int dx, int dy) {
	// определяет новое положение головы
	int newx = headxpos + dx;
	int newy = headypos + dy;

	// Думает есть ли еда
	if (map[newx + newy * mapwidth] == -2) {
		food++;
		generateFood();
	}
	else if (map[newx + newy * mapwidth] != 0) {
		running = false;
	}

	// Движение головы на новую позицию
	headxpos = newx;
	headypos = newy;
	map[headxpos + headypos * mapwidth] = food + 1;
}

// Метод отчистки экрана(Нафига? а потому что можем)))0) ) 
void clearScreen() {
	system("cls");
}

// Генирация еды
void generateFood() {
	int x = 0;
	int y = 0;
	do {
		x = rand() % (mapwidth - 2) + 1;
		y = rand() % (mapheight - 2) + 1;

		//Если клетка занята, повторить до победного
	} while (map[x + y * mapwidth] != 0);
	//Размещение еды
	map[x + y * mapwidth] = -2;
}

// Обновление карты
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

// Инициализация карты
void initMap()
{
	// Размещение головы змеи в центре
	headxpos = mapwidth / 2;
	headypos = mapheight / 2;
	map[headxpos + headypos * mapwidth] = 1;

	// Верхняя и нижняя стенки
	for (int x = 0; x < mapwidth; ++x) {
		map[x] = -1;
		map[x + (mapheight - 1) * mapwidth] = -1;
	}

	// Правая и левая стенки
	for (int y = 0; y < mapheight; y++) {
		map[0 + y * mapwidth] = -1;
		map[(mapwidth - 1) + y * mapwidth] = -1;
	}
	// Размещение первого фрукта
	generateFood();
}

// Отрисовка карты
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

// Возвращает символ для отображения из значения карты
char getMapValue(int value)
{
	if (value > 0) return 'o'; // Символ Змеи

	switch (value) {

	case -1: 
		return 'X'; // Символ Стены
	case -2: 
		return 'O'; // Символ Фрукта
	}
	return ' ';
}