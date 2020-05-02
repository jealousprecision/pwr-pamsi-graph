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

int getMinIdx(const nostd::Vector<unsigned>& idxToCost, bool traversedVertices[])
{
    unsigned minVal = std::numeric_limits<unsigned>::max();
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
std::tuple<nostd::Vector<unsigned>, nostd::Vector<int>>  // vertex idx to cost, vertex idx to parent idx
Dijkstra(GraphType& graph, typename GraphType::Vertex source)
{
    constexpr auto infinity = std::numeric_limits<unsigned>::max();
    auto vertices = graph.allVertices();

    nostd::Vector<unsigned> idxToCost(vertices.size(), infinity);
    nostd::Vector<int> vertexIdxToParentIdx(vertices.size(), -1);

    bool traversedVertices[vertices.size()];
    std::memset(traversedVertices, false, sizeof(bool) * vertices.size());

    idxToCost[source.getIdx()] = 0;

    while (!all_of(traversedVertices, traversedVertices + vertices.size()))
    {
        auto minIdx = getMinIdx(idxToCost, traversedVertices);

        if (minIdx == -1)  // disconnected graph
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

    //return {idxToCost, vertexIdxToParentIdx};
    return std::make_tuple(idxToCost, vertexIdxToParentIdx);
}

GraphList<std::tuple<unsigned, unsigned>, int> getGraphFromDijkstraOutput(
    const nostd::Vector<unsigned>& vertexIdxToCost,
    const nostd::Vector<int>& vertexIdxToParentIdx)
{
    GraphList<std::tuple<unsigned, unsigned>, int> result;

    for (unsigned idx = 0; idx < vertexIdxToCost.size(); ++idx)
        result.addVertex(std::make_tuple(idx, vertexIdxToCost[idx]));

    auto vertices = result.allVertices();

    for (unsigned idx = 0; idx < vertexIdxToParentIdx.size(); ++idx)
        if (vertexIdxToParentIdx[idx] != -1)
            result.addEdge(vertices[vertexIdxToParentIdx[idx]], vertices[idx], 0);

    return result;
}

template<typename GraphType>
std::tuple<nostd::Vector<unsigned>, nostd::Vector<int>>  // vertex idx to cost, vertex idx to parent idx
BellmanFord(GraphType& graph, typename GraphType::Vertex source)
{
    constexpr auto infinity = std::numeric_limits<unsigned>::max();
    auto vertices = graph.allVertices();

    nostd::Vector<unsigned> idxToCost(vertices.size(), infinity);

    idxToCost[source.getIdx()] = 0;

    for (unsigned idx = 1; idx < vertices.size(); ++idx)
    {

    }

    throw std::runtime_error("Not implemented yet");
}
