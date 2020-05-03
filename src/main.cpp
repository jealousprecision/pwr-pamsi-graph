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

int main()
{
    std::srand(std::time(nullptr));

    GraphList<int, int> graph;
    std::ifstream input_file("test_input.txt");
    std::ofstream out_file("graph.gv");

    loadGraph(input_file, graph);
    logIntoGraphVizFormat(out_file, graph);

    auto vertices = graph.allVertices();
    {
        auto resultDij = Dijkstra(graph, vertices[0]);
        std::cout << "Vertex\tDistance from source\n";
        for (unsigned idx = 0; idx < std::get<0>(resultDij).size(); ++idx)
            std::cout << *vertices[idx] << '\t' << std::get<0>(resultDij)[idx] << '\n';

        auto graphDijkstra = getGraphFromDijkstraOutput(std::get<0>(resultDij), std::get<1>(resultDij));
        std::ofstream dijkstraOut("dijkstraOut.gv");
        logIntoGraphVizFormat(dijkstraOut, graphDijkstra);
    }

    std::cout << std::endl;

    {
        auto resultBell = BellmanFord(graph, vertices[0]);
        for (unsigned idx = 0; idx < std::get<0>(resultBell).size(); ++idx)
            std::cout << *vertices[idx] << '\t' << std::get<0>(resultBell)[idx] << '\n';

        std::cout << "negative weight-cycles: " << std::get<2>(resultBell) << std::endl;

        auto graphBell = getGraphFromDijkstraOutput(std::get<0>(resultBell), std::get<1>(resultBell));
        std::ofstream bellOut("bellOut.gv");
        logIntoGraphVizFormat(bellOut, graphBell);
    }
}
