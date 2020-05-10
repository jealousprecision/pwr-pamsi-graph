#include <cstdlib>
#include <ctime>
#include <cmath>

#include <algorithm>
#include <iterator>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>
#include <map>

#include <nostd/Vector.hpp>
#include <nostd/List.hpp>
#include <TestObject.hpp>
#include <GraphList.hpp>
#include <GraphMatrix.hpp>
#include <GraphUtils.hpp>
#include <GraphAlgo.hpp>

template<typename GraphType>
class Test
{
public:
    Test(unsigned tests, unsigned vertices, double density) :
        tests_(tests), vertices_(vertices), density_(density)
    {}

    void operator()()
    {
        for (auto i = 0u; i < tests_; ++i)
        {
            GraphType graph(vertices_);
            fillGraph(graph, density_);

            auto start = std::chrono::high_resolution_clock::now();
            auto result = Dijkstra(graph, 0);
	        auto end = std::chrono::high_resolution_clock::now();

            double msTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
            testTimes_.push_back(msTime);
        }
    }

    const auto& getTimes() const { return testTimes_; }

protected:
    unsigned tests_, vertices_;
    double density_;

    nostd::Vector<double> testTimes_;
};

void runTests()
{
    unsigned vertices[] = {10, 50, 100, 500, 1000};
    double densities[] = {0.25, 0.5, 0.75, 1.0};

    nostd::Vector<std::tuple<std::string, nostd::Vector<double>>> testNameAndTime;

    for (auto vertNo : vertices)
    {
        for (auto density : densities)
        {
            auto testName = "GraphMatrix " + std::to_string(vertNo) + " " + std::to_string(density);
            std::cout << testName << std::endl;

            Test<GraphMatrix<VoidType, unsigned>> test(100, vertNo, density);
            test();
            testNameAndTime.push_back(std::make_tuple(testName, test.getTimes()));
        }
    }

    for (auto vertNo : vertices)
    {
        for (auto density : densities)
        {
            auto testName = "GraphList " + std::to_string(vertNo) + " " + std::to_string(density);
            std::cout << testName << std::endl;

            Test<GraphList<VoidType, unsigned>> test(100, vertNo, density);
            test();
            testNameAndTime.push_back(std::make_tuple(testName, test.getTimes()));
        }
    }

    std::ofstream csv("result.csv");
    csv << "SEP=,\n";
    for (auto& [testName, testTimes] : testNameAndTime)
    {
        csv << testName << ",";
        std::copy(testTimes.begin(), testTimes.end(), std::ostream_iterator<double>(csv, ", "));
        csv << "\n";
    }
}

int main()
{
    const char* prompt = "Option: ";
    const char* menu = "0. Exit\n1. Run tests\n2. Load file\n3. Save Dijkstra output\n";

    std::optional<GraphList<VoidType, unsigned>> graph;
    std::optional<GraphList<std::tuple<unsigned, unsigned>, int>> dijkstraTree;
    unsigned input;

    std::cout << menu << prompt;
    while (std::cin >> input)
    {
        switch(input)
        {
            case 0:
                goto exitProgram;
            case 1:
                runTests();
                break;
            case 2:
            {
                std::cout << "Filename: ";
                std::string filename;
                std::cin >> filename;

                std::ifstream inputFile(filename);
                graph.emplace();
                loadGraph(inputFile, *graph);

                auto result = Dijkstra(*graph, 0);
                dijkstraTree.emplace(getGraphFromDijsktraOutput(std::get<0>(result), std::get<1>(result)));

                break;
            }
            case 3:
            {
                if (!dijkstraTree.has_value())
                {
                    std::cerr << "Error: no file loaded\n";
                    break;
                }

                std::cout << "Filename: ";
                std::string filename;
                std::cin >> filename;

                std::ofstream outFile(filename);
                outFile << "vertex\tcost\tparents\n";
                saveDijkstraTreeInPwrFormat(outFile, *dijkstraTree, 0, nostd::Vector<unsigned>());

                break;
            }
        }
        std::cout << menu << prompt;
    }

    exitProgram:;
}
