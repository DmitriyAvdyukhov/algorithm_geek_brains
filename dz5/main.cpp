#include <iostream>
#include <queue>


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


void FillArray(int* ar, int size, int p)
{
	for (int i = 0; i < size; ++i)
	{
		ar[i] = rand() % p;
	}
}

void QuickSort(int*ar, int first, int last)  
{
	int i = first;
	int j = last;

	int x = ar[(first + last) / 2];
	if (ar[first] >= x && ar[first] <= ar[last])
	{
		x = ar[first];
	}
	else if (ar[last] >= x && ar[first] >= ar[last])
	{
		x = ar[last];
	}
	do {

		while (ar[i] < x) i++;
		while (ar[j] > x) j--;

		if (i <= j)
		{
			Swap(&ar[i], &ar[j]);
			i++;
			j--;
		}
	} while (i <= j);

	if (i < last)
	{
		QuickSort(ar, i, last);
	}
	if(first < j)
	{
		QuickSort(ar, first, j);
	}
}

void BucketSort(int* ar, int size)
{
	queue<int> buckets[10];
	for (int digit = 1; digit <= 1e9; digit *= 10)
	{
		for (int i = 0; i < size; ++i)
		{
			if (ar[i] % 2 == 0)
			{
				buckets[(ar[i] / digit) % 10].push(ar[i]);
				ar[i] = -1;
			}
		}
		int idx = 0;
		for (queue<int>& bucket : buckets)
		{
			while (!bucket.empty())
			{
				while (ar[idx] != -1)
				{
					++idx;
				}
				ar[idx] = bucket.front();
				bucket.pop();				
			}
		}
	}
}


int main()
{
	
	int ar[100];
	FillArray(ar, 100, 30);
	PrintArray(ar, 100);
	QuickSort(ar, 0, 99);
	PrintArray(ar, 100);
	FillArray(ar, 100, 100);
	PrintArray(ar, 100);
	BucketSort(ar, 100);
	PrintArray(ar, 100);
	
	return 0;
}