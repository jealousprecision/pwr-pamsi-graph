#pragma once

#include "List.hpp"
#include "Vector.hpp"

namespace nostd
{

class DisjointSet
{
public:
    DisjointSet(unsigned size) :
        data_(size, -1)
    {}

    bool unionize(unsigned parentIdx, unsigned childIdx)
    {
        childIdx = getIdx_(childIdx);
        parentIdx = getIdx_(parentIdx);

        if (childIdx == parentIdx)
            return false;

        data_[childIdx] = parentIdx;
        return true;
    }

    bool isUnion(unsigned parentIdx, unsigned childIdx)
    {
        return getIdx_(parentIdx) == getIdx_(childIdx) ? true : false;
    }

    unsigned operator[](unsigned idx) const { return data_[idx]; }
    unsigned size() const { return data_.size(); }
    auto begin() const { return data_.begin(); }
    auto end() const { return data_.end(); }

protected:
    unsigned getIdx_(unsigned idx)
    {
        while(data_[idx] != -1)
            idx = data_[idx];

        return idx;
    }

    nostd::Vector<int> data_;
};

}  // namespace nostd
