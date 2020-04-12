#pragma once

#include <cstddef>
#include <iterator>
#include <optional>

#define iterator_impl(iterator_name)\
\
    using difference_type = ptrdiff_t;\
    using iterator_category = std::bidirectional_iterator_tag;\
\
    friend List<T>;\
\
    iterator_name& operator++(){ data_ = data_->next; return *this; }\
    iterator_name  operator++(int){ iterator_name temp(data_); data_ = data_->next; return temp;}\
    iterator_name& operator--(){ data_ = data_->previous; return *this; }\
    iterator_name  operator--(int){ iterator_name temp(data_); data_ = data_->previous; return temp;}\
\
    bool operator==(iterator_name other) const { return data_ == other.data_; }\
    bool operator!=(iterator_name other) const { return data_ != other.data_; }

/////////////////////////////////////////////////////////////////////////////////////////

namespace nostd
{

template<typename T>
class List
{
public:
    List();

    List(const List<T>& other) = delete;
    List(List<T>&& other) = delete;
    List& operator=(const List<T>& other) = delete;
    List& operator=(List<T>&& other) = delete;

    ~List();

    struct Node
    {
        Node* next = nullptr;
        Node* previous = nullptr;
        std::optional<T> obj = std::nullopt;
    };

    void push_back(const T& obj)
    {
        end_->obj.emplace(obj);
        advanceEndNode_();
    }

    void push_back(T&& obj)
    {
        end_->obj.emplace(std::move(obj));
        advanceEndNode_();
    }

    template<typename... Args>
    void emplace_back(Args&&... args)
    {
        end_->obj.emplace(std::forward<Args>(args)...);
        advanceEndNode_();
    }

    T& front() { return front_->obj; }
    T& back() { return end_->previous->obj; }

    const T& front() const { return front_->obj; }
    const T& back() const { return end_->previous->obj; }

    size_t size() const { return size_; }

    class const_iterator;

    class iterator
    {
    public:
        iterator_impl(iterator)

        using value_type = T;
        using pointer = T*;
        using reference = T&;

        iterator(Node* data) : data_(data) {}
        operator const_iterator();

        T& operator*()
        {
            return *(data_->obj);
        }

        T* operator->()
        {
            return &*(data_->obj);
        }

    protected:
        Node* data_;
    };

    class const_iterator
    {
    public:
        iterator_impl(const_iterator)

        using value_type = const T;
        using pointer = const T*;
        using reference = const T&;

        const_iterator(const Node* data) : data_(data) {}

        const T& operator*()
        {
            return *(data_->obj);
        }

        const T* operator->()
        {
            return &*(data_->obj);
        }

    protected:
        const Node* data_;
    };

    iterator begin() { return iterator(front_); }
    iterator end() { return iterator(end_); }

    const_iterator begin() const { return const_iterator(front_); }
    const_iterator end() const { return const_iterator(end_); }

    void erase(const_iterator iterator)
    {
        Node* prevNode = iterator.data_->previous;
        Node* nextNode = iterator.data_->next;

        if (prevNode != nullptr)
            prevNode->next = nextNode;
        if (nextNode != nullptr)
            nextNode->previous = prevNode;

        if (iterator.data_ == front_)
            front_ = nextNode;

        delete iterator.data_;

        size_ -= 1;
    }

protected:
    void advanceEndNode_()
    {
        end_->next = new Node{nullptr, end_, std::nullopt};
        end_ = end_->next;
        size_ += 1;
    }

    size_t size_ = 0;
    Node* front_ = nullptr;
    Node* end_ = nullptr;
};

template<typename T>
List<T>::List()
{
    front_ = end_ = new Node{nullptr, nullptr, std::nullopt};
}

template<typename T>
List<T>::~List()
{
    Node* previous = front_;
    for (auto it = ++begin(); it != end(); ++it)
    {
        delete previous;
        previous = it.data_;
    }
    delete previous;
    delete end_;
}

template<typename T>
List<T>::iterator::operator const_iterator()
{
    return const_iterator(data_);
}

}  // namespace nostd

#undef iterator_impl
