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

    {
        TestObject obj(10);
        list.push_back(obj);
    }

    list.push_back(TestObject(11));

    list.emplace_back(12);

    list.erase(list.begin());
    list.erase(std::prev(list.end()));

    std::copy(list.begin(), list.end(), std::ostream_iterator<TestObject>(std::cout, ", "));
    std::cout << std::endl;
}
