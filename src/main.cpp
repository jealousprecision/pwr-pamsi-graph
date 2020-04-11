#include <cstdlib>
#include <ctime>

#include <iostream>
#include <string>
#include <vector>

#include <nostd/Vector.hpp>
#include <TestObject.hpp>

int main()
{
    nostd::Vector<TestObject> vec(0);

    for (int i = 1; i < 4; i++)
        vec.emplace_back(i * i);
}
