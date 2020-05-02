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

std::ostream& operator<<(std::ostream& os, std::tuple<unsigned, unsigned> idxCost)
{
    return os << std::get<0>(idxCost) << ", " << std::get<1>(idxCost);
}

int main()
{
    std::srand(std::time(nullptr));

    GraphList<int, int> graph;
    std::ifstream input_file("test_input.txt");
    std::ofstream out_file("graph.gv");

    loadGraph(input_file, graph);
    logIntoGraphVizFormat(out_file, graph);

    auto vertices = graph.allVertices();
    auto result = Dijkstra(graph, vertices[0]);
    std::cout << "Vertex\tDistance from source\n";
    for (unsigned idx = 0; idx < std::get<0>(result).size(); ++idx)
        std::cout << *vertices[idx] << '\t' << std::get<0>(result)[idx] << '\n';

    auto graphDijkstra = getGraphFromDijkstraOutput(std::get<0>(result), std::get<1>(result));
    std::ofstream dijkstraOut("dijkstraOut.gv");
    logIntoGraphVizFormat(dijkstraOut, graphDijkstra);
}
