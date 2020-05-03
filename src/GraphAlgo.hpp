#pragma once

#include <cstring>

#include <algorithm>
#include <limits>
#include <map>
#include <set>

#include <nostd/DisjointSet.hpp>

template<typename GraphType>
struct VertexWeight
{
    unsigned weight;
    typename GraphType::Vertex vertex;
};

int getMinIdx(const nostd::Vector<int>& idxToCost, bool traversedVertices[])
{
    int minVal = std::numeric_limits<int>::max();
    int minIdx = -1;

    for (unsigned i = 0; i < idxToCost.size(); ++i)
    {
        if (idxToCost[i] < minVal && !traversedVertices[i])
        {
            minVal = idxToCost[i];
            minIdx = i;
        }
    }

    return minIdx;
}

bool all_of(bool* begin, bool *end)
{
    while (begin != end)
        if (!(*begin++))
            return false;

    return true;
}

template<typename GraphType>
std::tuple<nostd::Vector<int>, nostd::Vector<int>>  // vertex idx to cost, vertex idx to parent idx
Dijkstra(GraphType& graph, typename GraphType::Vertex source)
{
    constexpr auto infinity = std::numeric_limits<int>::max();
    auto vertices = graph.allVertices();

    nostd::Vector<int> idxToCost(vertices.size(), infinity);
    nostd::Vector<int> vertexIdxToParentIdx(vertices.size(), -1);

    bool traversedVertices[vertices.size()];
    std::memset(traversedVertices, false, sizeof(bool) * vertices.size());

    idxToCost[source.getIdx()] = 0;

    while (!all_of(traversedVertices, traversedVertices + vertices.size()))
    {
        auto minIdx = getMinIdx(idxToCost, traversedVertices);

        if (minIdx == -1)  // disconnected graph, minIdx not found
            break;

        for (auto edge : vertices[minIdx].edgesOut())
        {
            if (traversedVertices[edge.to().getIdx()])
                continue;

            auto current = idxToCost[minIdx] + *edge;
            auto previousBest = idxToCost[edge.to().getIdx()];

            if (current < previousBest)
            {
                idxToCost[edge.to().getIdx()] = current;
                vertexIdxToParentIdx[edge.to().getIdx()] = edge.from().getIdx();
            }
        }

        traversedVertices[minIdx] = true;
    }

    return std::make_tuple(std::move(idxToCost), std::move(vertexIdxToParentIdx));
}

GraphList<std::tuple<unsigned, int>, int> getGraphFromDijkstraOutput(
    const nostd::Vector<int>& vertexIdxToCost,
    const nostd::Vector<int>& vertexIdxToParentIdx)
{
    GraphList<std::tuple<unsigned, int>, int> result;

    for (unsigned idx = 0; idx < vertexIdxToCost.size(); ++idx)
        result.addVertex(std::make_tuple(idx, vertexIdxToCost[idx]));

    auto vertices = result.allVertices();

    for (unsigned idx = 0; idx < vertexIdxToParentIdx.size(); ++idx)
        if (vertexIdxToParentIdx[idx] != -1)
            result.addEdge(vertices[vertexIdxToParentIdx[idx]], vertices[idx], 0);

    return result;
}

template<typename GraphType>
std::tuple<nostd::Vector<int>, nostd::Vector<int>, bool>  // vertex idx to cost, vertex idx to parent idx, does graph contain negative edge
BellmanFord(GraphType& graph, typename GraphType::Vertex source)
{
    constexpr auto infinity = std::numeric_limits<int>::max();
    auto vertices = graph.allVertices();

    nostd::Vector<int> idxToCost(vertices.size(), infinity);
    nostd::Vector<int> idxToParent(vertices.size(), -1);

    idxToCost[source.getIdx()] = 0;

    auto edges = graph.allEdges();
    std::random_shuffle(edges.begin(), edges.end());
    for (unsigned i = 0; i <  graph.verticesSize() - 1; ++i)
    {
        for (unsigned j = 0; j < graph.edgesSize(); ++j)
        {
            auto& edge = edges[j];

            if (idxToCost[edge.from().getIdx()] == infinity)
                continue;

            auto previous = idxToCost[edge.to().getIdx()];
            auto current = idxToCost[edge.from().getIdx()] + *edge;
            if (current < previous)
            {
                idxToCost[edge.to().getIdx()] = current;
                idxToParent[edge.to().getIdx()] = edge.from().getIdx();
            }
        }
        std::cout << "Bellman iteration " << i << ": ";
        for (auto weight : idxToCost)
            std::cout << weight << ", ";
        std::cout << std::endl;
    }

    // check for negative weights
    bool negativeWeights = false;
    for (auto edge : edges)
    {
        if (*edge < 0)
            std::cout << "flag1" << std::endl;

        auto previous = idxToCost[edge.to().getIdx()];
        auto current = idxToCost[edge.from().getIdx()] + *edge;
        if (previous > current)
        {
            negativeWeights = true;
            break;
        }
    }

    return std::make_tuple(std::move(idxToCost), std::move(idxToParent), negativeWeights);
}
