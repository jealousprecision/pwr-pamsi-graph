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

void testMatrix(const std::string& outputName)
{
    GraphMatrix<VoidType, unsigned> graph;
    std::ifstream input("graph.input");
    loadGraph2Way(input, graph);

    std::ofstream file(outputName);
    logIntoGraphVizFormat(file, graph);
    file.close();

    auto result = Dijkstra(graph, 0);

    std::cout << "Vertex\tCost\n";
    for (unsigned vertex = 0; vertex < std::get<0>(result).size(); ++vertex)
        std::cout << vertex << '\t' << std::get<0>(result)[vertex] << '\n';

    auto dijsktraTree = getGraphFromDijsktraOutput(std::get<0>(result), std::get<1>(result));
    file.open("tree.gv");
    logIntoGraphVizFormat(file, dijsktraTree);
    file.close();
}

int main()
{
    {
        GraphMatrix<VoidType, unsigned> graph(1000);
        fillGraph(graph, 0.5);

        auto start = std::chrono::steady_clock::now();
        Dijkstra(graph, 0);
        auto end = std::chrono::steady_clock::now();

        using std::cout, std::endl;

        cout << "Elapsed time in milliseconds : "
            << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0
            << " ms" << endl;
    }
}
