#pragma once

#include <cstddef>

#include <algorithm>
#include <iterator>

#define iterator_impl(iterator_name)\
\
    using difference_type = ptrdiff_t;\
    using iterator_category = std::random_access_iterator_tag;\
\
    iterator_name& operator++(){ data_++; return *this; }\
    iterator_name  operator++(int){ iterator_name temp(data_); data_++; return temp; }\
    iterator_name& operator--(){ data_--; return *this; }\
    iterator_name  operator--(int){ iterator_name temp(data_); data_--; return temp; }\
\
    iterator_name operator+(int value) const\
    {\
        iterator_name result(data_);\
        result.data_ += value;\
        return result;\
    }\
    iterator_name operator-(int value) const\
    {\
        iterator_name result(data_);\
        result.data_ -= value;\
        return result;\
    }\
    ptrdiff_t operator-(iterator_name other) const { return data_ - other.data_; }\
    bool operator==(iterator_name other) const { return data_ == other.data_; }\
    bool operator!=(iterator_name other) const { return data_ != other.data_; }\
    bool operator<(iterator_name other) const { return data_ < other.data_; }

/////////////////////////////////////////////////////////////////////////////////////////

namespace nostd
{

template<typename T, typename Allocator = std::allocator<T>>
class Vector
{
public:
    using value_type = T;

    Vector(size_t size)
    {
        if (size != 0)
        {
            first_ = allocator_.allocate(size);
            realEnd_ = end_ = first_ + size;
            constructorOnRange_(first_, end_);
        }
    }

    Vector(size_t size, const T& obj)
    {
        if (size != 0)
        {
            first_ = allocator_.allocate(size);
            realEnd_ = end_ = first_ + size;
            constructorOnRange_(first_, end_, obj);
        }
    }

    Vector()
    {
        first_ = end_ = allocator_.allocate(DEFAULT_SIZE);
        realEnd_ = first_ + DEFAULT_SIZE;
    }

    Vector(const Vector<T>& other)
    {
        reserveGrowTo_(other.size());
        std::copy(other.begin(), other.end(), std::back_inserter(*this));
    }

    Vector(Vector<T>&& other)
    {
        first_ = other.first_;
        end_ = other.end_;
        realEnd_ = other.realEnd_;

        other.first_ = other.end_ = other.realEnd_ = nullptr;
    }

    Vector<T>& operator=(const Vector<T>& other)
    {
        reserveGrowTo_(other.realSize_());
        std::copy(other.begin(), other.end(), begin());
    }

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

    void pop_back()
    {
        end_ -= 1;
        end_->~T();
    }

    T& front() { return *first_; }
    T& back() { return *(end_ - 1); }

    const T& front() const { return *first_; }
    const T& back() const { return *(end_ - 1); }

    T& operator[](size_t idx) { return first_[idx]; }
    const T& operator[](size_t idx) const { return first_[idx]; }

    /////////////////////////////////////////////////////////////////////////////////////

    class const_iterator;

    class iterator
    {
    public:
        iterator_impl(iterator)

        using value_type = T;
        using pointer = T*;
        using reference = T&;

        explicit iterator(T* ptr) : data_(ptr) {}
        iterator() : data_(nullptr) {}

        operator const_iterator() const;

        T& operator*() const
        {
            return *data_;
        }

        T* operator->() const
        {
            return data_;
        }

    protected:
        T* data_;
    };

    class const_iterator
    {
    public:
        iterator_impl(const_iterator)

        using value_type = const T;
        using pointer = const T*;
        using reference = const T&;

        explicit const_iterator(const T* ptr) : data_(ptr) {}
        const_iterator() : data_(nullptr) {}

        const T& operator*() const
        {
            return *data_;
        }

        const T* operator->() const
        {
            return data_;
        }

    protected:
        const T* data_;
    };

    iterator begin() { return iterator(first_); }
    iterator end() { return iterator(end_); }

    const_iterator begin() const { return const_iterator(first_); }
    const_iterator end() const { return const_iterator(end_); }

    /////////////////////////////////////////////////////////////////////////////////////

    void erase(const_iterator iterator)
    {
        throw std::runtime_error("Vector<T>::erase(): Method not implemented");
    }

    void resize(unsigned newSize)
    {
        if (newSize < size())
        {
            shrinkTo_(newSize);
        }
        else if (newSize > size())
        {
            growTo_(newSize);
        }
    }

protected:
    void shrinkTo_(unsigned newSize)
    {
        destructorOnRange_(first_ + newSize, end_);
        end_ = first_ + newSize;
    }

    void growTo_(unsigned newSize)
    {
        if (newSize <= realSize_())
        {
            constructorOnRange_(end_, first_ + newSize);
            end_ = first_ + newSize;
        }
        else
        {
            auto oldSize = size();
            auto newFirst = allocator_.allocate(newSize);

            for (size_t i = 0; i < oldSize; ++i)
                new(newFirst + i) T(std::move(first_[i]));

            destructorOnRange_(first_, end_);
            allocator_.deallocate(first_, oldSize);

            constructorOnRange_(newFirst + oldSize, newFirst + newSize);
            first_ = newFirst;
            end_ = realEnd_ = newFirst + newSize;
        }
    }

    void reserveGrowTo_(unsigned newSize)
    {
        auto oldSize = size();
        auto newFirst = allocator_.allocate(newSize);

        for (size_t i = 0; i < oldSize; ++i)
            new(newFirst + i) T(std::move(first_[i]));

        destructorOnRange_(first_, end_);
        allocator_.deallocate(first_, oldSize);

        first_ = newFirst;
        end_ = first_ + oldSize;
        realEnd_ = first_ + newSize;
    }

    void destructorOnRange_(T* first, T* end)
    {
        for (; first != end; ++first)
            first->~T();
    }

    void constructorOnRange_(T* first, T* end)
    {
        for(; first != end; ++first)
            new(first) T();
    }

    void constructorOnRange_(T* first, T* end, const T& obj)
    {
        for(; first != end; ++first)
            new(first) T(obj);
    }

    void grow_()
    {
        auto oldSize = size();
        reserveGrowTo_(oldSize == 0 ? 1 : 2 * oldSize);
    }

    size_t realSize_() const
    {
        return realEnd_ - first_;
    }

    constexpr static size_t DEFAULT_SIZE = 4;

    Allocator allocator_;
    T *first_ = nullptr, *end_ = nullptr, *realEnd_ = nullptr;
};

template<typename T, typename Allocator>
Vector<T, Allocator>::iterator::operator const_iterator() const
{
    return Vector<T>::const_iterator(data_);
}

}  // namespace nostd

#undef iterator_impl
