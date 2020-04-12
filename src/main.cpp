#include <cstdlib>
#include <ctime>
#include <cmath>

#include <algorithm>
#include <iterator>

#include <nostd/Vector.hpp>
#include <nostd/List.hpp>
#include <nostd/MergeSort.hpp>
#include <nostd/QuickSort.hpp>
#include <TestObject.hpp>

int main()
{
    srand(time(nullptr));

    nostd::List<TestObject> list;

    for (int i = 0; i < 5; ++i)
        list.push_back(i);

    list.erase(list.begin());
    list.erase(std::prev(list.end()));

    std::copy(list.begin(), list.end(), std::ostream_iterator<int>(std::cout, ", "));
    std::cout << std::endl;
}
