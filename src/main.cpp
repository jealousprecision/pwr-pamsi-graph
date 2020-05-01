#include <cstdlib>
#include <ctime>
#include <cmath>

#include <algorithm>
#include <iterator>
#include <fstream>

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

template<typename GraphType>
void logIntoGraphVizFormat(std::ostream& os, GraphType& graph)
{
    os << "digraph G{\n";
    for (auto edge : graph.allEdges())
        os << *edge.from() << " -> " << *edge.to() << " [label=\"" << *edge << "\"]\n";
    os << "}" << std::endl;
}

template<typename GraphType>
void fillGraph(GraphType& graph)
{
    std::string str = "A";
    for(; str[0] <= 'Z'; str[0]++)
        graph.addVertex(str);

    auto vertices = graph.allVertices();
    /*
    std::random_shuffle(vertices.begin(), vertices.end());
    auto sz = vertices.size() % 2 == 0 ? vertices.size() : vertices.size() - 1;

    for (unsigned i = 0; i < sz; i += 2)
    {
        graph.addEdge(vertices[i], vertices[i+1], i);
        graph.addEdge(vertices[i], vertices[std::rand() % vertices.size()], i + 1);
    }
    */

    int idx = 0;
    for (auto& vertex : vertices)
    {
        for (int i = 0, n = std::rand() % 5; i < n; ++i)
        {
            auto otherVert = vertices[std::rand() % vertices.size()];
            auto possibleEdge = graph.isEdgeBetween(vertex, otherVert);

            if (possibleEdge.has_value())
            {
                i -= 1;
                std::cout << "edge already exist!\n";
                continue;
            }


            graph.addEdge(vertex, otherVert, idx);
            idx += 1;
        }
    }
}

int main()
{
    std::srand(std::time(nullptr));

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

    {
        GraphList<std::string, int> graph;
        fillGraph(graph);
        std::ofstream file("graph.gv");
        logIntoGraphVizFormat(file, graph);
    }

    {
        GraphMatrix<std::string, int> graph;
        fillGraph(graph);
        std::ofstream file("graph2.gv");
        logIntoGraphVizFormat(file, graph);
    }
}
