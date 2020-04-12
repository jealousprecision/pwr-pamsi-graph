#include <cstdlib>
#include <ctime>

#include <algorithm>
#include <iterator>

#include <nostd/Vector.hpp>
#include <nostd/List.hpp>
#include <TestObject.hpp>

int main()
{
    srand(time(nullptr));

    nostd::Vector<int> vec;

    for (int i = 0; i < 10; ++i)
        vec.push_back(rand() % 10);

    std::sort(vec.begin(), vec.end());

    for (const auto& item : vec)
        std::cout << item << ", ";
    std::cout << std::endl;

    //std::cout << "Hello, size: " << std::distance(list.begin(), list.end()) << '\t' << list.size() << std::endl;
}
