#pragma once

#include <functional>
#include <iterator>

#include <nostd/List.hpp>
#include <nostd/Vector.hpp>

template<typename VertexLabel, typename EdgeLabel>
class GraphList
{
protected:
    enum class UsePerfectForward
    {
        True,
    };

public:
    struct Edge;
    struct Vertex;

    using EdgeIter = typename nostd::List<Edge>::iterator;
    using VertIter = typename nostd::List<Vertex>::iterator;

    struct Vertex
    {
        Vertex(const VertexLabel& vLabel) :
            data(vLabel)
        {}

        Vertex(VertexLabel&& vLabel) :
            data(std::move(vLabel))
        {}

        template<typename... Args>
        Vertex(UsePerfectForward dummy, Args&&... args) :
            data(std::forward<Args>(args)...)
        {}

        Vertex(Vertex&& vert) = default;

        Vertex() = delete;
        Vertex(const Vertex& other) = delete;
        void operator=(const Vertex& other) = delete;
        void operator=(Vertex&& other) = delete;

        VertexLabel& operator*() { return data; }
        const VertexLabel& operator*() const { return data; }

        using EdgeRef = std::reference_wrapper<Edge>;
        nostd::List<EdgeRef> edgesOut;
        nostd::List<EdgeRef> edgesIn;

        VertIter position;
        VertexLabel data;
    };

    struct Edge
    {
        Edge(Vertex& From, Vertex& To, const EdgeLabel& eLabel) :
            from(From), to(To), data(eLabel)
        {}

        Edge(Vertex& From, Vertex& To, EdgeLabel&& eLabel) :
            from(From), to(To), data(std::move(eLabel))
        {}

        template<typename... Args>
        Edge(UsePerfectForward, Vertex& From, Vertex& To, Args&&... args) :
            from(From), to(To), data(std::forward<Args>(args)...)
        {}

        Edge(Edge&& edge) = default;

        Edge() = delete;
        Edge(const Edge& edge) = delete;
        void operator=(const Edge& edge) = delete;
        void operator=(Edge&& edge) = delete;

        EdgeLabel& operator*() { return data; }
        const EdgeLabel& operator*() const { return data; }

        Vertex& from;
        Vertex& to;

        EdgeIter position;
        EdgeLabel data;
    };

    VertIter addVertex(const VertexLabel& vLabel)
    {
        vertices_.push_back(Vertex(vLabel));
        return initLastVertex_();
    }

    VertIter addVertex(VertexLabel&& vLabel)
    {
        vertices_.push_back(Vertex(std::move(vLabel)));
        return initLastVertex_();
    }

    template<typename... Args>
    VertIter emplaceVertex(Args&&... args)
    {
        vertices_.emplace_back(UsePerfectForward::True, std::forward<Args>(args)...);
        return initLastVertex_();
    }

    EdgeIter addEdge(Vertex& from, Vertex& to, const EdgeLabel& eLabel)
    {
        edges_.push_back(Edge(from, to, eLabel));
        return initLastEdge_();
    }

    EdgeIter addEdge(Vertex& from, Vertex& to, EdgeLabel&& eLabel)
    {
        edges_.push_back(Edge(from, to, std::move(eLabel)));
        return initLastEdge_();
    }

    template<typename... Args>
    EdgeIter emplaceEdge(Vertex& from, Vertex& to, Args&&... args)
    {
        edges_.emplace_back(UsePerfectForward::True, from, to, std::forward<Args>(args)...);
        return initLastEdge_();
    }

    void removeEdge(const Edge& edge)
    {
        edge.from.edgesOut.remove_if([&](const Edge& el){ return (&el) == (&edge); });
        edge.to.edgesIn.remove_if([&](const Edge& el){ return (&el) == (&edge); });
    }

protected:
    auto initLastVertex_()
    {
        auto lastVertex = std::prev(vertices_.end());
        lastVertex->position = lastVertex;
        return lastVertex;
    }

    auto initLastEdge_()
    {
        auto lastEdge = std::prev(edges_.end());

        lastEdge->position = lastEdge;
        lastEdge->from.edgesOut.push_back(*lastEdge);
        lastEdge->to.edgesIn.push_back(*lastEdge);

        return lastEdge;
    }

    nostd::List<Vertex> vertices_;
    nostd::List<Edge> edges_;
};
