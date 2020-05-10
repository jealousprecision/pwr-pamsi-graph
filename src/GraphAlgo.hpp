#pragma once

#include <cstring>

#include <algorithm>
#include <limits>
#include <map>
#include <set>

#include <GraphList.hpp>
#include <GraphMatrix.hpp>

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
        int from = pickMin(idxToCost, traversedVertices);

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

struct VertexWeight
{
    unsigned idx;
    unsigned weight;
};

class MinHeap
{
public:
    MinHeap(unsigned size, unsigned root) :
        heap_(size),
        idxToPositionInHeap_(size),
        perceivedHeapSize_(size)
    {
        constexpr auto infinity = std::numeric_limits<unsigned>::max();

        for (unsigned i = 0; i < size; ++i)
        {
            heap_[i].idx = i;
            heap_[i].weight = infinity;
        }

        for (unsigned i = 0; i < size; ++i)
            idxToPositionInHeap_[i] = i;

        heap_[root].weight = 0;

        swapInHeap_(0, root);
    }

    VertexWeight extract()
    {
        auto result = heap_.front();
        swapInHeap_(0, size() - 1);
        perceivedHeapSize_ -= 1;
        pushDownHeap_(0);
        return result;
    }

    /////////////////////////////////////////////////////////////////

    const auto& getHeap() const { return heap_; }
    const auto& getIdxToPos() const { return idxToPositionInHeap_; }

    unsigned getWeightOfVertex(unsigned vertex) const { return heap_[idxToPositionInHeap_[vertex]].weight; }
    void setWeightOfVertex(unsigned vertex, unsigned weight)
    {
        auto heapPos = idxToPositionInHeap_[vertex];
        heap_[heapPos].weight = weight;

        if (shouldPushUp_(heapPos))
        {
            pushUpHeap_(heapPos);
        }
        else if (shouldPushDown_(heapPos))
        {
            pushDownHeap_(heapPos);
        }
    }

    unsigned size() const { return perceivedHeapSize_; }
    unsigned realSize() const { return heap_.size(); }

protected:
    void swapInHeap_(unsigned i, unsigned j)
    {
        std::swap(idxToPositionInHeap_[heap_[i].idx], idxToPositionInHeap_[heap_[j].idx]);
        std::swap(heap_[i], heap_[j]);
    }

    bool shouldPushUp_(unsigned idx) const
    {
        if (idx != 0 && heap_[idx].weight < heap_[(idx -1) / 2].weight)
            return true;

        return false;
    }

    bool shouldPushDown_(unsigned idx) const
    {
        auto lIdx = 2 * idx + 1;
        auto rIdx = 2 * idx + 2;

        if (lIdx < size() && heap_[idx].weight > heap_[lIdx].weight)
            return true;

        if (rIdx < size() && heap_[idx].weight > heap_[lIdx].weight)
            return true;

        return false;
    }

    void pushDownHeap_(unsigned idx)
    {
        auto smallest = heap_[idx].weight;
        auto smallestIdx = idx;
        auto leftIdx = 2 * idx + 1;
        auto rightIdx = 2 * idx + 2;

        if (leftIdx < size() && heap_[leftIdx].weight < smallest)
        {
            smallest = heap_[leftIdx].weight;
            smallestIdx = leftIdx;
        }

        if (rightIdx < size() && heap_[rightIdx].weight < smallest)
        {
            smallest = heap_[rightIdx].weight;
            smallestIdx = rightIdx;
        }

        if (smallestIdx != idx)
        {
            swapInHeap_(smallestIdx, idx);
            pushDownHeap_(smallestIdx);
        }
    }

    void pushUpHeap_(unsigned idx)
    {
        if (idx == 0)
            return;

        idx = (idx - 1) / 2;  // return to parent
        auto smallest = heap_[idx].weight;
        auto smallestIdx = idx;
        auto leftIdx = 2 * idx + 1;
        auto rightIdx = 2 * idx + 2;

        if (heap_[leftIdx].weight < smallest)
        {
            smallest = heap_[leftIdx].weight;
            smallestIdx = leftIdx;
        }

        if (heap_[rightIdx].weight < smallest)
        {
            smallest = heap_[rightIdx].weight;
            smallestIdx = rightIdx;
        }

        if (smallestIdx != idx)
        {
            swapInHeap_(smallestIdx, idx);
            pushUpHeap_(idx);
        }
    }

    nostd::Vector<VertexWeight> heap_;
    nostd::Vector<unsigned> idxToPositionInHeap_;
    unsigned perceivedHeapSize_;
};

template<typename V, typename E>
auto Dijkstra(GraphList<V, E>& graph, unsigned source) -> std::tuple<nostd::Vector<unsigned>, nostd::Vector<int>> // idxToCost, idxToParent
{
    constexpr auto infinity = std::numeric_limits<unsigned>::max();
    nostd::Vector<int> idxToParent(graph.verticesSize(), -1);
    MinHeap heap(graph.verticesSize(), source);

    while (heap.size() != 0)
    {
        VertexWeight minVert = heap.extract();

        if (minVert.weight == infinity)
            break;  // disconnected graph

        for (auto edge : graph.getEdgesOut(minVert.idx))
        {
            auto destVert = graph.getVertexTo(edge);
            auto newWeight = minVert.weight + graph.getEdge(edge);

            if (newWeight < heap.getWeightOfVertex(destVert))
            {
                heap.setWeightOfVertex(destVert, newWeight);
                idxToParent[destVert] = minVert.idx;
            }
        }
    }

    nostd::Vector<unsigned> idxToCost(graph.verticesSize(), infinity);

    for (unsigned i = 0; i < heap.realSize(); ++i)
        idxToCost[i] = heap.getWeightOfVertex(i);

    return std::make_tuple(std::move(idxToCost), std::move(idxToParent));
}

auto getGraphFromDijsktraOutput(const nostd::Vector<unsigned>& idxToCost, const nostd::Vector<int>& idxToParent)
    -> GraphList<std::tuple<unsigned, unsigned>, int>  // <(idx, cost), placeholder>
{
    GraphList<std::tuple<unsigned, unsigned>, int> graph;

    for (unsigned i = 0; i < idxToCost.size(); ++i)
        graph.addVertex(std::make_tuple(i, idxToCost[i]));

    for (unsigned i = 0; i < idxToParent.size(); ++i)
        if (idxToParent[i] >= 0)
            graph.addEdge(idxToParent[i], i, 0);

    return graph;
}

template<typename V, typename E>
void saveDijkstraTreeInPwrFormat(std::ostream& os, GraphList<V, E>& graph, unsigned source, nostd::Vector<unsigned> parents)
{
    os << std::get<0>(graph.getVertex(source)) << "\t" << std::get<1>(graph.getVertex(source)) << "\t(";
    for (auto parent : parents)
        os << parent << ", ";
    os << ")\n";

    parents.push_back(std::get<0>(graph.getVertex(source)));

    for (auto edge : graph.getEdgesOut(source))
        saveDijkstraTreeInPwrFormat(os, graph, graph.getVertexTo(edge), parents);
}
