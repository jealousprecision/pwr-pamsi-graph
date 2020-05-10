#pragma once

#include <nostd/List.hpp>
#include <nostd/Vector.hpp>

#include <Matrix.hpp>
#include <VoidType.hpp>

template<typename VertexLabel, typename EdgeLabel>
class GraphMatrix
{
public:
    GraphMatrix() :
        adjacencyMatrix_(0, 0)
    {}

    unsigned addVertex(const VertexLabel& vLabel)
    {
        vertices_.push_back(vLabel);
        adjacencyMatrix_.growByOne(nullptr);
        return vertices_.size() - 1;
    }

    void addEdge(unsigned from, unsigned to, const EdgeLabel& eLabel)
    {
        edges_.emplace_back(eLabel);
        adjacencyMatrix_(from, to) = &edges_.back();
    }

    unsigned verticesSize() const
    {
        return vertices_.size();
    }

    const EdgeLabel* getEdgeBetween(unsigned from, unsigned to) const
    {
        return adjacencyMatrix_(from, to);
    }

    const VertexLabel& getVertex(unsigned id) const
    {
        return vertices_[id];
    }

    bool isEdgeBetween(unsigned srcVert, unsigned destVert)
    {
        return adjacencyMatrix_(srcVert, destVert);
    }

protected:
    nostd::Vector<VertexLabel> vertices_;
    nostd::List<EdgeLabel> edges_;
    Matrix<EdgeLabel*> adjacencyMatrix_;
};

template<typename EdgeLabel>
class GraphMatrix<VoidType, EdgeLabel>
{
public:
    GraphMatrix() :
        adjacencyMatrix_(0, 0)
    {}

    GraphMatrix(unsigned vertices) :
        adjacencyMatrix_(vertices, vertices)
    {}

    unsigned addVertex()
    {
        adjacencyMatrix_.growByOne(nullptr);
        return adjacencyMatrix_.cols();
    }

    template<typename T>
    unsigned addVertex(T&&)
    {
        return addVertex();
    }

    void addEdge(unsigned from, unsigned to, const EdgeLabel& eLabel)
    {
        edges_.emplace_back(eLabel);
        adjacencyMatrix_(from, to) = &edges_.back();
    }

    unsigned verticesSize() const
    {
        return adjacencyMatrix_.cols();
    }

    EdgeLabel* getEdgeBetween(unsigned from, unsigned to)
    {
        return adjacencyMatrix_(from, to);
    }

    unsigned getVertex(unsigned id) const
    {
        return id;
    }

    bool isEdgeBetween(unsigned srcVert, unsigned destVert)
    {
        return adjacencyMatrix_(srcVert, destVert);
    }

protected:
    nostd::List<EdgeLabel> edges_;
    Matrix<EdgeLabel*> adjacencyMatrix_;
};
