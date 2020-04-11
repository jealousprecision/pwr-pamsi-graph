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

    TestObject(int i) :
        data_(new int(i))
    {
        std::cout << "TestObject::TestObject(int)" << std::endl;
    }

    TestObject(const TestObject& other) :
        data_(new int(*(other.data_)))
    {
        std::cout << "TestObject::TestObject(const TestObject&)" << std::endl;
    }

    TestObject(TestObject&& other)
    {
        std::cout << "TestObject::TestObject(TestObject&&)" << std::endl;

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

    void set(int i)
    {
        *data_ = i;
    }

    friend std::ostream& operator<< (std::ostream& os, const TestObject& obj);

protected:
    int* data_ = nullptr;
};

inline std::ostream& operator<< (std::ostream& os, const TestObject& obj)
{
    return os << *(obj.data_);
}
