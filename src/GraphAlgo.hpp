#pragma once

#include <cstring>

#include <algorithm>
#include <limits>
#include <map>
#include <set>

#include <GraphList.hpp>

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

template<typename Iter, typename Comp>
void push_down(Iter rootIt, size_t size, size_t idx, Comp comp)
{
    auto largest = rootIt;
    auto largestIdx = idx;
    auto end = std::next(rootIt, size);

    auto lIt = 2 * idx + 1 < size ? std::next(rootIt, idx + 1) : end;
    auto rIt = 2 * idx + 2 < size ? std::next(lIt, 1) : end;

    if (lIt != end && comp(*largest, *lIt))
    {
        largest = lIt;
        largestIdx = 2 * idx + 1;
    }

    if (rIt != end && comp(*largest, *rIt))
    {
        largest = rIt;
        largestIdx = 2 * idx + 2;
    }

    if (largestIdx != idx)
    {
        std::swap(*largest, *rootIt);
        push_down(largest, size, largestIdx, comp);
    }
}

template<typename GraphType>
struct VertexWeight
{
    int weight;
    typename GraphType::Vertex vertex;
};

template<typename VertexLabel, typename EdgeLabel>
std::tuple<nostd::Vector<int>, nostd::Vector<int>> // vertex idx to cost, vertex idx to parent idx
Dijkstra(GraphList<VertexLabel, EdgeLabel>& graph, typename GraphList<VertexLabel, EdgeLabel>::Vertex source)
{
    using VertexWeightTyped = VertexWeight<GraphList<VertexLabel, EdgeLabel>>;
    constexpr auto infinity = std::numeric_limits<int>::max();
    auto greaterByWeight = [](const VertexWeightTyped& lhs, const VertexWeightTyped& rhs) { return lhs.weight > rhs.weight; };

    auto vertices = graph.allVertices();
    nostd::Vector<VertexWeightTypes> verticesWeight;

    std::transform(vertices.begin(), vertices.end(), std::back_inserter(verticesWeight),
        [](const auto& vertex)
        {
            return VertexWeightTyped{infinity, vertex};
        });
}
