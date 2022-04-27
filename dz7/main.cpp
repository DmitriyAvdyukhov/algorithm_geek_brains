#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <iterator>
#include <cstddef>
#include <iostream>

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

template <typename Type>
class SingleLinkedList
{
    struct Node
    {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next)
        {}
        Type value = Type();
        Node* next_node = nullptr;
    };

    // Шаблон класса Базовый Итератор.
    // Определяет поведение итератора на элементы односвязного списка
    // ValueType - совпадает с Type (для Iterator) либо с const Type (для ConstIterator)

    template <typename ValueType>
    class BasicIterator
    {
        // Класс списка объявляется дружественным, чтобы из методов списка
        // был доступ к приватной области итератора
        friend class SingleLinkedList;

        // Конвертирующий конструктор итератора из указателя на узел списка
        explicit BasicIterator(Node* node) :node_(node) {}

    public:
        // Объявленные ниже типы сообщают стандартной библиотеке о свойствах этого итератора

        // Категория итератора - forward iterator
        // (итератор, который поддерживает операции инкремента и многократное разыменование)

        using iterator_category = std::forward_iterator_tag;

        // Тип элементов, по которым перемещается итератор

        using value_type = Type;

        // Тип, используемый для хранения смещения между итераторами

        using difference_type = std::ptrdiff_t;

        // Тип указателя на итерируемое значение

        using pointer = ValueType*;

        // Тип ссылки на итерируемое значение

        using reference = ValueType&;

        BasicIterator() = default;

        // Конвертирующий конструктор/конструктор копирования
        // При ValueType, совпадающем с Type, играет роль копирующего конструктора
        // При ValueType, совпадающем с const Type, играет роль конвертирующего конструктора
        BasicIterator(const BasicIterator<Type>& other) noexcept
        {
            node_ = other.node_;
        }

        // Чтобы компилятор не выдавал предупреждение об отсутствии оператора = при наличии
        // пользовательского конструктора копирования, явно объявим оператор = и
        // попросим компилятор сгенерировать его за нас.
        BasicIterator& operator=(const BasicIterator& rhs) = default;

        // Оператор сравнения итераторов (в роли второго аргумента выступает константный итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка, либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept
        {
            return this->node_ == rhs.node_;
        }

        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept
        {
            return !(*this == rhs);
        }

        // Оператор сравнения итераторов (в роли второго аргумента итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка, либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept
        {
            return this->node_ == rhs.node_;
        }

        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept
        {
            return !(*this == rhs);
        }

        // Оператор прединкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает ссылку на самого себя
        // Инкремент итератора, не указывающего на существующий элемент списка, приводит к неопределённому поведению
        BasicIterator& operator++() noexcept
        {
            assert(node_ != nullptr);
            node_ = node_->next_node;
            return *this;
        }

        // Оператор постинкремента. После его вызова итератор указывает на следующий элемент списка.
        // Возвращает прежнее значение итератора
        // Инкремент итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        BasicIterator operator++(int) noexcept
        {
            auto old_value(*this);
            ++(*this);
            return old_value;
        }

        // Операция разыменования. Возвращает ссылку на текущий элемент
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] reference operator*() const noexcept
        {
            return node_->value;
        }

        // Операция доступа к члену класса. Возвращает указатель на текущий элемент списка.
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] pointer operator->() const noexcept
        {
            return &node_->value;
        }

    private:
        Node* node_ = nullptr;
    };

public:

    SingleLinkedList() = default;

    SingleLinkedList(std::initializer_list<Type> values)
    {
        for (const auto& temp : values)
        {
            PushBack(temp);
        }
    }

    SingleLinkedList(const SingleLinkedList& other)
    {
        assert(size_ == 0 && head_.next_node == nullptr);
        SingleLinkedList temp;
        for (auto it = other.begin(); it != other.end(); ++it)
        {
            temp.PushBack(*it);
        }
        swap(temp);
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs)
    {
        if (head_.next_node != rhs.head_.next_node)
        {
            SingleLinkedList temp(rhs);
            swap(temp);
        }
        return *this;
    }

    // Обменивает содержимое списков за время O(1)
    void swap(SingleLinkedList& other) noexcept
    {
        Node* temp_head = other.head_.next_node;
        other.head_.next_node = head_.next_node;
        head_.next_node = temp_head;
        std::swap(size_, other.size_);
    }

    ~SingleLinkedList()
    {
        Clear();
    }

    // Возвращает количество элементов в списке за время O(1)
    [[nodiscard]] size_t GetSize() const noexcept
    {
        return size_;
    }

    // Сообщает, пустой ли список за время O(1)
    [[nodiscard]] bool IsEmpty() const noexcept
    {
        if (size_ == 0)
        {
            return true;
        }
        return false;
    }

    void PushFront(const Type& value);
    void PushBack(const Type& value);
    void Clear() noexcept;
    void PopFront() noexcept;

    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator begin() noexcept
    {
        return Iterator{ head_.next_node };
    }

    [[nodiscard]] Iterator end() noexcept
    {
        return Iterator{};
    }

    [[nodiscard]] ConstIterator begin() const noexcept
    {
        return  ConstIterator{ head_.next_node };
    }

    [[nodiscard]] ConstIterator end() const noexcept
    {
        return ConstIterator{};
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept
    {
        return ConstIterator{ head_.next_node };
    }

    [[nodiscard]] ConstIterator cend() const noexcept
    {
        return ConstIterator{};
    }

    // Возвращает итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator before_begin() noexcept
    {
        return before_begin_;
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept
    {

        return  before_begin_;
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator before_begin() const noexcept
    {
        return  before_begin_;
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value)
    {
        if (pos.node_ == nullptr && pos != end())
        {
            PushFront(value);
            return Iterator{ head_.next_node };
        }
        Node* next_node = pos.node_->next_node;
        Node* new_node = new Node(value, next_node);
        pos.node_->next_node = new_node;
        ++size_;

        return Iterator{ new_node };
    }

    Iterator EraseAfter(ConstIterator pos) noexcept
    {
        Node* node_to_erase = pos.node_->next_node;
        Node* temp = node_to_erase->next_node;
        pos.node_->next_node = temp;
        delete node_to_erase;
        size_--;

        return Iterator{ temp };
    }

private:
    Node head_;
    size_t size_ = 0;
    Iterator before_begin_{ &head_ };
};

template <typename Type>
void SingleLinkedList<Type>::PopFront() noexcept
{
    Node* temp = head_.next_node;
    head_.next_node = temp->next_node;
    delete temp;
    size_--;
}

template <typename Type>
void SingleLinkedList<Type>::Clear() noexcept
{
    while (head_.next_node != nullptr)
    {
        Node* temp = head_.next_node;
        head_.next_node = temp->next_node;
        delete temp;
        size_--;
    }
}

template <typename Type>
void SingleLinkedList<Type>::PushBack(const Type& value)
{
    Node* temp = &head_;
    while (temp->next_node != nullptr)
    {
        temp = temp->next_node;
    }
    temp->next_node = new Node(value, nullptr);
    ++size_;
}

template <typename Type>
void SingleLinkedList<Type>::PushFront(const Type& value)
{
    head_.next_node = new Node(value, head_.next_node);
    ++size_;
}

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept
{
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return !std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{

    return !std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end())
        || std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{

    return !std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return !std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end())
        || std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}


bool CheckErase(char lch, char rch)
{
    if ((lch == '(' && rch == ')')
        || (lch == '[' && rch == ']')
        || (lch == '{' && rch == '}'))
    {
        return true;
    }
    return false;
}

bool CheckString(const std::string& str)
{
    Stack<char> temp;
    for (size_t i = 0; i < str.size(); ++i)
    {
        if (temp.Size())
        {
            if (CheckErase(temp.Top(), str[i]))
            {
                temp.Pop();
                continue;
            }
        }
        if (str[i] == '(' || str[i] == '[' || str[i] == '{')
        {
            temp.Push(str[i]);
        }
    }
    return !temp.Size();
}

void TestCheckString()
{
    std::string str{ "{((12 + 5)*str[4]-10)}" };
    assert(CheckString(str));
    str = "{((12 + 5)(*str[4]-10)}}";
    assert(!CheckString(str));
	std::cout << "Test string is Ok"s << std::endl;
}

void TestCopyList() 
{
    // Копирование списков
    {
        const SingleLinkedList<int> empty_list{};
        // Копирование пустого списка
        {
            auto list_copy(empty_list);
            assert(list_copy.IsEmpty());
        }

        SingleLinkedList<int> non_empty_list{ 1, 2, 3, 4 };
        // Копирование непустого списка
        {
            auto list_copy(non_empty_list);

            assert(non_empty_list.begin() != list_copy.begin());
            assert(list_copy == non_empty_list);
        }
    }  
	std::cout << "Test Copy list is Ok"s << str::endl;	
}

template<class Type>
bool IsSortList(const SingleLinkedList<Type>& list)
{
    Type check = *list.begin();
    for (const auto& l : list)
    {
        if (check > l)
        {
            return false;
        }
        check = l;
    }
    return true;
}

void TestCheckSortList()
{
    SingleLinkedList<int> ls{1, 2, 3, 4, 5, 6};
    assert(IsSortList(ls));
    SingleLinkedList<int> ls1{ 1, 2, 8, 4, 5, 6 };
    assert(!IsSortList(ls1));
	std::cout << "Test CheckSortList is Ok" << std::endl;
}

in main()
{
	TestCheckString();
	TestCopyList();
	TestCheckSortList();
	return 0;
}