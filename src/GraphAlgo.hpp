#pragma once

#include <cstring>

#include <algorithm>
#include <limits>
#include <map>
#include <set>

#include <GraphList.hpp>
#include <GraphMatrix.hpp>


GraphList<std::tuple<unsigned, int>, int> getGraphFromDijkstraOutput(
    const nostd::Vector<int>& vertexIdxToCost,
    const nostd::Vector<int>& vertexIdxToParentIdx)
{
    /*
    GraphList<std::tuple<unsigned, int>, int> result;

    for (unsigned idx = 0; idx < vertexIdxToCost.size(); ++idx)
        result.addVertex(std::make_tuple(idx, vertexIdxToCost[idx]));

    auto vertices = result.allVertices();

    for (unsigned idx = 0; idx < vertexIdxToParentIdx.size(); ++idx)
        if (vertexIdxToParentIdx[idx] != -1)
            result.addEdge(vertices[vertexIdxToParentIdx[idx]], vertices[idx], 0);

    return result;
    */
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

int pickMin(const nostd::Vector<unsigned>& idxToCost, const nostd::Vector<bool>& traversedVertices)
{
    int min_idx = -1;
    unsigned min_el = std::numeric_limits<unsigned>::max();

    for (unsigned i = 0; i < idxToCost.size(); ++i)
    {
        if (idxToCost[i] < min_el && !traversedVertices[i])
        {
            min_idx = i;
            min_el = idxToCost[i];
        }
    }

    return min_idx;
}

template<typename V, typename E>
auto Dijkstra(GraphMatrix<V, E>& graph, unsigned source) -> std::tuple<nostd::Vector<unsigned>, nostd::Vector<int>> // idxToCost, idxToParent
{
    constexpr auto infinity = std::numeric_limits<unsigned>::max();
    nostd::Vector<unsigned> idxToCost(graph.verticesSize(), infinity);
    nostd::Vector<int> idxToParent(graph.verticesSize(), -1);
    nostd::Vector<bool> traversedVertices(graph.verticesSize(), false);

    idxToCost[source] = 0;

    while (!std::all_of(traversedVertices.begin(), traversedVertices.end(), [](auto el){return el;}))
    {
        auto from = pickMin(idxToCost, traversedVertices);

        if (from == -1)
            break;  // disconnected graph

        traversedVertices[from] = true;
        for (unsigned to = 0; to < graph.verticesSize(); ++to)
        {
            if (auto* edge = graph.getEdgeBetween(from, to))
            {
                auto current = idxToCost[from] + *edge;

                if (current < idxToCost[to])
                {
                    idxToCost[to] = current;
                    idxToParent[to] = from;
                }
            }
        }
    }

    return std::make_tuple(std::move(idxToCost), std::move(idxToParent));
}
