#include <cstdlib>
#include <ctime>
#include <cmath>

#include <algorithm>
#include <iterator>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <chrono>

#include <nostd/Vector.hpp>
#include <nostd/List.hpp>
#include <TestObject.hpp>
#include <GraphList.hpp>
#include <GraphMatrix.hpp>
#include <GraphUtils.hpp>
#include <GraphAlgo.hpp>

using namespace std;

// Returns in ms
template<typename Func>
double timeLambda(const Func& func)
{
    auto start = chrono::steady_clock::now();
    func();
	auto end = chrono::steady_clock::now();

    return chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0;
}

template<typename GraphType>
class Test
{
public:
    Test(unsigned tests, unsigned vertices, double density) :
        tests_(tests), vertices_(vertices), density_(density)
    {}

    void operator()() const
    {
        for (auto i = 0u; i < tests_; ++i)
        {
            GraphType graph(vertices_);
            fillGraph(graph, density_);

            auto start = chrono::steady_clock::now();
            auto result = Dijkstra(graph, 0);
	        auto end = chrono::steady_clock::now();

            static bool done = false;
            if (!done)
            {
                done = true;

                auto tree = getGraphFromDijsktraOutput(std::get<0>(result), std::get<1>(result));
                std::ofstream file("graph.gv");
                logIntoGraphVizFormat(file, tree);
            }

            double msTime = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0;
            testTimes_.push_back(msTime);
        }
    }

    const auto& getTimes() const { return testTimes_; }

protected:
    unsigned tests_, vertices_;
    double density_;

    mutable nostd::Vector<double> testTimes_;
};

int main()
{
    Test<GraphMatrix<VoidType, unsigned>> test(1000, 100, 1.0\);

    test();
    for (auto el : test.getTimes())
        std::cout << el << std::endl;
}
