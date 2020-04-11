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
    nostd::Vector<int> vec(3);

    TestObject* obj = nullptr;
    //void* memory = malloc(sizeof(std::string));
    std::allocator<TestObject> allocator;
    TestObject* memory = allocator.allocate(1);

    obj = new(memory) TestObject;

    std::cout << *obj << std::endl << obj << '\t' << memory << std::endl;

    obj->set(-15);

    destroy(obj);
    allocator.deallocate(obj, 1);
}
