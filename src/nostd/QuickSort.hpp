#pragma once

#include <functional>
#include <iterator>
#include <nostd/Algo.hpp>

namespace nostd
{

template<typename Iter, typename Comp = std::less<typename Iter::value_type>>
void quick_sort(Iter first, Iter end, Comp comp = std::less<typename Iter::value_type>())
{
    if (first == end)
        return;

    auto keyIt = std::next(first, std::distance(first, end) / 2);
    auto it = algo::partition_with_pivot(first, keyIt, end, comp);

    quick_sort(first, it, comp);
    quick_sort(std::next(it), end, comp);
}

} // namespace nostd
