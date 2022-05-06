#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

std::string EncryptorCeasar(const std::string& str, int shift)
{
	std::string result;
	for (size_t i = 0; i < str.size(); ++i)
	{
		result += str[i] + shift;
	}
	return result;
}

std::string DecoderCeasar(const std::string& str, int shift)
{
	std::string result;
	for (size_t i = 0; i < str.size(); ++i)
	{
		result += str[i] - shift;
	}
	return result;
}

std::string EncryptorColumn(const std::string& str, int column)
{
	std::string result;
	size_t size_temp_str = 0;
	str.size() % column == 0 ? size_temp_str = str.size() / column : size_temp_str = str.size() / column + 1;
	
	for (size_t i = 0; i < size_temp_str; ++i)
	{
		for (size_t j = 0; j < column; ++j)
		{
			if (i + size_temp_str * j < str.size())
			{
				result += str[i + size_temp_str * j];
			}
			else
			{
				result += '*';
			}			
		}		
	}	
	return result;
}

std::string DecoderColumn(const std::string& str, int column)
{
	std::string result;
	size_t size_temp_str = str.size() / column;
	for (size_t i = 0; i < column; ++i)
	{
		for (size_t j = 0; j < size_temp_str; ++j)
		{
			if (str[i + column * j]== '*')
			{
				continue;
			}
			else
			{
				result += str[i + column * j];
			}
		}
	}
	return result;
}
std::string EncryptorBlock(std::string str, int column)
{
	std::vector<std::string> columns_words;
	while (str.size())
	{
		std::string temp;
		for (size_t i = 0; i < column; ++i)
		{
			if (str[i] != '\0')
			{
				temp += str[i];
			}
			
		}
		columns_words.push_back(EncryptorCeasar(temp,column));
		str.erase(0, column);
	}
	for (size_t i = 0; i < columns_words.size() / 2; ++i)
	{
		columns_words[i].swap(columns_words[columns_words.size() - i - 1]);
	}
	std::string result;
	result.reserve(columns_words.size() * column);
	for (std::string word : columns_words)
	{
		result +=  std::move(word);
	}
	return result;
}

std::string DecoderBlock(std::string str, int column)
{
	std::vector<std::string> columns_words;
	while (str.size())
	{
		std::string temp;
		if (str.size() >= column)
		{
			for (size_t i = 0; i < column; ++i)
			{
				temp += str[str.size() - 1 - i];
			}
			std::string temp1 = DecoderCeasar(temp, column);
			columns_words.push_back({ temp1.rbegin(), temp1.rend() });
			str.erase(str.size() - column);
		}
		else
		{
			columns_words.push_back(DecoderCeasar(str, column));
			str.clear();
		}		
	}	
	std::string result;
	result.reserve(columns_words.size() * column);
	for (std::string word : columns_words)
	{
		result += std::move(word);
	}
	return result;
}

void TestEncrption()
{
	{
		std::cout << "Test encryption of Ceasar" << std::endl;
		std::string str = "Hello world";
		int shift = 4;
		std::string encryption_str = EncryptorCeasar(str, shift);
		assert(str == DecoderCeasar(encryption_str, shift));
		std::cout << "Test encryption of Ceasar is good" << std::endl;
	}
	{
		std::cout << "Test encryption of column" << std::endl;
		std::string str = "This is asecret message";
		int column = 4;
		std::string encryption_str = EncryptorColumn(str, column);
		assert(encryption_str == "Tsrsh esiatass g emeice*");
		assert(str == DecoderColumn(encryption_str, column));
		std::cout << "Test encryption of column is good" << std::endl;
	}
	{
		std::cout << "Test encryption of block" << std::endl;
		std::string str = "Hello world";
		int column = 3;
		std::string encryption_str = EncryptorBlock(str, column);
		assert(str == DecoderBlock(encryption_str, column));
		std::cout << "Test encryption of block is good" << std::endl;
	}	
	std::cout << "Test encription is Ok" << std::endl;
}


int main()
{	
	TestEncrption();	
	
	return 0;
}