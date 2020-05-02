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
#include <nostd/DisjointSet.hpp>
#include <TestObject.hpp>
#include <GraphList.hpp>
#include <GraphMatrix.hpp>
#include <GraphUtils.hpp>
#include <GraphAlgo.hpp>

void logDisjointSet(const nostd::DisjointSet& set)
{
    std::cout << std::setw(2);

    for (unsigned i = 0; i < set.size(); ++i)
        std::cout << i << " ";
    std::cout << '\n';

    for (const auto& val : set)
        std::cout << val << " ";
    std::cout << '\n';
}

int main()
{
    std::srand(std::time(nullptr));

    GraphList<int, int> graph;
    std::ifstream input_file("test_input.txt");
    std::ofstream out_file("graph.gv");

    loadGraph(input_file, graph);
    logIntoGraphVizFormat(out_file, graph);

    {
        GraphList<int, int> graph;
        auto v1 = graph.addVertex(0);
        auto v2 = graph.addVertex(1);
        auto v3 = graph.addVertex(2);
        graph.addEdge(v1, v2, 42);
        graph.addEdge(v1, v3, 69);
        graph.addEdge(v2, v3, 80);
        //graph.addEdge(v3, v1, 13);

        std::cout << isCycle(graph.allEdges(), graph.verticesSize()) << std::endl;
    }
}
