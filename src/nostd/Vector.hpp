#pragma once

#include <cstring>

#include <algorithm>
#include <memory>
#include <iterator>
#include <utility>

//#define DEFAULT_SIZE 4

namespace nostd
{

template<typename T, typename Allocator = std::allocator<T>>
class Vector
{
public:
    Vector(size_t size)
    {
        if (size != 0)
        {
            first_ = end_ = allocator_.allocate(size);
            realEnd_ = first_ + size;
        }
    }

    Vector() : Vector(DEFAULT_SIZE) {}

    Vector(const Vector<T>& other) : Vector(other.size())
    {
        std::copy(other.begin(), other.end(), std::back_inserter(*this));
    }

    Vector(Vector<T>&& other)
    {
        first_ = other.first_;
        end_ = other.end_;
        realEnd_ = other.realEnd_;

        other.first_ = other.end_ = other.realEnd_ = nullptr;
    }

    Vector<T>& operator=(const Vector<T>& other) = delete;

    Vector<T>& operator=(Vector<T>&& other)
    {
        destructorOnRange_(first_, end_);
        allocator_.deallocate(first_, realSize_());

        first_ = other.first_;
        end_ = other.end_;
        realEnd_ = other.realEnd_;

        other.first_ = other.end_ = other.realEnd_ = nullptr;
    }

    ~Vector()
    {
        destructorOnRange_(first_, end_);
        allocator_.deallocate(first_, realSize_());
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

    void push_back(T&& obj)
    {
        if (end_ == realEnd_)
            grow_();

        new(end_) T(std::move(obj));
        end_ += 1;
    }

    template<typename... Args>
    void emplace_back(Args&&... args)
    {
        if (end_ == realEnd_)
            grow_();

        new(end_) T(std::forward<Args>(args)...);
        end_ += 1;
    }

    T& front() { return *first_; }
    T& back() { return *(end_ - 1); }

    const T& front() const { return *first_; }
    const T& back() const { return *(end_ - 1); }

    T& operator[](size_t idx) { return first_[idx]; }
    const T& operator[](size_t idx) const { return first_[idx]; }

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

        const_iterator(const T* ptr) : data_(ptr) {}

        const_iterator& operator++()
        {
            data_++;
            return *this;
        }

        const_iterator operator++(int)
        {
            const_iterator ret(data_);
            data_++;
            return ret;
        }

        const_iterator& operator--()
        {
            data_--;
            return *this;
        }

        const_iterator operator--(int)
        {
            const_iterator ret(data_);
            data_--;
            return ret;
        }

        const T& operator*()
        {
            return *data_;
        }

        const T* operator->()
        {
            return data_;
        }

        bool operator==(const_iterator other) const
        {
            return data_ == other.data_;
        }

        bool operator!=(const_iterator other) const
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
    void destructorOnRange_(T* first, T* end)
    {
        for (; first != end; ++first)
            first->~T();
    }

    void grow_()
    {
        auto newSize = size() ? 2 * size() : 1;  // to work when vector is empty
        auto newFirst = allocator_.allocate(newSize);

        size_t i = 0;
        for (auto& el : *this)
            new(newFirst + i++) T(std::move(el));

        destructorOnRange_(first_, end_);
        allocator_.deallocate(first_, newSize / 2);

        first_ = newFirst;
        end_ = newFirst + newSize / 2;
        realEnd_ = newFirst + newSize;
    }

    size_t realSize_()
    {
        return realEnd_ - first_;
    }

    constexpr static size_t DEFAULT_SIZE = 4;

    Allocator allocator_;
    T *first_ = nullptr, *end_ = nullptr, *realEnd_ = nullptr;
};

}  // namespace nostd

//#undef DEFAULT_SIZE
