#pragma once

#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>

#include <nostd/Vector.hpp>

namespace algo
{

template<typename Iter, typename Predicate>
Iter partition(Iter first, Iter end, Predicate pred)
{
    auto endIt = end;
    auto it = first;

    while (it != endIt)
    {
        if (pred(*it))
            ++it;
        else
            std::swap(*it, *--endIt);
    }

    return it;
}

template<typename Iter, typename Comp>
Iter partition_with_pivot(Iter first, Iter pivot, Iter end, Comp comp)
{
    auto predicate = std::bind(comp, std::placeholders::_1, *pivot);
    auto preEnd = std::prev(end);

    std::swap(*pivot, *preEnd);
    auto it = ::algo::partition(first, preEnd, predicate);
    std::swap(*it, *preEnd);

    return it;
}

template<typename Iter, typename Comp>
void merge(Iter first, Iter halfRangeEnd, Iter end, Comp comp)
{
    nostd::Vector<typename Iter::value_type> temp(std::distance(first, end));

    auto halfRangeIt = halfRangeEnd;
    auto it = first;
    auto inserterIt = temp.begin();
    while (true)
    {
        if (it == halfRangeEnd)
        {
            std::copy(halfRangeIt, end, inserterIt);
            break;
        }
        if (halfRangeIt == end)
        {
            std::copy(it, halfRangeEnd, inserterIt);
            break;
        }

        if (comp(*halfRangeIt, *it))
            *inserterIt++ = *halfRangeIt++;
        else
            *inserterIt++ = *it++;
    }

    std::copy(temp.begin(), temp.end(), first);
}

template<typename InsertIt, typename Gen>
void generate_n_el(InsertIt It, size_t count, Gen gen)
{
    for (size_t i = 0; i < count; ++i)
        *It++ = gen();
}

constexpr size_t pow(size_t a, size_t b)
{
    size_t ret = 1;
    for (size_t i = 0; i < b; ++i)
        ret *= a;
    return ret;
}

template<typename Iter, typename T>
T average(Iter begin, Iter end, T firstValue)
{
    return std::accumulate(begin, end, firstValue) / std::distance(begin, end);
}

template<typename Iter>
void fill_random(Iter iter, Iter end)
{
    for (; iter != end; ++iter)
        *iter = std::rand();
}

template<typename Iter>
void fill_random(Iter iter, Iter end, size_t max)
{
    for (; iter != end; ++iter)
        *iter = std::rand() % max;
}

}  // namespace algo
