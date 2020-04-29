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

int main()
{

    GraphList<std::string, int> graph;

    auto vert1 = graph.addVertex("A");
    auto vert2 = graph.addVertex("B");
    auto vert3 = graph.addVertex("C");

    auto edge = graph.addEdge(vert1, vert2, 42);
    auto edge2 = graph.addEdge(vert2, vert3, 84);
    auto edge3 = graph.addEdge(vert1, vert3, 63);

    for (auto edge : graph.allEdges())
        std::cout << *edge.from() << " -(" << *edge << ")-> " << *edge.to() << std::endl;

    std::cout << std::endl;

    graph.removeVertex(vert2);

    for (auto edge : graph.allEdges())
        std::cout << *edge.from() << " -(" << *edge << ")-> " << *edge.to() << std::endl;

}
