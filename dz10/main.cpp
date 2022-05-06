#include <iostream>
#include <sstream>
#include <cassert>
#include <vector>

size_t SimpleHash(const std::string& str)
{
	size_t result = 1;
	for (size_t i = 0; i < str.length(); ++i)
	{
		result = result * 10 + str[i];
	}
	return result;
}


size_t NaiveHash(const std::string& str)
{
	size_t result = 1;
	for (size_t i = 0; i < str.length(); ++i)
	{
		result +=  str[i];
	}
	return result;
}

std::vector<int> Purse(const std::vector<int>& coins, int amount_money){
	
	std::vector<int> result;
	int sum = 0;
	int max_coin_position = 0;
	while (sum != amount_money)
	{
		if (coins[max_coin_position] <= amount_money - sum)
		{
			sum += coins[max_coin_position];
			result.push_back(coins[max_coin_position]);
		}
		else
		{
			++max_coin_position;
		}
	}
	return result;
}


void PrintPurse(std::ostream& out, const std::vector<int>& purse)
{
	for (const auto& coin : purse)
	{
		out << coin << " ";
	}
	out << std::endl;
}

void TestChash()
{
	{
		size_t chash_str = NaiveHash("qwe");
		size_t chash_str1 = NaiveHash("ewq");
		assert(chash_str == chash_str1);
		std::cout << chash_str << std::endl;
		std::cout << chash_str1 << std::endl;
	}
	{
		size_t chash_str = SimpleHash("qwe");
		size_t chash_str1 = SimpleHash("ewq");
		assert(chash_str != chash_str1);
		std::cout << chash_str << std::endl;
		std::cout << chash_str1 << std::endl;
	}	
}

void TestGreedyAlgorithm()
{
	const std::vector<int> coins = { 50, 10, 5, 2, 1 };
	std::stringstream purse1;
	PrintPurse(purse1, Purse(coins,98));
	assert(purse1.str() == "50 10 10 10 10 5 2 1 \n");
	std::stringstream purse2;
	PrintPurse(purse2, Purse(coins, 57));
	assert(purse2.str() == "50 5 2 \n");
}

int main()
{	
	TestChash();
	TestGreedyAlgorithm();
	
	return 0;
}