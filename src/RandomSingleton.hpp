#pragma once

#include <iostream>
#include <random>

class RandomSingleton
{
    struct RandomImpl
    {
        RandomImpl() :
            mt(rd()),
            dist(2500, 1000)
        {}

        double operator()()
        {
            return dist(mt);
        }

        std::random_device rd;
        std::mt19937 mt;
        std::normal_distribution<double> dist;
    };

public:
    static unsigned rand()
    {
        static RandomImpl randImpl;

        int ret;
        do {
            ret = randImpl();
        } while(ret < 0);

        //std::cout << "Alive! " << ret << std::endl;
        return ret;
    }
};
