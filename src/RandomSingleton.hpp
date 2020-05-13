#pragma once

#include <random>

class RandomSingleton
{
public:
    static double rand()
    {
        static std::random_device rd;
        static std::mt19937 mt(rd());
        static std::normal_distribution<double> dist(2500, 1500);

        return dist(mt);
    }
};
