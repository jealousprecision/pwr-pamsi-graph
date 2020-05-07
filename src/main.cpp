#include <cstdlib>
#include <ctime>
#include <cmath>

#include <algorithm>
#include <iterator>
#include <fstream>
#include <iostream>
#include <iomanip>

#include <nostd/Vector.hpp>
#include <nostd/List.hpp>
#include <TestObject.hpp>
#include <GraphList.hpp>
#include <GraphMatrix.hpp>
#include <GraphUtils.hpp>
#include <GraphAlgo.hpp>

template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, std::tuple<T1, T2> idxCost)
{
    return os << std::get<0>(idxCost) << ", " << std::get<1>(idxCost);
}

void test()
{
    GraphMatrix<std::string, unsigned> graph;co
    fillGraph(graph);

    auto result = Dijkstra(graph, 0);
}

int main()
{
    std::srand(std::time(nullptr));

    GraphMatrix<VoidType, unsigned> graph;
    //fillGraph(graph);
    std::ifstream input("graph.input");
    loadGraph2Way(input, graph);

    std::ofstream file("graph.gv");
    logIntoGraphVizFormat(file, graph);

    auto result = Dijkstra(graph, 0);

    std::cout << "Vertex\tCost\n";
    for (unsigned vertex = 0; vertex < std::get<0>(result).size(); ++vertex)
        std::cout << vertex << '\t' << std::get<0>(result)[vertex] << '\n';
}
