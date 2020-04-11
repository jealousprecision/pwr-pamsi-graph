#include <cstdlib>
#include <ctime>

#include <iostream>
#include <string>
#include <vector>

#include <nostd/Vector.hpp>
#include <TestObject.hpp>

int main()
{
    srand(time(nullptr));
    nostd::Vector<nostd::Vector<std::vector<int>>> vec;

    for (int i = 0, n = (rand() % 100) + 100; i < n; ++i)
    {
        vec.emplace_back();
        for (int j = 0, m = (rand() % 100) + 100; j < m; ++j)
        {
            vec.back().emplace_back();
            for (int k = 0, l = (rand() % 100) + 100; k < l; ++k)
            {
                vec.back().back().push_back(i * j * k);
            }
        }
    }
}
