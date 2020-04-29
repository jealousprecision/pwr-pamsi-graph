#pragma once

#include <optional>

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

    class Edge;
    class Vertex;

    struct VertexData
    {
    public:
        VertexData(const VertexLabel& obj) :
            data(obj)
        {}

        VertexData(VertexLabel&& obj) :
            data(std::move(obj))
        {}

        VertexData(VertexData&& vert) = default;

        VertexData() = delete;
        VertexData(const VertexData& other) = delete;
        void operator=(const VertexData& other) = delete;
        void operator=(VertexData&& other) = delete;

        unsigned idx;
        VertexLabel data;
    };

    class Vertex
    {
        using VertexDataIter = typename nostd::List<VertexData>::iterator;

    public:
        Vertex(VertexDataIter iter, GraphMatrix& graph) :
            graph_(graph), vertexData_(iter)
        {}

        VertexLabel& operator*() const { return vertexData_-> data; }
        VertexLabel* operator->() const { return vertexData_->data; }

        nostd::List<Edge> edgesOut() const
        {
            nostd::List<Edge> result;
            auto row = vertexData_->idx;
            auto& matrix = graph_.adjacencyMatrix_;

            for (unsigned col = 0; col < matrix.cols(); ++col)
                if (matrix(row, col).has_value())
                    result.push_back(Edge(matrix(row, col).value()));

            return result;
        }

        nostd::List<Edge> edgesIn() const
        {
            nostd::List<Edge> result;
            auto col = vertexData_->idx;
            auto& matrix = graph_.adjacencyMatrix_;

            for (unsigned row = 0; row < matrix.rows(); ++row)
                if (matrix(row, col).has_value())
                    result.push_back(Edge(matrix(row, col).value()));

            return result;
        }

        friend class GraphMatrix<VertexLabel, EdgeLabel>;

    protected:
        GraphMatrix& graph_;
        VertexDataIter vertexData_;
    };

    struct EdgeData
    {
        EdgeData(const EdgeLabel& eLabel) :
            data(eLabel)
        {}

        EdgeData(EdgeLabel&& eLabel) :
            data(std::move(eLabel))
        {}

        EdgeData(EdgeData&& edge) = default;

        EdgeData() = delete;
        EdgeData(const EdgeData& edge) = delete;
        void operator=(const EdgeData& edge) = delete;
        void operator=(EdgeData&& edge) = delete;

        std::tuple<unsigned, unsigned> idx;  // col, row
        EdgeLabel data;
    };

    class Edge
    {
        using EdgeDataIter = typename nostd::List<EdgeData>::iterator;

    public:
        explicit Edge(EdgeDataIter iter, GraphMatrix& graph) :
            graph_(graph), edgeData_(iter)
        {}

        EdgeLabel& operator*() const { return edgeData_->data; }
        EdgeLabel* operator->() const { return &edgeData_->data; }

        Vertex from() const
        {
            return Vertex(std::next(graph_.vertices_.begin(), std::get<0>(edgeData_->idx)), graph_);
        }

        Vertex to() const
        {
            return Vertex(std::next(graph_.vertices_.begin(), std::get<1>(edgeData_->idx)), graph_);
        }

        friend class GraphMatrix<VertexLabel, EdgeLabel>;

    protected:
        GraphMatrix& graph_;
        EdgeDataIter edgeData_;
    };

    Vertex addVertex(const VertexLabel& vLabel)
    {
        vertices_.emplace_back(vLabel);
        return initLastVertex();
    }

    Vertex addVertex(VertexLabel&& vLabel)
    {
        vertices_.emplace_back(std::move(vLabel));
        return initLastVertex();
    }

    Edge addEdge(Vertex from, Vertex to, const EdgeLabel& eLabel)
    {
        edges_.emplace_back(eLabel);
        return initLastEdge(from, to);
    }

    Edge addEdge(Vertex from, Vertex to, EdgeLabel&& eLabel)
    {
        edges_.emplace_back(std::move(eLabel));
        return initLastEdge(from, to);
    }

    nostd::List<Vertex> allVertices()
    {
        // once again, no lazy ranges...
        nostd::List<Vertex> result;

        for (auto it = vertices_.begin(); it != edges_.end(); ++it)
            result.emplace_back(it, *this);

        return result;
    }

    nostd::List<Edge> allEdges()
    {
        nostd::List<Edge> result;

        for (auto it = edges_.begin(); it != edges_.end(); ++it)
            result.emplace_back(it, *this);

        return result;
    }

protected:
    Vertex initLastVertex()
    {
        auto vertIt = std::prev(vertices_.end());
        vertIt->idx = vertices_.size() - 1;
        adjacencyMatrix_.growByOne();
        return Vertex(vertIt, *this);
    }

    Edge initLastEdge(Vertex from, Vertex to)
    {
        auto edgeIt = std::prev(edges_.end());
        adjacencyMatrix_(from.vertexData_->idx, to.vertexData_->idx).emplace(edgeIt);
        edgeIt->idx = std::tie(from.vertexData_->idx, to.vertexData_->idx);
        return Edge(edgeIt, *this);
    }

    using EdgeDataIter = typename nostd::List<EdgeData>::iterator;

    nostd::List<VertexData> vertices_;
    nostd::List<EdgeData> edges_;
    Matrix<std::optional<EdgeDataIter>> adjacencyMatrix_;
};
