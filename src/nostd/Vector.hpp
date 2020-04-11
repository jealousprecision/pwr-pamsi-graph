#pragma once

#include <memory>

#define DEFAULT_SIZE 3

namespace nostd
{

template<typename T, typename Allocator = std::allocator<T>>
class Vector
{
public:
    Vector(size_t size)
    {
        first_ = end_ = allocator_.allocate(size);
        realEnd_ = first_ + size;
    }

    ~Vector()
    {
        for (auto& el : *this)
        {
            el.~T();
        }

        allocator_.deallocate(first_, size());
    }

    size_t size() const
    {
        return end_ - first_;
    }

    void push_back(const T& obj)
    {
        if (end_ == realEnd_)
            grow_();

        new(end_) T(obj);  // make object in place pointed by end_
        end_ += 1;
    }

    class iterator
    {
    public:
        using value_type = T;

        iterator(T* ptr) : data_(ptr) {}

        iterator& operator++()
        {
            data_++;
            return *this;
        }

        iterator operator++(int)
        {
            iterator ret(data_);
            data_++;
            return ret;
        }

        iterator& operator--()
        {
            data_--;
            return *this;
        }

        iterator operator--(int)
        {
            iterator ret(data_);
            data_--;
            return ret;
        }

        T& operator*()
        {
            return *data_;
        }

        T* operator->()
        {
            return data_;
        }

        bool operator==(iterator other) const
        {
            return data_ == other.data_;
        }

        bool operator!=(iterator other) const
        {
            return data_ != other.data_;
        }

    protected:
        T* data_;
    };

    class const_iterator
    {
    public:
        using value_type = T;

        const_iterator(T* ptr) : data_(ptr) {}

        iterator& operator++()
        {
            data_++;
            return *this;
        }

        iterator operator++(int)
        {
            iterator ret(data_);
            data_++;
            return ret;
        }

        iterator& operator--()
        {
            data_--;
            return *this;
        }

        iterator operator--(int)
        {
            iterator ret(data_);
            data_--;
            return ret;
        }

        T& operator*()
        {
            return *data_;
        }

        T* operator->()
        {
            return data_;
        }

        bool operator==(iterator other) const
        {
            return data_ == other.data_;
        }

        bool operator!=(iterator other) const
        {
            return data_ != other.data_;
        }

    protected:
        const T* data_;
    };

    iterator begin() { return iterator(first_); }
    iterator end() { return iterator(end_); }

    const_iterator begin() const { return const_iterator(first_); }
    const_iterator end() const { return const_iterator(end_); }

protected:
    void grow_()
    {
        //TODO!
    }

    Allocator allocator_;
    T *first_ = nullptr, *end_ = nullptr, *realEnd_ = nullptr;
};

}  // namespace nostd

#undef DEFAULT_SIZE
