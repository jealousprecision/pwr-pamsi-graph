#pragma once

#include <cstdlib>

#include <iostream>
#include <string>

template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const std::tuple<T1, T2>& var)
{
    return os << std::get<0>(var) << ", " << std::get<1>(var);
}

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

constexpr auto MAXIMUM_WEIGHT = 5000;
constexpr auto EPSILON = 0.0001;

bool isCloseTo(double wanted, double val)
{
    return wanted - EPSILON < val && val < wanted + EPSILON;
}

template<typename E>
void fillGraph(GraphMatrix<VoidType, E>& graph, double graphDensity)
{
    if (isCloseTo(1.0, graphDensity))
    {
        fillGraphFull(graph);
        return;
    }

    auto vertices = graph.verticesSize();
    unsigned nOfEdges = graphDensity * vertices * (vertices - 1);

    for (auto i = 0u; i < nOfEdges; ++i)
    {
        unsigned src, dest;
        do {
            src = std::rand() % vertices;
            dest = std::rand() % vertices;
        } while(graph.isEdgeBetween(src, dest));

        graph.addEdge(src, dest, std::rand() % MAXIMUM_WEIGHT);
    }
}

template<typename E>
void fillGraph(GraphList<VoidType, E>& graph, double graphDensity)
{
    if (isCloseTo(1.0, graphDensity))
    {
        fillGraphFull(graph);
        return;
    }

    auto vertices = graph.verticesSize();
    unsigned nOfEdges = graphDensity * vertices * (vertices - 1);

    Matrix<bool> adjacencyMatrix(vertices, vertices);
    adjacencyMatrix.fill(false);

    for (auto i = 0u; i < nOfEdges; ++i)
    {
        unsigned src, dest;
        do {
            src = std::rand() % vertices;
            dest = std::rand() % vertices;
        } while(adjacencyMatrix(src, dest));

        adjacencyMatrix(src, dest) = true;
        graph.addEdge(src, dest, std::rand() % MAXIMUM_WEIGHT);
    }
}

template<typename GraphType>
void fillGraphFull(GraphType& graph)
{
    for (auto src = 0u; src < graph.verticesSize(); ++src)
        for (auto dest = 0u; dest < graph.verticesSize(); ++dest)
            if (src != dest)
                graph.addEdge(src, dest, std::rand() % MAXIMUM_WEIGHT);
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
