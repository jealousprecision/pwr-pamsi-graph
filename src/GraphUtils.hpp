#pragma once

#include <cstdlib>

#include <iostream>
#include <string>

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
        os << "\t\"" << *edge.from() << "\" -> \"" << *edge.to() << "\" [label=\"" << *edge << "\"]\n";
    os << "}" << std::endl;
}

template<typename GraphType>
void fillGraph(GraphType& graph)
{
    std::string str = "A";
    for(; str[0] <= 'Z'; str[0]++)
        graph.addVertex(str);

    auto vertices = graph.allVertices();

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

struct VertLabelDefaultTranslator
{
    unsigned operator()(unsigned idx) const
    {
        return idx;
    }
};

template<
    typename GraphType,
    typename IdxToVertexLabelTranslator = VertLabelDefaultTranslator>
void loadGraph(std::istream& is, GraphType& graph, IdxToVertexLabelTranslator translate = VertLabelDefaultTranslator())
{
    unsigned nOfEdges, nOfVertices, treeStartVertex;
    is >> nOfEdges >> nOfVertices >> treeStartVertex;

    for (unsigned idx = 0; idx < nOfVertices; ++idx)
        graph.addVertex(translate(idx));

    auto vertices = graph.allVertices();

    int startVertex, endVertex, weight;
    while (is >> startVertex >> endVertex >> weight)
        graph.addEdge(vertices[startVertex], vertices[endVertex], weight);
}
