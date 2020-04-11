#include <iostream>
#include <string>

#include <nostd/Vector.hpp>
#include <TestObject.hpp>

template<typename T>
void destroy(T* ptr)
{
    ptr->~T();
}

int main()
{
    nostd::Vector<TestObject> vec(1);

    // move object
    vec.push_back(TestObject(10));

    // copy object
    TestObject obj(11);
    vec.push_back(obj);
    obj.set(-1);

    // make object at the storage already
    vec.emplace_back(12);

    nostd::Vector<TestObjectMultiplies> vec2;
    vec2.emplace_back(5, 5);
    vec2.emplace_back(2, 8);
    vec2.emplace_back(1, 1, 1, 1, 1, 3);
    vec2.emplace_back(2, 3, 4, 5, 6);


    std::cout << "vec: ";
    for (const TestObject& el : vec)
        std::cout << el << ", ";
    std::cout << std::endl;

    std::cout << "vec2: ";
    for (const auto& el : vec2)
        std::cout << el.value << ", ";
    std::cout << std::endl;
}
