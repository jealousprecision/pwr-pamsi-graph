#pragma once

#include <cstdlib>

#include <iostream>
#include <string>

template<typename V, typename E>
void logGraph(GraphMatrix<V, E>& graph)
{
    for (unsigned from = 0; from < graph.verticesSize(); ++from)
        for (unsigned to = 0; to < graph.verticesSize(); ++to)
            if (auto* edge = graph.getEdgeBetween(from, to))
                std::cout << graph.getVertex(from) << " -("
                    << *edge << ")-> " << graph.getVertex(to) << std::endl;
}

template<typename V, typename E>
void logGraph(GraphList<V, E>& graph)
{
    for (unsigned vertex = 0, n = graph.verticesSize(); vertex < n; ++vertex)
        for (auto edge : graph.getEdgesOut(vertex))
            std::cout << graph.getVertex(vertex) << " -("
                << graph.getEdge(edge) << ")-> " << graph.getVertex(graph.getVertexTo(edge)) << std::endl;
}

template<typename V, typename E>
void logIntoGraphVizFormat(std::ostream& os, GraphMatrix<V, E>& graph)
{
    os << "digraph G {\n";

    for (unsigned from = 0; from < graph.verticesSize(); ++from)
        for (unsigned to = 0; to < graph.verticesSize(); ++to)
            if (auto* edge = graph.getEdgeBetween(from, to))
                os << "\t\""
                    << graph.getVertex(from) << "\" -> \""
                    << graph.getVertex(to) << "\""
                    << " [label=\"" << *edge << "\"]\n";

    os << "}" << std::endl;
}

template<typename V, typename E>
void logIntoGraphVizFormat(std::ostream& os, GraphList<V, E>& graph)
{
    os << "digraph G{\n";

    for (unsigned vertex = 0, n = graph.verticesSize(); vertex < n; ++vertex)
        for (auto edge : graph.getEdgesOut(vertex))
            os << "\t\""
                << graph.getVertex(vertex) << "\" -> \""
                << graph.getVertex(graph.getVertexTo(edge)) << "\" "
                << "[label=\"" << graph.getEdge(edge) << "\"]\n";

    os << "}" << std::endl;
}

template<typename V, typename E>
void fillGraph(GraphMatrix<V, E>& graph)
{
    std::string s("A");
    for (; s[0] <= 'Z'; s[0]++)
        graph.addVertex(s);

    for (unsigned from = 0; from < graph.verticesSize(); ++from )
    {
        for (unsigned i = 0, n = std::rand() % 5 + 1; i < n; ++i)
        {
            unsigned to;
            do {
                to = std::rand() % graph.verticesSize();
            } while (graph.getEdgeBetween(from, to));

            graph.addEdge(from, to, std::rand() % std::numeric_limits<uint16_t>::max());
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

    int startVertex, endVertex, weight;
    while (is >> startVertex >> endVertex >> weight)
        graph.addEdge(startVertex, endVertex, weight);
}

template<
    typename GraphType,
    typename IdxToVertexLabelTranslator = VertLabelDefaultTranslator>
void loadGraph2Way(std::istream& is, GraphType& graph, IdxToVertexLabelTranslator translate = VertLabelDefaultTranslator())
{
    unsigned nOfEdges, nOfVertices, treeStartVertex;
    is >> nOfEdges >> nOfVertices >> treeStartVertex;

    for (unsigned idx = 0; idx < nOfVertices; ++idx)
        graph.addVertex(translate(idx));

    int startVertex, endVertex, weight;
    while (is >> startVertex >> endVertex >> weight)
    {
        graph.addEdge(startVertex, endVertex, weight);
        graph.addEdge(endVertex, startVertex, weight);
    }
}
