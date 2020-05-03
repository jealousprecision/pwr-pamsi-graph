#pragma once

#include <iostream>

class TestObject
{
public:
    TestObject() :
        data_(new int(42))
    {
        std::cout << "TestObject::TestObject()" << std::endl;
    }

    explicit TestObject(int i) :
        data_(new int(i))
    {
        std::cout << "TestObject::TestObject(int = " << i << ")" << std::endl;
    }

    TestObject(const TestObject& other) :
        data_(new int(*(other.data_)))
    {
        std::cout << "TestObject::TestObject(const TestObject& = " << *data_ << ")" << std::endl;
    }

    TestObject(TestObject&& other)
    {
        std::cout << "TestObject::TestObject(TestObject&& = " << *other.data_ << ")" << std::endl;

        data_ = other.data_;
        other.data_ = nullptr;
    }

    ~TestObject()
    {
        std::cout << "TestObject::~TestObject(): data_ = ";

        if (data_ == nullptr)
        {
            std::cout << "nullptr" << std::endl;
        }
        else
        {
            std::cout << *data_ << std::endl;
            delete data_;
            data_ = nullptr;
        }
    }

    operator int() const
    {
        std::cout << "TestObject::operator int() = " << *data_ << std::endl;
        return *data_;
    }

    TestObject& operator=(const TestObject& other)
    {
        if (other.data_ == nullptr)
            throw std::runtime_error("TestObject::operator=(const TestObject&): copy from empty object");

        std::cout << "TestObject::operator=(const TestObject& = " << *other.data_ << ")" << std::endl;

        *data_ = *other.data_;
        return *this;
    }

    TestObject& operator=(TestObject&& other)
    {
        if (other.data_ == nullptr)
            throw std::runtime_error("TestObject::operator=(TestObject&&): move from empty object");

        std::cout << "TestObject::operator=(TestObject&& = " << *other.data_ << ")" << std::endl;

        data_ = other.data_;
        other.data_ = nullptr;

        return *this;
    }

    void set(int i)
    {
        *data_ = i;
    }

    friend std::ostream& operator<< (std::ostream& os, const TestObject& obj);

protected:
    void tryDeletingOwnMemory_()
    {
        if (data_ != nullptr)
            delete data_;
    }

    int* data_ = nullptr;
};

inline std::ostream& operator<< (std::ostream& os, const TestObject& obj)
{
    return os << *(obj.data_);
}

class TestObjectMultiplies
{
public:
    TestObjectMultiplies(int i)
    {
        value *= i;
    }

    template<typename... Args>
    TestObjectMultiplies(int i, Args&&... args) : TestObjectMultiplies(args...)
    {
        value *= i;
    }

    int value = 1;
};
