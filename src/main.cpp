#include <cstdlib>
#include <ctime>
#include <cmath>

#include <algorithm>
#include <iterator>

#include <nostd/Vector.hpp>
#include <nostd/List.hpp>
#include <nostd/MergeSort.hpp>
#include <nostd/QuickSort.hpp>
#include <TestObject.hpp>
#include <GraphList.hpp>
#include <GraphMatrix.hpp>

template<typename GraphType>
void logGraph(GraphType& graph)
{
    for (auto edge : graph.allEdges())
        std::cout << *edge.from() << " -(" << *edge << ")-> " << *edge.to() << std::endl;
}

int main()
{
    {
        GraphList<std::string, int> graph;

        auto vert1 = graph.addVertex("A");
        auto vert2 = graph.addVertex("B");
        auto vert3 = graph.addVertex("C");

        auto edge = graph.addEdge(vert1, vert2, 42);
        auto edge2 = graph.addEdge(vert2, vert3, 84);
        auto edge3 = graph.addEdge(vert1, vert3, 63);

        logGraph(graph);

        std::cout << std::endl;

        graph.removeVertex(vert2);

        logGraph(graph);
    }

    std::cout << std::endl;

    {
        GraphMatrix<std::string, int> graph;

        auto vert1 = graph.addVertex("X");
        auto vert2 = graph.addVertex("Y");
        auto vert3 = graph.addVertex("Z");

        auto edge = graph.addEdge(vert1, vert2, 16);
        auto edge2 = graph.addEdge(vert2, vert3, -5);
        auto edge3 = graph.addEdge(vert1, vert3, 420);

        logGraph(graph);


    }
}
