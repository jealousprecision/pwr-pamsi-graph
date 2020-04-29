#pragma once

#include <nostd/Vector.hpp>

template<typename T>
class Matrix
{
public:
    Matrix(unsigned rows, unsigned cols) :
        data_(rows, nostd::Vector<T>(cols)),
        rows_(rows),
        cols_(cols)
    {
    }

    T& operator()(unsigned y, unsigned x)
    {
        return data_[y][x];
    }

    const T& operator()(unsigned y, unsigned x) const
    {
        return data_[y][x];
    }

    void log(std::ostream& os)
    {
        for (unsigned y = 0; y < rows(); ++y)
        {
            for (unsigned x = 0; x < cols(); ++x)
                os << data_[y][x] << ',';
            os << '\n';
        }
    }

    unsigned rows() const
    {
        return rows_;
    }

    unsigned cols() const
    {
        return cols_;
    }

    void growByOne()
    {
        data_.resize(data_.size() + 1);

        auto oldSize = cols();
        for (auto& vec : data_)
            vec.resize(oldSize + 1);

        rows_ += 1;
        cols_ += 1;
    }

protected:
    nostd::Vector<nostd::Vector<T>> data_;
    unsigned rows_, cols_;
};
