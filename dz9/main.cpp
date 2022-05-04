#include <cstdlib>
#include <vector>
#include <iostream>
#include <cassert>
#include <sstream>
#include <queue>


template <typename Type>
class Stack
{
public:   

    struct Node
    {
        Node() = default;

        Node(const Type& val)
            : value(val)
        {}

        Type value = Type();
        Node* next = nullptr;
        Node* prev = nullptr;
    };

public:

    Stack() = default;

    ~Stack()
    {
        if (size_)
        {
            size_t count = size_;
            for (size_t i = 0; i < count; ++i)
            {
                Pop();
            }
        }
    }

    void Push(const Type& value)
    {
        Node* node = new Node(value);
        if (size_ == 0)
        {
            head_ = std::move(node);
            ++size_;
        }
        else
        {
            Node* temp = std::move(head_);
            head_ = std::move(node);
            head_->next = std::move(temp);
            head_->next->prev = head_;
            ++size_;
        }

    }

    void Pop()
    {
        if (size_ > 0 && size_ != 1)
        {
            Node* temp = std::move(head_);
            head_ = nullptr;
            head_ = std::move(temp->next);
            head_->prev = nullptr;
            --size_;
            delete temp;
        }
        else
        {
            delete head_;
            head_ = nullptr;
            --size_;
        }
    }

    Type Top() const
    {
        return head_->value;
    }

    size_t Size() const
    {
        return size_;
    }

    bool Empty()
    {
        return !size_;
    }

    Node* GetNode() const
    {
        return head_;
    }

private:
    Node* head_ = nullptr;
    size_t size_ = 0;
};

class Graph
{
public:
    Graph() = default;

    Graph(size_t count_vertex) :vertices_(count_vertex)
    {
        adjacency_matrix_.resize(count_vertex);
        for (size_t i = 0; i < count_vertex; ++i)
        {
            adjacency_matrix_[i].resize(count_vertex);
        }

    }

    void AdEdge(int vertex_from, int vertex_to)
    {       
        adjacency_matrix_[vertex_from - 1][vertex_to - 1] = 1;
    }

    void PrintGraph() const
    {
        bool is_first = true;
        for (size_t i = 0; i < vertices_.size(); ++i)
        {            
            if (is_first)
            {
                std::cout << "  " << i << " ";
                is_first = false;
            }
            else
            {
                std::cout << i << " ";
            }
        }
        std::cout << std::endl;
        for (size_t i = 0; i < vertices_.size(); ++i)
        {
            std::cout << i << " ";
            for (size_t j = 0; j < vertices_.size(); ++j)
            {
                std::cout << adjacency_matrix_[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }    

    void DepthFirstSearch(std::ostream& out, int st)
    {
        DepthSearch(out, st);
        Reset();
        out << std::endl;
    }   
    
    void BreadthFirstSearch(std::ostream& out, int st)
    {
        BreadthSearch(out, st);
        Reset();
        out << std::endl;
    }

    void DepthFirstSearchByStack(std::ostream& out, int st_vertex)
    {
        SearchByStack(out, st_vertex);
        Reset();
        out << std::endl;
    }

    std::vector<int> CountReferenceVertexByAdjacency() const
    {
        std::vector<int> result(vertices_.size());
        for (size_t i = 0; i < vertices_.size(); ++i)
        {
            for (size_t j = 0; j < vertices_.size(); ++j)
            {
                if (adjacency_matrix_[i][j] == 1 && i != j)
                {
                    ++result[i];
                }
            }
        }        
        return result;
    }

    std::vector<int> CountReferenceVertexByBreadthFirstSearch() 
    {
        std::vector<int> result(vertices_.size());
        vertices_[0] = true;
        std::queue<int> q; 
        q.push(0);
        while (!q.empty())
        {
            int vertex = q.front(); 
            q.pop();
            
            vertices_[vertex] = true;
            for (size_t i = 0; i < vertices_.size(); ++i)
            {                
                if (adjacency_matrix_[vertex][i])
                {  
                    if (i != vertex)
                    {
                        ++result[vertex];
                    }
                    if (!vertices_[i])
                    {
                        vertices_[i] = true;
                        q.push(i);
                    }
                }
            }
        }
        return result;
    }

private:
    std::vector<int> vertices_;
    std::vector<std::vector<int>> adjacency_matrix_;
    

    void SearchByStack(std::ostream& out, int st_vertex)
    {
        Stack<int> stack;
        out << st_vertex << " ";
        vertices_[st_vertex] = 1;
        stack.Push(st_vertex);
        while (stack.Size())
        {
            int vertex = stack.Top();
            int next_vertex = -1;
            for (size_t i = 0; i < vertices_.size(); ++i)
            {
                bool used = adjacency_matrix_[vertex][i];
                if (!vertices_[i] && used)
                {
                    next_vertex = i;
                    break;
                }
            }
            if (next_vertex == -1)
            {
                stack.Pop();
            }
            else
            {
                vertices_[next_vertex] = true;
                out << next_vertex << " ";
                stack.Push(next_vertex);
            }
        }        
    }

    void BreadthSearch(std::ostream& out, int st_vertex)
    {
        vertices_[st_vertex] = true;
        std::queue<int> q;
        q.push(st_vertex);
        out << st_vertex << " ";
        while (!q.empty())
        {
            int vertex = q.front();
            q.pop();
            for (size_t i = 0; i < vertices_.size(); ++i)
            {
                bool used = adjacency_matrix_[vertex][i];
                if (!vertices_[i] && used)
                {
                    vertices_[i] = true;
                    out << i << " ";
                    q.push(i);
                }
            }
        }       
    }

    void DepthSearch(std::ostream& out, int vertex)
    {
        out << vertex << " ";        
        vertices_[vertex] = 1;
        for (size_t i = 0; i < vertices_.size(); ++i)
        {
            if (adjacency_matrix_[vertex][i] == 1 && !vertices_[i])
            {
                DepthSearch(out, i);
            }
        }
    }

    void Reset()
    {
        for (auto& vertex : vertices_)
        {
            vertex = 0;
        }        
    }
};

void PrintCountVertexAdjacency(std::ostream& out, const Graph& graph)
{
    std::vector<int> v = graph.CountReferenceVertexByAdjacency();
    for (const auto& count : v)
    {
        out << count << " ";
    }
    out << std::endl;
}

void PrintCountVertexByBreadthFirstSearch(std::ostream& out,  Graph& graph)
{
    std::vector<int> v = graph.CountReferenceVertexByBreadthFirstSearch();
    for (const auto& count : v)
    {
        out << count << " ";
    }
    out << std::endl;
}

void TestGraph()
{
    Graph graph(6);
    graph.AdEdge(1, 2);
    graph.AdEdge(1, 3);
    graph.AdEdge(2, 4);
    graph.AdEdge(2, 5);
    graph.AdEdge(2, 6);
    graph.AdEdge(3, 6);
    graph.AdEdge(4, 1);
    graph.AdEdge(6, 5);    
    graph.PrintGraph();
    graph.DepthFirstSearch(std::cout, 0);
    graph.DepthFirstSearchByStack(std::cout, 0);
    
    {
        std::stringstream str;
        std::stringstream str1;
        graph.DepthFirstSearch(str, 0);
        graph.DepthFirstSearchByStack(str1, 0);
        assert(str.str() == str1.str());
    }
    {
        std::stringstream str;
        std::stringstream str1;
        graph.DepthFirstSearch(str, 1);
        graph.DepthFirstSearchByStack(str1, 1);
        assert(str.str() == str1.str());
    }
    {
        std::stringstream str;
        std::stringstream str1;
        graph.DepthFirstSearch(str, 3);
        graph.DepthFirstSearchByStack(str1, 3);
        assert(str.str() == str1.str());
    }   
    {
        std::stringstream str;
        std::stringstream str1;
        PrintCountVertexAdjacency(str, graph);
        PrintCountVertexByBreadthFirstSearch(str1, graph);
        assert(str.str() == str1.str());
        assert(str.str() == "2 3 1 1 0 1 \n" && str1.str() == "2 3 1 1 0 1 \n");
    }
    

    std::cout << "Ok";
}

int main()
{	
	TestGraph();
	
	return 0;
}