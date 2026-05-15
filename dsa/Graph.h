#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>
#include <unordered_map>
#include <iostream>

class Graph {
private:
    int vertices;
    std::vector<std::list<int>> adjList;
    std::unordered_map<int, std::pair<int, int>> nodePositions; // Maps node ID to (row, col)

public:
    Graph(int v) : vertices(v) {
        adjList.resize(v);
    }

    // Add an edge between two vertices
    void addEdge(int u, int v) {
        if (u >= 0 && u < vertices && v >= 0 && v < vertices) {
            adjList[u].push_back(v);
            adjList[v].push_back(u); // Undirected graph
        }
    }

    // Get adjacent vertices
    const std::list<int>& getAdjacent(int v) const {
        static std::list<int> empty;
        if (v >= 0 && v < vertices) {
            return adjList[v];
        }
        return empty;
    }

    // Get number of vertices
    int getVertices() const {
        return vertices;
    }

    // Store position mapping for maze integration
    void setNodePosition(int nodeId, int row, int col) {
        nodePositions[nodeId] = {row, col};
    }

    // Get position of a node
    std::pair<int, int> getNodePosition(int nodeId) const {
        auto it = nodePositions.find(nodeId);
        if (it != nodePositions.end()) {
            return it->second;
        }
        return {-1, -1};
    }

    // Print graph (for debugging)
    void printGraph() const {
        for (int i = 0; i < vertices; i++) {
            std::cout << "Vertex " << i << ": ";
            for (int neighbor : adjList[i]) {
                std::cout << neighbor << " ";
            }
            std::cout << std::endl;
        }
    }

    // Check if edge exists
    bool hasEdge(int u, int v) const {
        if (u < 0 || u >= vertices) return false;
        for (int neighbor : adjList[u]) {
            if (neighbor == v) return true;
        }
        return false;
    }
};

#endif // GRAPH_H
