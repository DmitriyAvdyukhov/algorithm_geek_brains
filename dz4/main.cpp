#include <iostream>

#define SIZE 4
using namespace std;

template<class T>
void Swap(T* a, T* b)
{
	T t;
	t = *a;
	*a = *b;
	*b = t;
}

template<class T>
void PrintArray(T* ar, int size)
{
	for (size_t i = 0; i < size; ++i)
	{
		cout << ar[i] << " ";
	}
	cout << endl;
}

void PrintArray(int(*ar)[SIZE])
{
	for (int i = 0; i < SIZE; ++i)
	{			
		PrintArray(ar[i], SIZE);
	}
}

void Sort(int(*ar)[SIZE])
{
	int* t;
	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
			t = &ar[i][j];
			for (int c = 0; c < SIZE; ++c)
			{
				for (int r = 0; r < SIZE; ++r)
				{
					if (*t < ar[c][r])
					{
						Swap(t, &ar[c][r]);
					}
				}
			}
		}
	}
}

void Sort1(int(*ar)[SIZE])
{
	for (size_t i = 0; i < SIZE * SIZE; ++i)
	{
		int col = 0;
		int row = 0;
		
		for (size_t j = 0; j < SIZE * SIZE - 1; ++j)
		{		
			int col_next = col;
			int row_next = row + 1;
			if (row_next > SIZE - 1)
			{
				col_next = col + 1;
				row_next = 0;
			}

			if (ar[col][row] > ar[col_next][row_next])
			{
				Swap(&ar[col][row], &ar[col_next][row_next]);
			}
			++row;
			if (row > SIZE - 1)
			{
				++col;
				row = 0;
			}
		}
	}
}

void FillArray(int(*ar)[SIZE], int p)
{
	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
			ar[i][j] = rand() % p;
		}
	}
}

template<class T>
void SwapArray(T ar[], int size)
{
	for (size_t i = 0; i < size / 2; i++)
	{
		Swap(&ar[i], &ar[11 - 1 - i]);
	}
}

template<class T>
void FillArray(T* ar, int size)
{
	T number = 0;
	for (size_t i = 0; i < size; ++i)
	{
		cin >> number;
		ar[i] = number;
	}
}

template<class T>
void Proces(T* ar, int size)
{
	for (size_t i = 0; i < size; i++)
	{
		auto result = sqrt(fabs(ar[i])) + 5 * pow(ar[i], 3);
		if (result > 400)
		{
			cout << "Your result is more then 400 " << endl;
		}
		else
		{
			cout << result << endl;
		}
	}
}

void TrabbaPrado()
{
	double ar[11];
	FillArray(ar, 11);
	PrintArray(ar, 11);
	SwapArray(ar, 11);
	PrintArray(ar, 11);
	Proces(ar, 11);
}

int main()
{
	
	int ar[SIZE][SIZE];
	FillArray(ar, 17);
	PrintArray(ar);
	std::cout << std::endl;
	Sort(ar);
	PrintArray(ar);
	std::cout << std::endl;
	FillArray(ar, 17);
	PrintArray(ar);
	std::cout << std::endl;
	Sort1(ar);
	PrintArray(ar);
	TrabbaPrado();
	
	return 0;
}