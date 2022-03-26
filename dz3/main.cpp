#include <iostream>
#include <cassert>
#include <sstream>

#define HEIGHT 8
#define WIDTH 8
#define QUEENS 8
#define KNIGHTS HEIGHT * WIDTH - 1

using namespace std;

void DecToBin(int x, ostream& out)
{
	if (x > 0)
	{
		DecToBin(x / 2, out);
	}
	out << x % 2;
}

int Exponentiation(int number, int degree)
{
	if (degree == 0)
	{
		return 1;
	}
	return number * Exponentiation(number, --degree);
}

int Exponentiation1(int number, int degree)
{
	if (degree == 0)
	{
		return 1;
	}
	if (degree % 2 == 0)
	{
		degree /= 2;
		return number * number * Exponentiation(number, degree);
	}
	else
	{
		return number * Exponentiation(number, --degree);
	}
	
}
namespace king
{
	struct Board
	{
		int board[HEIGHT][WIDTH];
	};

	void Annul(Board& board)
	{
		for (int i = 0; i < HEIGHT; i++)
		{
			for (int j = 0; j < WIDTH; j++)
			{
				board.board[i][j] = 0;
			}
		}
	}

	void SetFigure(Board& board)
	{
		int count_figure;
		std::cout << "How many figures do you want install? Pleas enter count from 1 to 16" << std::endl;
		std::cin >> count_figure;
		for (int i = 1; i <= count_figure; i++)
		{
			int x, y;
			std::cout << "Enter coordinates your figure number: "
				<< i << " x and y from 0 to 8" << std::endl;
			std::cin >> x >> y;
			board.board[x][y] = -1; //занятую клетку будем помечать -1
		}
	}

	int Routes(const Board& board, int x, int y)
	{
		if (x == 0 && y == 0)
		{
			return 0;
		}
		else if ((x == 0 && y != 0) || (y == 0 && x != 0))
		{
			return 1;
		}
		else if (board.board[x - 1][y] != 0)
		{
			return Routes(board, x, y - 1);
		}
		else if (board.board[x][y - 1] != 0)
		{
			return Routes(board, x - 1, y);
		}
		else
		{
			return Routes(board, x, y - 1) + Routes(board, x - 1, y);
		}
	}

	void PrintKingMoves(const Board& board)
	{
		std::cout << "King can move to the lower rigth corner by making the numbers of moves" << std::endl;
		for (int i = 0; i < HEIGHT; i++)
		{
			for (int j = 0; j < WIDTH; j++)
			{
				if (board.board[i][j] == 0)
				{
					std::cout << Routes(board, i, j) << " ";
				}
				else
				{
					std::cout << board.board[i][j] << " ";
				}
			}
			std::cout << std::endl;
		}
	}
	
	void KingMove()
	{
		Board board;
		Annul(board);
		SetFigure(board);
		PrintKingMoves(board);
	}
}

int main()
{		
	//   задание 1
	{
		{
			stringstream st;
			DecToBin(5, st);
			assert("0101"s == st.str());
		}
		{
			stringstream st;
			DecToBin(10, st);
			assert("01010"s == st.str());
		}
		{
			stringstream st;
			DecToBin(0, st);
			assert("0"s == st.str());
		}
		int x = 0;
		cout << "\tConverting a number from decimal to binary\nEnter your number" << endl;
		cin >> x;
		cout << "Your number in dec: " << x << " your number in bin: ";
		DecToBin(x, cout);
		cout << endl;
	}
	// задание 2.1
	{
		assert(8 == Exponentiation(2, 3));
		assert(3125 == Exponentiation(5, 5));
		int number, degree;
		cout << "\tExponentiation number in degree\n\tFirst way\nEnter your number and degree" << endl;
		cin >> number >> degree;
		cout << "Exponentiation number " << number << " in degree " << degree << " equal " << Exponentiation(number, degree) << endl;
		std::cout << std::endl;
	}
	//задание 2.2
	{
		assert(8 == Exponentiation1(2, 3));
		assert(3125 == Exponentiation1(5, 5));
		int number, degree;
		cout << "\tSecond way\nEnter your number and degree" << endl;
		cin >> number >> degree;
		cout << "Exponentiation number " << number << " in degree " << degree << " equal " << Exponentiation(number, degree) << endl;
		std::cout << std::endl;
	}
	//задание 3
	{
		king::KingMove();
	}
	return 0;
}