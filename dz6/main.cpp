#include <cassert>
#include <iostream>
# include <sstream>

using namespace std;

template <typename Type>
class QueueInterface
{
public:

    struct Value
    {
        Value() = default;

        Value(Type val, int priority = 0)
            : value(val)
            , priority(priority)
        {}

        Type value = 0;
        int priority = 0;
    };

    struct Node
    {
        Node() = default;

        Node(const Value& val)
            : value(val)           
        {}

        Value value = Type();
        Node* next = nullptr;
        Node* prev = nullptr;
    };

public:

    QueueInterface() = default;
    

    ~QueueInterface()
    {
        if (size_)
        {
            size_t count = size_;
            
            for (size_t i = 0; i < count; ++i)
            {
                Node* temp = std::move(head_);
                head_ = std::move(temp->next);
                delete temp;
            }
        }        
    }

    virtual void Push(const Type& value, int priority = 0)
    {
        Node* node = new Node({ value,priority });
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

    virtual void Pop()
    {
        if (size_ == 0)
        {
            throw std::logic_error("Queue is empty");
        }
        if (size_ > 0 && size_ != 1)
         {
             Node** temp = &head_;
             while ((*temp)->next != nullptr)
             {
                 temp = &(*temp)->next;
             }   
             --size_;
             (*temp)->prev->next = nullptr;
             delete (*temp);
        }
        else
        {
            delete head_;
             head_ = nullptr;
             --size_;
        }
    }

    void PushPriority(const Type& value, int priority = 0)
    {
        Node* node = new Node({ value,priority });
        if (size_ == 0)
        {
            head_ = std::move(node);
            ++size_;
        }
       
        else
        {
            Node** tmp = &head_;
            size_t count = size_;
            for (size_t i = 0; i < count; ++i)
            {
                if ((*tmp)->value.priority <= priority)
                {
                    if ((*tmp)->prev == nullptr)
                    {
                        Node* temp = std::move(head_);
                        head_ = std::move(node);
                        head_->next = std::move(temp);
                        head_->next->prev = head_;
                        ++size_;
                        break;
                    }
                    else
                    {   
                        Node* temp = std::move((*tmp));
                        (*tmp)->prev->next = std::move(node);
                        node->next = std::move(temp);
                        node -> prev = temp->prev;
                        node->next->prev = node;  
                        ++size_;
                        break;
                    }
                   
                }
                if ((*tmp)->next == nullptr)
                {                   
                    (*tmp)->next = std::move(node);
                    (*tmp)->next->prev = (*tmp)->next;
                    ++size_;
                }
                tmp = &(*tmp)->next;
            }
        }
    }

    void PopPriority()
    {
        if (head_ != nullptr)
        {
            Node** temp = &head_;
            Node* tmp = head_;
            bool first = true;
            size_t count = size_;
            while (count > 0)
            {
                if (tmp->value.priority < (*temp)->value.priority)
                {
                    tmp = (*temp);
                    first = false;
                }
                else if (tmp->value.priority == (*temp)->value.priority && first)
                {
                    tmp = (*temp);
                    first = false;
                }
                
                temp = &(*temp)->next;
                --count;
            }

            if (tmp->prev != nullptr && tmp->next != nullptr)
            {
                tmp->prev->next = tmp->next;
                tmp->next->prev = tmp->prev;
                delete tmp;
                --size_;
            }
            else if (size_ > 1 && tmp->next != nullptr)
            {
                tmp->next->prev = nullptr;
                head_ = tmp->next;                
                delete tmp;
                --size_;
            }
            else if (size_ == 1)
            {
                delete head_;
                head_ = nullptr;
                --size_;
            }
            else if(tmp->next == nullptr)
            {
                tmp->prev->next = nullptr;
                delete tmp;
                --size_;
            }

        }
    }    

    void Print() const
    {
        Print(head_);
    }    

    size_t Size() const
    {
        return size_;
    }

    bool Empty() const
    {
        return size_ == 0;
    }

    Type& Front() const
    {
        return head_->value.value;
    }

    Type& Back() 
    {
        Node** temp = &head_;
        while ((*temp)->next != nullptr)
        {
            temp = &(*temp)->next;
        }
        return (*temp)->value.value;
    }

private:
    void Print(Node* node) const
    {
        if (node != nullptr)
        {
            if (node->next == nullptr && size_ > 0)
            {
                std::cout << "Prioritet: "s << node->value.priority << " Value: "s << node->value.value << std::endl;
                return;
            }
            else
            {
                std::cout << "Prioritet: "s << node->value.priority << " Value: "s << node->value.value << std::endl;
                Print(node->next);
            }
        }
        else
        {
            std::cout << "Queue is empty"s;
        }
    }

    Node* head_ = nullptr;
    size_t size_ = 0;
};

template<class Type>
class Queue : public QueueInterface<Type>
{    
    void PopPriority()  = delete;

    void PushPriority() = delete;
};

template<class Type>
class QueueWithPriorityPush :public QueueInterface<Type>
{
public:
    void PopPriority() = delete;

    void PushPriority() = delete;

    void Push(const Type& value, int priority = 0) override
    {
        return QueueInterface<Type>::PushPriority(value, priority);
    }

    void Pop() override
    {
        return QueueInterface<Type>::Pop();
    }


};

template<class Type>
class QueueWithPriorityPop :public QueueInterface<Type>
{
public:
    void PopPriority() = delete;

    void PushPriority() = delete;

    void Push(const Type& value, int priority = 0) override
    {
        return QueueInterface<Type>::Push(value, priority);
    }
    void Pop() override
    {
        return QueueInterface<Type>::PopPriority();
    }
};

void TestQueue()
{
    std::cout << "TestQueue"s << std::endl;
    Queue<int> q;
    q.Push(10);
    q.Push(12);
    q.Push(13);
    q.Print();
    std::cout << std::endl;
    assert(10 == q.Back());
    assert(13 == q.Front());
    assert(3 == q.Size());
    assert(!q.Empty());
    q.Pop();
    q.Print();
    std::cout << std::endl;
    assert(12 == q.Back() && 13 == q.Front());
    assert(2 == q.Size());
    q.Pop();
    q.Print(); std::cout << std::endl;
    assert(13 == q.Back() && 13 == q.Front());
    assert(1 == q.Size());
    q.Pop();
    q.Print();
    std::cout << std::endl;
    assert(q.Empty());   
	std::cout << "TestQueue is Ok\n" << std::endl;	
}

void TestQueueWithPriorityPush()
{
    std::cout << "TestQueueWithPriorityPush"s << std::endl;
    QueueWithPriorityPush<int> q;
    assert(q.Empty());
    q.Push(10, 2);
    q.Push(9, 5);
    q.Print();
    std::cout << std::endl;
    assert(9 == q.Front());
    assert(10 == q.Back());
    assert(2 == q.Size());
    q.Push(11, 3);
    q.Push(5, 4);
    q.Print();
    std::cout << std::endl;
    assert(9 == q.Front());
    assert(10 == q.Back());
    assert(4 == q.Size());
    q.Pop();
    q.Print();
    q.Push(5, 0);
    q.Print();
   
    std::cout << std::endl;
    assert(9 == q.Front());
    assert(5 == q.Back());
    assert(4 == q.Size());
	std::cout << "TestQueueWithPriorityPush is Ok\n" << std::endl;
}

void TestQueueWithPriorityPop()
{   
    std::cout << "TestQueueWithPriorityPop"s << std::endl; 
    QueueWithPriorityPop<int> q;
    assert(q.Empty());
    q.Push(10, 10);
    q.Push(9, 5);
    q.Print();
    std::cout << std::endl;
    assert(9 == q.Front());
    assert(10 == q.Back());
    assert(2 == q.Size());
    q.Push(5, 4);
    q.Print();
    std::cout << std::endl;
    assert(5 == q.Front());
    assert(10 == q.Back());
    assert(3 == q.Size());
    q.Pop();
    q.Print();
    std::cout << std::endl;
    assert(5 == q.Front());
    assert(9 == q.Back());
    assert(2 == q.Size());
    q.Push(1, 10);
    q.Print();
    std::cout << std::endl;
    assert(1 == q.Front());
    assert(9 == q.Back());
    assert(3 == q.Size());
    q.Pop();
    q.Print();
    std::cout << std::endl;
    assert(5 == q.Front());
    assert(9 == q.Back());
    assert(2 == q.Size());
	std::cout << "TestQueueWithPriorityPop is Ok\n" << std::endl;
}

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

template<class Type>
std::ostream& operator<<(std::ostream& out, const Stack<Type>& s)
{
    if (s.Size())
    {
        auto *tmp = s.GetNode();
        for (size_t i = 0; i < s.Size(); ++i)
        {
            out << tmp->value;
            tmp = tmp->next;
        }      
    }
    else
    {
        out << "Stack is empty"s;
    }
    return out;
}

void TestStack()
{	   
	std::cout << "TestDecToBin" << std::endl;
    Stack<int> s;
    assert(s.Empty());
    s.Push(1);
    assert(1 == s.Top());
    assert(1 == s.Size());
    s.Push(2);
    assert(2 == s.Top());
    assert(2 == s.Size());
    s.Push(3);
    assert(3 == s.Top());
    assert(3 == s.Size());    
    s.Pop();
    assert(2 == s.Top());
    assert(2 == s.Size());
	std::cout << "TestDecToBin is Ok\n" << std::endl;
}

void DecToBinWithStack(int x, Stack<int>& result)
{
    result.Push(x % 2);
    if (x > 0)
    {
        DecToBinWithStack(x / 2, result);
    }   
}

void DecToBin(int x, std::ostream& out)
{
    if (x > 0)
    {
        DecToBin(x / 2, out);
    }
    out << x % 2;
}

void TestDecToBin()
{
	std::cout << "TestDecToBin" << std::endl;
    Stack<int> result;
    int number = 100;
    std::stringstream str;   
    DecToBinWithStack(number, result);
    str << result;
    std::stringstream str1;
    DecToBin(number, str1);
    assert(str.str() == str1.str());
	std::cout << "TestDecToBin is Ok\n" << std::endl;
}


int main()
{	
	TestStack();    
	TestDecToBin();
	TestQueue();
	TestQueueWithPriorityPush();
	TestQueueWithPriorityPop();
	
	
	return 0;
}