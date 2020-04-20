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
    srand(time(nullptr));

    GraphList<TestObject, int> graph;

    decltype(graph.addVertex(TestObject())) vert3;
    {
        TestObject obj(36);
        vert3 = graph.addVertex(obj);
    }

    auto vert2 = graph.addVertex(TestObject(24));
    auto vert1 = graph.emplaceVertex(12);

    auto edge = graph.addEdge(*vert1, *vert2, 0);
    auto edge2 = graph.addEdge(*vert1, *vert3, 2);

    for (const decltype(graph)::Edge& edge : vert1->edgesOut)
        std::cout << *edge.from << " --(" << *edge << ")--> " << *edge.to << std::endl;
    std::cout << std::endl;

    graph.removeEdge(*edge);
    for (const decltype(graph)::Edge& edge : vert1->edgesOut)
        std::cout << *edge.from << " --(" << *edge << ")--> " << *edge.to << std::endl;
}
