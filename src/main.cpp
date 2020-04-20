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
#include <GraphList.hpp>
#include <Matrix.hpp>

int main()
{
    Matrix<int> matrix(3, 4);

    int i = 0;
    for (int x = 0; x < 4; ++x)
        for (int y = 0; y < 3; ++y)
            matrix(y, x) = i++;

    matrix.growByOne();
    matrix.log(std::cout);
}
