#pragma once

#include <functional>
#include <iterator>
#include <nostd/Algo.hpp>

namespace nostd
{

template<typename Iter, typename Comp = std::less<typename Iter::value_type>>
void merge_sort(Iter first, Iter end, Comp comp = std::less<typename Iter::value_type>())
{
    auto size = std::distance(first, end);
    if (size <= 1)
        return;

    auto halfRangeEnd = std::next(first, size / 2);

    merge_sort(first, halfRangeEnd, comp);
    merge_sort(halfRangeEnd, end, comp);

    algo::merge(first, halfRangeEnd, end, comp);
}

}  // namespace nostd
