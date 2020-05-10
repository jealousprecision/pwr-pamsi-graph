#pragma once

#include <algorithm>

#include <nostd/Vector.hpp>
#include <VoidType.hpp>

template<typename VertexLabel, typename EdgeLabel>
class GraphList
{
public:
    struct Vertex
    {
    public:
        Vertex(const VertexLabel data) :
            data_(data)
        {}

        friend class GraphList<VertexLabel, EdgeLabel>;

    protected:
        nostd::Vector<unsigned> edgesOut_;
        nostd::Vector<unsigned> edgesIn_;
        VertexLabel data_;
    };

    struct Edge
    {
    public:
        Edge(unsigned from, unsigned to, const EdgeLabel& data) :
            from_(from),
            to_(to),
            data_(data)
        {}

        friend class GraphList<VertexLabel, EdgeLabel>;

    protected:
        unsigned from_, to_;
        EdgeLabel data_;
    };

    /////////////////////////////////////////////////////////////////

    unsigned addVertex(const VertexLabel& vLabel)
    {
        vertices_.emplace_back(vLabel);
        return vertices_.size() - 1;
    }

    unsigned addEdge(unsigned from, unsigned to, const EdgeLabel& edge)
    {
        edges_.emplace_back(from, to, edge);
        return edges_.size() - 1;
    }

    /////////////////////////////////////////////////////////////////

    const VertexLabel& getVertex(unsigned vertex) const
    {
        return vertices_[vertex].data_;
    }

    const EdgeLabel& getEdge(unsigned edge) const
    {
        return edges_[edge].data_;
    }

    /////////////////////////////////////////////////////////////////

    const nostd::Vector<unsigned>& getEdgesOut(unsigned vertex) const
    {
        return vertices_[vertex].edgesOut_;
    }

    const nostd::Vector<unsigned>& getEdgesIn(unsigned vertex) const
    {
        return vertices_[vertex].edgesIn_;
    }

    unsigned getVertexFrom(unsigned edge) const
    {
        return edges_[edge].from_;
    }

    unsigned getVertexTo(unsigned edge) const
    {
        return edges_[edge].to_;
    }

    bool isEdgeBetween(unsigned srcVert, unsigned destVert)
    {
        const auto& edgesOut = vertices_[srcVert].edgesOut_;

        return std::any_of(edgesOut.begin(), edgesOut.end(),
            [&](auto edge)
            {
                return this->getVertexTo(edge) == destVert;
            });
    }

protected:
    nostd::Vector<Vertex> vertices_;
    nostd::Vector<Edge> edges_;
};

template<typename EdgeLabel>
class GraphList<VoidType, EdgeLabel>
{
public:
    struct Vertex
    {
    public:
        friend class GraphList<VoidType, EdgeLabel>;

    protected:
        nostd::Vector<unsigned> edgesOut_;
        nostd::Vector<unsigned> edgesIn_;
    };

    struct Edge
    {
    public:
        Edge(unsigned from, unsigned to, const EdgeLabel& data) :
            from_(from),
            to_(to),
            data_(data)
        {}

        friend class GraphList<VoidType, EdgeLabel>;

    protected:
        unsigned from_, to_;
        EdgeLabel data_;
    };

    /////////////////////////////////////////////////////////////////

    GraphList() = default;
    GraphList(unsigned vertices) :
        vertices_(vertices)
    {}


    unsigned addVertex()
    {
        vertices_.emplace_back();
        return vertices_.size() - 1;
    }

    template<typename T>
    unsigned addVertex(T&&)
    {
        return addVertex();
    }

    unsigned addEdge(unsigned from, unsigned to, const EdgeLabel& edge)
    {
        edges_.emplace_back(from, to, edge);

        auto idx = edges_.size() - 1;
        vertices_[from].edgesOut_.push_back(idx);
        vertices_[to].edgesIn_.push_back(idx);

        return idx;
    }

    /////////////////////////////////////////////////////////////////

    unsigned getVertex(unsigned vertex) const
    {
        return vertex;
    }

    const EdgeLabel& getEdge(unsigned edge) const
    {
        return edges_[edge].data_;
    }

    /////////////////////////////////////////////////////////////////

    const nostd::Vector<unsigned>& getEdgesOut(unsigned vertex) const
    {
        return vertices_[vertex].edgesOut_;
    }

    const nostd::Vector<unsigned>& getEdgesIn(unsigned vertex) const
    {
        return vertices_[vertex].edgesIn_;
    }

    unsigned getVertexFrom(unsigned edge) const
    {
        return edges_[edge].from_;
    }

    unsigned getVertexTo(unsigned edge) const
    {
        return edges_[edge].to_;
    }

    bool isEdgeBetween(unsigned srcVert, unsigned destVert)
    {
        const auto& edgesOut = vertices_[srcVert].edgesOut_;

        return std::any_of(edgesOut.begin(), edgesOut.end(),
            [&](auto edge)
            {
                return getVertexTo(edge) == destVert;
            });
    }

    /////////////////////////////////////////////////////////////////

    unsigned verticesSize() const
    {
        return vertices_.size();
    }

protected:
    nostd::Vector<Vertex> vertices_;
    nostd::Vector<Edge> edges_;
};
