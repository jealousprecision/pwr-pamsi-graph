#pragma once

#include <cstddef>
#include <iterator>

#define iterator_impl(iterator_name)\
\
    using iterator_category = std::bidirectional_iterator_tag;\
\
    iterator_name& operator++(){ data_++; return *this; }\
    iterator_name& operator--(){ data_--; return *this; }\
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
        T* obj = nullptr;
    };

    void push_back(const T& obj)
    {
        end_->obj = new T(obj);
        advanceEndNode_();
    }

    void push_back(T&& obj)
    {
        end_->obj = new T(std::move(obj));
        advanceEndNode_();
    }

    template<typename... Args>
    void emplace_back(Args&&... args)
    {
        end_->obj = new T(std::forward<Args>(args)...);
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

        friend List<T>;
        friend const_iterator;

        iterator(Node* data) : data_(data) {}

        T& operator*()
        {
            return data_->obj;
        }

        T* operator->()
        {
            return &(data_->obj);
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
        const_iterator(iterator iter) : data_(iter.data_) {}

        const T& operator*()
        {
            return data_->obj;
        }

        const T* operator->()
        {
            return &(data_->obj);
        }

    protected:
        const Node* data_;
    };

    iterator begin() { return iterator(front_); }
    iterator end() { return iterator(end_); }

    const_iterator begin() const { return const_iterator(front_); }
    const_iterator end() const { return const_iterator(end_); }

protected:
    void advanceEndNode_()
    {
        end_->next = new Node{nullptr, end_, nullptr};
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
    end_ = new Node{nullptr, nullptr, nullptr};
    front_ = end_;
}

template<typename T>
List<T>::~List()
{
    Node* previous = front_;
    for (auto it = ++begin(); it != end(); ++it)
    {
        delete previous->obj;  // for each node, delete held object
        delete previous;  // and then node itself
        previous = it.data_;
    }
    delete previous->obj;
    delete previous;
    delete end_;  // end node doesn't hold an object
}

}  // namespace nostd

#undef iterator_impl
