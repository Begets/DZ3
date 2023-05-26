#include <iostream>
#include <ctime>
using namespace std;

int map[9][9]; //Размер поля для игрока
int field[9][9]; //Размер невидимого поля

const int Row = 9, Col = 9, Bomb = 10;  //Количество строк, столбцов и мин

bool is_win()
{
	int is_opened = 0; //Открыть клетку
	for (int i = 0; i < Row; i++)
	{
		for (int j = 0; j < Col; j++)
		{
			if (map[i][j] >= 0)
			{
				is_opened++;  //Проверить каждую клетку и инкрементировать, при условии что она открыта
			}
		}
	}
	return (is_opened == (Col * Row - Bomb)); //Дать положительное значение, если все клетки открыты
}

void print_field()
{
	system("cls"); //Опустошить поле
	cout << " ";
	for (int i = 0; i < Row; i++)
	{
		cout << " " << i + 1; //Вывести номер столбца
	}
	cout << endl;

	for (int i = 0; i < Row; i++)
	{
		cout << i + 1 << " "; //Вывести номер строки
		for (int j = 0; j < Col; j++)
		{

			if (map[i][j] == -2)
			{
				cout << "_ "; //-2 - Закрытая клетка
			}

			else if (map[i][j] == -1)
			{
				cout << "f "; //-1 - Флажок
			}

			else if (map[i][j] == -3)
			{
				cout << "* "; //-3 - Мина
			}

			else
			{
				cout << map[i][j] << " ";
			}
		}
		cout << endl;
	}
}

void end_game(bool is_win = false)
{
	for (int i = 0; i < Row; i++)
	{
		for (int j = 0; j < Col; j++)
		{
			if (field[i][j] == -1)
			{
				map[i][j] = -3; //Показать все мины

			}
			else if (map[i][j] == -1)
			{
				map[i][j] = -2; //Открыть мины, закрытые флажком
			}
		}
	}

	print_field();
	if (is_win)
	{
		cout << "You won! Write something to continue" << endl;;
	}
	else
	{
		cout << "You lost! Write something to continue" << endl;
	}
	string a;
	cin >> a;
}

void zero_field(int x, int y)
{
	if (x < 0 || x >= Row || y < 0 || y >= Col || map[x][y] > -2)
	{
		return;
	}
	map[x][y] = field[x][y];

	if (map[x][y] > 0)
	{
		return;
	}

	for (int i2 = x - 1; i2 <= x + 1; i2++)
	{
		for (int j2 = y - 1; j2 <= y + 1; j2++)
		{
			if (i2 != x || j2 != y)
			{
				zero_field(i2, j2); //Проверить все ближайшие клетки, и продолжать, если клетка равна нулю
			}
		}
	}
}

bool open_cell(int x, int y) //Проверка, является ли клетка открытой
{

	if (field[x][y] == -1)
	{
		return 0; //Клетка закрыта
	}
	if (field[x][y] > 0)
	{
		map[x][y] = field[x][y]; //Клетка открыта
		return 1;
	}
	zero_field(x, y);
}



int main()
{
	while (true)
	{
		srand(time(nullptr)); //Запускает ГПСЧ
		for (int i = 0; i < Row; i++)
		{
			for (int j = 0; j < Col; j++)
			{
				field[i][j] = 0;
				map[i][j] = -2; //Заполнить все игровое поле закрытыми клетками
			}

		}
		for (int i = 0; i < Bomb; )
		{
			int x = rand() % Row;  //Мины в случайном порядке расходятся по полю
			int y = rand() % Col;
			if (field[x][y] != -1)
			{
				field[x][y] = -1;
				i++;
			}
		}

		for (int i = 0; i < Row; i++)
		{
			for (int j = 0; j < Col; j++)
			{
				if (field[i][j] != -1)
				{
					for (int i2 = i - 1; i2 <= i + 1; i2++)
					{
						for (int j2 = j - 1; j2 <= j + 1; j2++)
						{
							if (field[i2][j2] == -1 && i2 >= 0 && i2 < Row && j2 >= 0 && j2 < Col)
							{
								field[i][j]++;  //Присвоить значение клетки, в зависимости от количества ближайших мин
							}
						}
					}
				}
			}
		}

		while (true)
		{


			print_field();

			string command;
			cout << "Enter command: \\o (open cell)  \\f (set flag)  \\n  (new game)  \\q (quit game): ";
			cin >> command;
			if (command == "\\o")
			{
				int x, y;
				cin >> x >> y;
				x--;
				y--;

				if (!open_cell(x, y))
				{
					end_game(); //Закончить игру, если игрок проиграл
					break;
				}
				else
				{
					print_field();
				}

				if (is_win())
				{
					end_game(true); //Победа, если все клетки без мин заполнены. Закончить игру
					break;
				}
			}
			else if (command == "\\f")
			{
				int x, y;
				cin >> x >> y;
				x--;
				y--;
				if (x < 0 || y < 0 || x >= Row || y >= Col || map[x][y] >= 0)
				{
					continue;
				}
				else
				{
					map[x][y] = -1; //Поставить флажок
				}
			}
			else if (command == "\\n")
			{
				break; //Начать новую игру
			}
			else if (command == "\\q")
			{
				return 0; //Закончить игру
			}
		}
	}
}