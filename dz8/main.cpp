#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <memory>

template<class T>
struct Node
{
	Node() = default;
	Node(T value) :key(value)
	{}
	T key{};
	std::shared_ptr<Node<T>> left = nullptr;
	std::shared_ptr<Node<T>> rigth = nullptr;
};

template<class T>
class Tree
{
public:

	Tree() = default;

	Tree(int n, std::istream& in = std::cin)
	{
		head_ = std::move(CreatBalancedTree(n, in));
	}
	
	Tree(Tree<T>&& other)
	{
		size_ = std::move(other.size_);
		head_ = std::move(other.head_);
		other.head_ = nullptr;
	}
	~Tree() = default;

	Tree operator=(Tree<T> other)
	{
		head_ = other.head_;
		return *this;
	}

	std::shared_ptr<Node<T>> CreatBalancedTree(int n, std::istream& in = std::cin)
	{
		if (in)
		{
			std::shared_ptr<Node<T>> new_node = nullptr;
			T key;
			int nl;
			int nr;
			if (n == 0)
			{
				new_node = nullptr;
			}
			else
			{
				in >> key;
				nl = n / 2;
				nr = n - nl - 1;
				new_node = std::make_shared<Node<T>>(key);
				new_node->left = CreatBalancedTree(nl, in);
				new_node->rigth = CreatBalancedTree(nr, in);
			}
			if (new_node)
			{
				++size_;
			}
			return new_node;
		}
		return nullptr;
	}

	void Insert(T data)
	{
		std::shared_ptr<Node<T>> new_node = std::make_shared<Node<T>>(data);
		std::shared_ptr<Node<T>> current = head_;
		std::shared_ptr<Node<T>> parent = head_;
		if (!head_)
		{
			head_ = std::move(new_node);
			++size_;
		}
		else
		{
			while (current->key != data)
			{
				parent = current;
				if (current->key > data)
				{
					current = current->left;
					if (current == nullptr)
					{
						parent->left = std::move(new_node);
						++size_;
						return;
					}
				}
				else
				{
					current = current->rigth;
					if (current == nullptr)
					{
						parent->rigth = std::move(new_node);
						++size_;
						return;
					}
				}
			}
		}
	}
	

	void Print()
	{
		Print(head_);
		std::cout << std::endl;
	}
	
	std::shared_ptr<Node<T>> GetSuccerssor(std::shared_ptr<Node<T>> node)
	{
		std::shared_ptr<Node<T>> current = std::make_shared<Node<T>>(node->rigth_);
		std::shared_ptr<Node<T>> parent = std::make_shared<Node<T>>(node);
		std::shared_ptr<Node<T>> s = std::make_shared<Node<T>>(node);
		while (current != nullptr)
		{
			parent = s;
			s = current;
			current = current->left;
		}
		if (s != node->rigth)
		{
			parent->left = s->rigth;
			s->rigth = node->rigth;
		}
		return s;
	}

	bool Delete(const T& key)
	{
		std::shared_ptr<Node<T>> current = std::make_shared<Node<T>>(head_);
		std::shared_ptr<Node<T>> parent = std::make_shared<Node<T>>(head_);
		bool is_left_child = true;
		while (current->key != key)
		{
			parent = current;
			if (key<current->key)
			{
				current = current->left;
				is_left_child = true;
			}
			else
			{
				current = current->rigth;
				is_left_child = false;
			}
			if (current == nullptr)
			{
				return false;
			}
		}
		
		if (current->left == nullptr && current->rigth == nullptr)
		{
			if (current == head_)
			{
				head_ = nullptr;
			}
			else if (is_left_child)
			{
				parent->left = nullptr;
			}
			else
			{
				parent->rigth = nullptr;
			}
		}
		else if (current->rigth == nullptr)
		{
			if (is_left_child)
			{
				parent->left = current->left;
			}
			else
			{
				parent->rigth = current->left;
			}
		}
		else if (current->left == nullptr)
		{
			if (is_left_child)
			{
				parent->left = current->rigth;
			}
			else
			{
				parent->rigth = current->rigth;
			}
		}
		else
		{
			std::shared_ptr<Node<T>> successor = GetSuccerssor(current);
			if (current == head_)
			{
				head_ = std::move(successor);
			}
			else if (is_left_child)
			{
				parent->left = std::move(successor);
			}
			else
			{
				parent->rigth = std::move(successor);
			}
			successor->left = std::move(current->left);
		}
		--size_;
		return true;
	}

	void PreOrderTravers(std::ostream& out = std::cout)
	{
		if (out)
		{
			PreOrderTravers(head_, out);
			out << std::endl;
		}
	}

	void InOrderTravers(std::ostream& out = std::cout)
	{
		if(out)
		{
			InOrderTravers(head_, out);
			out << std::endl;
		}
	}

	void PostOrderTravers(std::ostream& out = std::cout)
	{
		if (out)
		{
			PostOrderTravers(head_, out);
			out << std::endl;
		}
	}

	bool Search(const T& value)
	{
		return Search(head_, value);
	}

	bool BinSearch(const T& value)
	{
		return BinSearch(head_, value);
	}

	bool IsBalanceTree()
	{
		int count_left = 0;
		int count_rigth = 0;
		std::shared_ptr<Node<T>> current = head_->left;
		while (current != nullptr)
		{	
			if (current->left != nullptr)
			{
				current =current->left;
				++count_left;
			}
			else
			{
				current = current->rigth;
				++count_left;
			}			
		}
		current = head_->rigth;
		while (current != nullptr)
		{
			if (current->left != nullptr)
			{
				current = current->left;
				++count_rigth;
			}
			else
			{
				current = current->rigth;
				++count_rigth;
			}
		}
		if (count_left == count_rigth
			|| count_left - 1 == count_rigth
			|| count_left == count_rigth -1)
		{
			return true;
		}
		return false;
	}

	size_t GetSize()
	{
		return size_;
	}

private:

	bool BinSearch(std::shared_ptr<Node<T>> root, const T& value)
	{
		if (root == nullptr)
		{
			return false;
		}
		if (root->key == value)
		{
			return true;
		}
		std::shared_ptr<Node<T>> current = root;
		while (current->key != value)
		{
			if (value < current->key)
			{
				current = current->left;
			}
			else
			{
				current = current->rigth;
			}
			if (current == nullptr)
			{
				return false;
			}
			if (current->key == value)
			{
				return true;
			}
		}
		return false;
	}

	bool Search(std::shared_ptr<Node<T>> root, const T& value)
	{
		if (root == nullptr)
		{
			return false;
		}
		if (root->key == value)
		{
			return true;
		}
		else if(Search(root->left, value))
		{
			return true;
			
		}
		else if (Search(root->rigth, value))
		{
			return true;
		}
		return false;
	}

	void PreOrderTravers(std::shared_ptr<Node<T>> node, std::ostream& out)
	{
		if (node)
		{
			out << node->key << " ";
			PreOrderTravers(node->left, out);
			PreOrderTravers(node->rigth, out);
		}
	}
	void InOrderTravers(std::shared_ptr<Node<T>> node, std::ostream& out)
	{
		if (node)
		{
			InOrderTravers(node->left, out);
			out << node->key << " ";
			InOrderTravers(node->rigth, out);
		}
	}
	void PostOrderTravers(std::shared_ptr<Node<T>> node, std::ostream& out)
	{
		if (node)
		{
			PostOrderTravers(node->left, out);			
			PostOrderTravers(node->rigth, out);
			out << node->key << " ";
		}
	}
	
	void Print(std::shared_ptr<Node<T>> root)
	{
		if (root)
		{
			std::cout << root->key;
			{
				if (root->left || root->rigth)
				{
					std::cout << '(';
					if (root->left)
					{
						Print(root->left);
					}
					else
					{
						std::cout << "NULL";
					}
					std::cout << ", ";
					if (root->rigth)
					{
						Print(root->rigth);
					}
					else
					{
						std::cout << "NULL";
					}
					std::cout << ")";
				}
			}
		}		
	}
	
	std::shared_ptr<Node<T>> head_ = nullptr;
	size_t size_ = 0;
};

int GetRandomNumber(int min, int max)
{
	
	int num = min + rand() % ((max + 1) - min);

	return num;
}


void TestTrees()
{
	const int SIZEARR = 50;
	const int MAXCOUNT = 10000;
	std::vector<Tree<int>> result;
	

	for (int i = 0; i < SIZEARR; ++i)
	{		
		Tree<int> temp;
		for (int j = 0; j < MAXCOUNT; ++j)
		{
			int noumber = GetRandomNumber(0, MAXCOUNT);
			temp.Insert(noumber);
			
		}
		
		result.push_back(std::move(temp));
	}
	
	int balance = 0;
	for (int i = 0; i < SIZEARR; ++i)
	{
		if (result[i].IsBalanceTree())
		{
			++balance;
		}
	}
	std::cout << "The percentage of balanced trees is: " << balance * 100 / SIZEARR << " %" << std::endl;

	

	Tree<int> tree;
	tree.Insert(5);
	tree.Insert(6);
	tree.Insert(4);
	tree.Insert(3);
	tree.Insert(8);
	tree.Print();	
	assert(tree.Search(8));
	assert(!tree.Search(10));
	std::cout << std::boolalpha << tree.Search(8) << std::endl;
	std::cout << std::boolalpha << tree.Search(10) << std::endl;
	std::cout << "Ok";
}

int main()
{	
	TestTrees();
	
	return 0;
}