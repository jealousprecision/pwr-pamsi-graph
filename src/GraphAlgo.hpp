#pragma once

#include <algorithm>
#include <nostd/DisjointSet.hpp>

template<typename Edges>
bool isCycle(const Edges& edges, unsigned nOfVertices)
{
    nostd::DisjointSet dset(nOfVertices);

    for (auto edge : edges)
        if (!dset.unionize(edge.from().getIdx(), edge.to().getIdx()))
            return true;

    return false;
}

template<typename GraphType>
bool isCycleDFS(const GraphType& graph)
{
    if (graph.verticesSize() == 0)
        return false;

    nostd::Vector<bool> traversedVertices(graph.verticesSize(), false);
    nostd::Vector<GraphType::Vertex> vertexStack;
    vertexStack.push_back(*graph.allVertices().begin());

    while (vertexStack.size() != 0)
    {
        auto vertex = vertexStack.back();
        vertexStack.pop_back();
        traversedVertices[vertex.getIdx()] = true;

        for (auto edge : vertex.outEdges())
            if (traversedVertices[edge.to().getIdx()])
                return false;  // link to edge already traversed, cycle
            else
                vertexStack.push_back(edge.to());
    }
}

template<typename GraphType>
GraphType KruskalMST(GraphType& graph)
{
    auto edges = graph.allEdges();

    std::sort(edges.begin(), edges.end(), [](const auto& el){return el;});



}
