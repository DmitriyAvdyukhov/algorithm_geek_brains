#include <iostream>
#include <cassert>

using namespace std;


bool IsNotural(int a)
{
	for (size_t i = 1; i < a; i++)
	{
		if (a % i == 0 && a != i && i != 1)	
		{
			return false;
		}		
	}
	return true;
}

int main()
{
	assert(false == IsNotural(6));
	assert(true == IsNotural(5));
	int a = 0;
	cout << "Enter your number" << endl;
	cin >> a;
	if(IsNotural(a))
	{
		cout << "Your number is notural" << endl;
	}
	else 
	{
		cout << "Your nimbet is ot notural" << endl;
	}	

	return 0;
}