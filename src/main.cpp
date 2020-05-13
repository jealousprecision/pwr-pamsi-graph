#include <cstdlib>
#include <ctime>
#include <cmath>

#include <algorithm>
#include <iterator>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>

#include <nostd/Vector.hpp>
#include <nostd/List.hpp>
#include <TestObject.hpp>
#include <GraphList.hpp>
#include <GraphMatrix.hpp>
#include <GraphUtils.hpp>
#include <GraphAlgo.hpp>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <boost/algorithm/algorithm.hpp>

using namespace boost;
using Graph = adjacency_list<listS, vecS, directedS, no_property, property<edge_weight_t, unsigned>>;

template<typename BoostGraphType, typename E>
void fillGraphs(BoostGraphType& boostGraph, GraphList<VoidType, E>& myGraph, unsigned vertices)
{
    const unsigned nOfEdges = vertices * (vertices - 1) * 0.75;
    constexpr unsigned max_weight = 5000u;

    Matrix<bool> adjacencyMatrix(vertices, vertices, false);  // acount so no parralel edges are made

    for (unsigned i = 0; i < nOfEdges; ++i)
    {
        unsigned src, dest;
        do {
            src = std::rand() % vertices;
            dest = std::rand() % vertices;
        } while (!adjacencyMatrix(src, dest));

        adjacencyMatrix(src, dest) = true;

        auto weight = std::rand() % max_weight;
        add_edge(src, dest, weight, boostGraph);
        myGraph.addEdge(src, dest, weight);
    }
}

template<typename BoostGraphType, typename GraphType>
void fillFullGraphs(BoostGraphType& boostGraph, GraphType& myGraph, unsigned vertices)
{
    constexpr unsigned max_weight = 5000u;

    for (unsigned src = 0; src < vertices; ++src)
    {
        for (unsigned dest = 0; dest < vertices; ++dest)
        {
            if (src == dest)
                continue;

            unsigned weight = std::rand() % max_weight;

            add_edge(src, dest, weight, boostGraph);
            myGraph.addEdge(src, dest, weight);
        }
    }
}

template<typename BoostGraphType, typename E>
void fillGraphs(BoostGraphType& boostGraph, GraphMatrix<VoidType, E>& myGraph, unsigned vertices)
{
    const unsigned nOfEdges = vertices * (vertices - 1) * 0.75;
    constexpr unsigned max_weight = 1000u;

    for (unsigned i = 0; i < nOfEdges; ++i)
    {
        unsigned src, dest, weight;
        do {
            src = std::rand() % vertices;
            dest = std::rand() % vertices;
            weight = std::rand() % max_weight;
        } while (myGraph.getEdgeBetween(src, dest));

        add_edge(src, dest, weight, boostGraph);
        myGraph.addEdge(src, dest, weight);
    }
}

template<typename MyGraphType>
void test()
{
    std::srand(std::time(nullptr));
    const unsigned nOfVerts = 1000;

    for (int i = 0; i < 100; ++i)
    {
        Graph boostGraph(nOfVerts);
        MyGraphType myGraph(nOfVerts);
        fillGraphs(boostGraph, myGraph, nOfVerts);
        //fillFullGraphs(boostGraph, myGraph, nOfVerts);


        std::vector<unsigned> boostIdxToCost(nOfVerts);
        auto source = *vertices(boostGraph).first;

        auto start = std::chrono::steady_clock::now();
        dijkstra_shortest_paths(boostGraph, source, distance_map(boostIdxToCost.data()));
        auto end = std::chrono::steady_clock::now();
        std::cout << "boost: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / pow(10, 6) << std::endl;

        start = std::chrono::steady_clock::now();
        auto res = Dijkstra(myGraph, 0);
        end = std::chrono::steady_clock::now();
        std::cout << "my: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / pow(10, 6) << std::endl;
        const auto& myIdxToCost = std::get<0>(res);


        bool passed = std::equal(boostIdxToCost.begin(), boostIdxToCost.end(), myIdxToCost.begin(), myIdxToCost.end());
        if (!passed)
        {
            std::cout << "Error: idxToCost not equal" << std::endl;
            return;
        }

        std::cout << i << '\n';
    }
}

int main()
{
    test<GraphList<VoidType, unsigned>>();
}
