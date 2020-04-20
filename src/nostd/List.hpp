#pragma once

#include <cstddef>

#include <algorithm>
#include <iterator>
#include <functional>

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
    List(const List<T>& other);
    List(List<T>&& other);

    List& operator=(const List<T>& other) = delete;
    List& operator=(List<T>&& other) = delete;

    ~List();

    struct AbstractNode
    {
        virtual ~AbstractNode() = default;

        virtual T& get() = 0;
        virtual const T& get() const = 0;

        AbstractNode* next = nullptr;
        AbstractNode* previous = nullptr;
    };

    struct EmptyNode : public AbstractNode
    {
        T& get() override
        {
            throw std::runtime_error("List<>::EmptyNode::get(): dereferencing empty node");
        }

        const T& get() const override
        {
            throw std::runtime_error("List<>::EmptyNode::get(): dereferencing empty node");
        }
    };

    struct BasicNode : public AbstractNode
    {
        BasicNode(const T& obj) :
            obj_(obj)
        {}

        BasicNode(T&& obj) :
            obj_(std::move(obj))
        {}

        template<typename... Args>
        BasicNode(Args&&... args) :
            obj_(std::forward<Args>(args)...)
        {}

        T& get() override
        {
            return obj_;
        }

        const T& get() const override
        {
            return obj_;
        }

    protected:
        T obj_;
    };

    void push_back(const T& obj)
    {
        advanceEndNode_(new BasicNode(obj));
    }

    void push_back(T&& obj)
    {
        advanceEndNode_(new BasicNode(std::move(obj)));
    }

    template<typename... Args>
    void emplace_back(Args&&... args)
    {
        advanceEndNode_(new BasicNode(std::forward<Args>(args)...));
    }

    T& front() { return front_->get(); }
    T& back() { return end_->previous->get(); }

    const T& front() const { return front_->get(); }
    const T& back() const { return end_->previous->get(); }

    size_t size() const { return size_; }

    /////////////////////////////////////////////////////////////////////////////////////

    class const_iterator;

    class iterator
    {
    public:
        iterator_impl(iterator)

        using value_type = T;
        using pointer = T*;
        using reference = T&;

        explicit iterator(AbstractNode* data) : data_(data) {}
        iterator() : data_(nullptr) {}

        operator const_iterator() const;

        T& operator*()
        {
            return data_->get();
        }

        T* operator->()
        {
            return &data_->get();
        }

    protected:
        AbstractNode* data_;
    };

    class const_iterator
    {
    public:
        iterator_impl(const_iterator)

        using value_type = const T;
        using pointer = const T*;
        using reference = const T&;

        explicit const_iterator(const AbstractNode* data) : data_(data) {}
        const_iterator() : data_(nullptr) {}

        const T& operator*()
        {
            return data_->get();
        }

        const T* operator->()
        {
            return &data_->get();
        }

    protected:
        const AbstractNode* data_;
    };

    iterator begin() { return iterator(front_); }
    iterator end() { return iterator(end_); }

    const_iterator begin() const { return const_iterator(front_); }
    const_iterator end() const { return const_iterator(end_); }

    /////////////////////////////////////////////////////////////////////////////////////

    void erase(const_iterator iterator)
    {
        auto prevNode = iterator.data_->previous;
        auto nextNode = iterator.data_->next;

        // first node
        if (prevNode != nullptr)
            prevNode->next = nextNode;
        else
            front_ = nextNode;

        delete iterator.data_;

        size_ -= 1;
    }

    template<typename Predicate>
    void remove_if(Predicate predicate)
    {
        auto part = std::partition(begin(), end(), predicate);
        size_ -= std::distance(part, end());
        deleteRange_(begin(), part);
    }

//protected:
    void advanceEndNode_(AbstractNode* newNode)
    {
        if (size_ == 0)
        {
            front_ = newNode;

            newNode->next = end_;
            end_->previous = newNode;
        }
        else
        {
            auto prevNode = end_->previous;

            newNode->previous = prevNode;
            newNode->next = end_;

            prevNode->next = newNode;
            end_->previous = newNode;
        }

        size_ += 1;
    }

    void deleteRange_(iterator first, iterator end)
    {
        if (first == end)
            return;

        bool changeFrontPtr = false;
        AbstractNode* beforeRange = nullptr;
        if (first.data_->previous != nullptr)
        {
            // first != front_
            beforeRange = first.data_->previous;
            beforeRange->next = end.data_;
        }
        else
        {
            // first == front_
            changeFrontPtr = true;
        }

        AbstractNode* afterRange = end.data_;
        afterRange->previous = beforeRange;

        auto previous = first.data_;
        for (auto it = std::next(first); it != end; ++it)
        {
            delete previous;
            previous = it.data_;
        }
        delete previous;

        if (changeFrontPtr)
            front_ = end.data_;
    }

    size_t size_ = 0;
    AbstractNode* front_ = nullptr;
    AbstractNode* end_ = nullptr;
};

template<typename T>
List<T>::List()
{
    front_ = end_ = new EmptyNode();
}

template<typename T>
List<T>::List(const List<T>& other) :
    List()
{
    std::copy(other.begin(), other.end(), std::back_inserter(*this));
}

template<typename T>
List<T>::List(List<T>&& other)
{
    front_ = other.front_;
    end_ = other.end_;
    size_ = other.size_;

    other.front_ = other.end_ = new EmptyNode();
    other.size_ = 0;
}

template<typename T>
List<T>::~List()
{
    deleteRange_(begin(), end());
    delete end_;
}

template<typename T>
List<T>::iterator::operator const_iterator() const
{
    return const_iterator(data_);
}

}  // namespace nostd

#undef iterator_impl
