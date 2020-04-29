#pragma once

#include <nostd/List.hpp>
#include <nostd/Vector.hpp>

#include <Matrix.hpp>

template<typename VertexLabel, typename EdgeLabel>
class GraphMatrix
{
public:
    GraphMatrix() :
        adjacencyMatrix_(0, 0)
    {}

    struct Edge;
    struct Vertex;

    using EdgeIter = typename nostd::List<Edge>::iterator;
    using VertIter = typename nostd::List<Vertex>::iterator;

    struct Vertex
    {
    public:
        Vertex(const VertexLabel& obj) :
            data(obj)
        {}

        Vertex(VertexLabel&& obj) :
            data(std::move(obj))
        {}

        Vertex(Vertex&& vert) = default;

        Vertex() = delete;
        Vertex(const Vertex& other) = delete;
        void operator=(const Vertex& other) = delete;
        void operator=(Vertex&& other) = delete;

        VertexLabel& operator*() { return data; }
        const VertexLabel& operator*() const { return data; }

        nostd::Vector<EdgeIter> edgesOut(const GraphMatrix& graph)
        {
            nostd::Vector<EdgeIter> result;
            for (unsigned i = 0; i < graph.adjacencyMatrix_.cols(); ++i)
                if (graph.adjacencyMatrix_(idx, i) != nullptr)
                    result.push_back(graph.adjacencyMatrix_(idx, i)->position);

            return result;
        }

        nostd::Vector<EdgeIter> edgesIn(const GraphMatrix& graph)
        {
            nostd::Vector<EdgeIter> result;
            for (unsigned i = 0; i < graph.adjacencyMatrix_.rows(); ++i)
                if (graph.adjacencyMatrix_(i, idx) != nullptr)
                    result.push_back(graph.adjacencyMatrix_(i, idx)->position);

            return result;
        }

        unsigned idx;
        VertexLabel data;
    };

    struct Edge
    {
        Edge(const EdgeLabel& eLabel) :
            data(eLabel)
        {}

        Edge(EdgeLabel&& eLabel) :
            data(std::move(eLabel))
        {}

        Edge(Edge&& edge) = default;

        Edge() = delete;
        Edge(const Edge& edge) = delete;
        void operator=(const Edge& edge) = delete;
        void operator=(Edge&& edge) = delete;

        EdgeLabel& operator*() { return data; }
        const EdgeLabel& operator*() const { return data; }

        VertIter source(const GraphMatrix& graph) { return std::next(graph.vertices_.begin(), std::get<0>(idx)); }
        VertIter target(const GraphMatrix& graph) { return std::next(graph.vertices_.begin(), std::get<1>(idx)); }

        std::tuple<unsigned, unsigned> idx;
        EdgeLabel data;
    };

    VertIter addVertex(const VertexLabel& vLabel)
    {
        vertices_.emplace_back(vLabel);
        return initLastVertex();
    }

    VertIter addVertex(VertexLabel&& vLabel)
    {
        vertices_.emplace_back(std::move(vLabel));

        return initLastVertex();
    }

    EdgeIter addEdge(VertIter from, VertIter to, const EdgeLabel& eLabel)
    {
        edges_.emplace_back(eLabel);
        return initLastEdge(from, to);
    }

    EdgeIter addEdge(VertIter from, VertIter to, EdgeLabel&& eLabel)
    {
        edges_.emplace_back(std::move(eLabel));
        return initLastEdge(from, to);
    }

protected:
    VertIter initLastVertex()
    {
        auto vertIt = std::prev(vertices_.end());
        vertIt->idx = vertices_.size() - 1;
        adjacencyMatrix_.growByOne();
        return vertIt;
    }

    EdgeIter initLastEdge(VertIter from, VertIter to)
    {
        auto edgeIt = std::prev(edges_.end());

        adjacencyMatrix_(from->idx, to->idx) = &*edgeIt;

        edgeIt->idx = std::tie(from->idx, to->idx);

        return edgeIt;
    }

    nostd::List<Vertex> vertices_;
    nostd::List<Edge> edges_;
    Matrix<Edge*> adjacencyMatrix_;
};
