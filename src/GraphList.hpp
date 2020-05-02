#pragma once

#include <algorithm>
#include <functional>
#include <iterator>

#include <nostd/List.hpp>
#include <nostd/Vector.hpp>

template<typename VertexLabel, typename EdgeLabel>
class GraphList
{
public:
    struct EdgeData;
    struct VertexData;

    class Vertex;
    class Edge;

    struct VertexData
    {
        VertexData(const VertexLabel& vLabel) :
            data(vLabel)
        {}

        VertexData(VertexLabel&& vLabel) :
            data(std::move(vLabel))
        {}

        VertexData(VertexData&& vert) = default;

        VertexData() = delete;
        VertexData(const VertexData& other) = delete;
        void operator=(const VertexData& other) = delete;
        void operator=(VertexData&& other) = delete;

        /////////////////////////////////////////////////////

        unsigned idx;
        nostd::List<Edge> edgesOut;
        nostd::List<Edge> edgesIn;
        VertexLabel data;
    };

    class Vertex
    {
        using VertexDataIter = typename nostd::List<VertexData>::iterator;

    public:
        explicit Vertex(VertexDataIter iter) : vertexData_(iter) {}

        VertexLabel& operator*() const { return vertexData_->data; }
        VertexLabel* operator->() const { return &vertexData_->data; }
        bool operator==(const Vertex& other) const { return vertexData_ == other.vertexData_; }
        bool operator!=(const Vertex& other) const { return vertexData_ != other.vertexData_; }

        const nostd::List<Edge>& edgesOut() const { return vertexData_->edgesOut; }
        const nostd::List<Edge>& edgesIn() const { return vertexData_->edgesIn; }
        unsigned getIdx() const { return vertexData_->idx; }

        friend class GraphList<VertexLabel, EdgeLabel>;

    protected:
        VertexDataIter vertexData_;
    };

    struct EdgeData
    {
        EdgeData(Vertex _from, Vertex _to, const EdgeLabel& eLabel) :
            from(_from), to(_to), data(eLabel)
        {}

        EdgeData(Vertex _from, Vertex _to, EdgeLabel&& eLabel) :
            from(_from), to(_to), data(std::move(eLabel))
        {}

        EdgeData(EdgeData&& edge) = default;

        EdgeData() = delete;
        EdgeData(const Edge& edge) = delete;
        void operator=(const Edge& edge) = delete;
        void operator=(Edge&& edge) = delete;

        ///////////////////////////////////////////////

        Vertex from;
        Vertex to;
        EdgeLabel data;
    };

    class Edge
    {
        using EdgeDataIter = typename nostd::List<EdgeData>::iterator;

    public:
        explicit Edge(EdgeDataIter iter) : edgeData_(iter) {}

        EdgeLabel& operator*() const { return edgeData_->data; }
        EdgeLabel* operator->() const { return &edgeData_->data; }

        Vertex from() const { return edgeData_->from; }
        Vertex to() const { return edgeData_->to; }

        friend class GraphList<VertexLabel, EdgeLabel>;

    protected:
        EdgeDataIter edgeData_;
    };

    Vertex addVertex(const VertexLabel& vLabel)
    {
        vertices_.emplace_back(vLabel);
        return initLastVertex_();
    }

    Vertex addVertex(VertexLabel&& vLabel)
    {
        vertices_.emplace_back(std::move(vLabel));
        return initLastVertex_();
    }

    Edge addEdge(Vertex from, Vertex to, const EdgeLabel& eLabel)
    {
        edges_.emplace_back(from, to, eLabel);
        return initLastEdge_();
    }

    Edge addEdge(Vertex from, Vertex to, EdgeLabel&& eLabel)
    {
        edges_.emplace_back(from, to, std::move(eLabel));
        return initLastEdge_();
    }

    void removeEdge(Edge edge)
    {
        auto edgePointsToSameObj = [&](const auto& otherEdge) { return edge.edgeData_ == otherEdge.edgeData_; };
        edge.from().vertexData_->edgesOut.remove_first(edgePointsToSameObj);
        edge.to().vertexData_->edgesIn.remove_first(edgePointsToSameObj);

        edges_.erase(edge.edgeData_);
    }

    void removeVertex(Vertex vertex)
    {
        nostd::Vector<Edge> edgesToRemove;
        std::copy(vertex.edgesIn().begin(), vertex.edgesIn().end(), std::back_inserter(edgesToRemove));
        std::copy(vertex.edgesOut().begin(), vertex.edgesOut().end(), std::back_inserter(edgesToRemove));

        for (auto edge : edgesToRemove)
            removeEdge(edge);

        vertices_.erase(vertex.vertexData_);
    }

    nostd::Vector<Vertex> allVertices()
    {
        // c++ doesn't have transform iterator or lazy ranges, sooooo
        nostd::Vector<Vertex> result;

        for (auto it = vertices_.begin(); it != vertices_.end(); ++it)
            result.emplace_back(it);

        return result;
    }

    unsigned verticesSize()
    {
        return vertices_.size();
    }

    nostd::Vector<Edge> allEdges()
    {
        nostd::Vector<Edge> result;

        for (auto it = edges_.begin(); it != edges_.end(); ++it)
            result.emplace_back(it);

        return result;
    }

    unsigned edgesSize()
    {
        return edges_.size();
    }

    std::optional<Edge> isEdgeBetween(Vertex from, Vertex to)
    {
        auto found =  std::find_if(from.edgesOut().begin(), from.edgesOut().end(),
            [&](const auto& edge)
            {
                return edge.to() == to;
            });

        if (found == from.edgesOut().end())
            return std::nullopt;

        return *found;
    }

protected:
    Vertex initLastVertex_()
    {
        auto lastVertex = std::prev(vertices_.end());
        lastVertex->idx = vertices_.size() - 1;
        return Vertex{lastVertex};
    }

    Edge initLastEdge_()
    {
        auto lastEdge = std::prev(edges_.end());

        lastEdge->from.vertexData_->edgesOut.push_back(Edge{lastEdge});
        lastEdge->to.vertexData_->edgesIn.push_back(Edge{lastEdge});

        return Edge{lastEdge};
    }

    nostd::List<VertexData> vertices_;
    nostd::List<EdgeData> edges_;
};
