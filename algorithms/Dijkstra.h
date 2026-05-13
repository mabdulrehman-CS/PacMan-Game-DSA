#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "../dsa/Graph.h"
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

class Dijkstra {
public:
    // Find shortest path using Dijkstra's algorithm (using STL priority_queue)
    static std::vector<int> findShortestPath(const Graph& graph, int source, int target) {
        if (source == target) {
            return {source};
        }

        int vertices = graph.getVertices();
        std::vector<int> distance(vertices, std::numeric_limits<int>::max());
        std::vector<int> parent(vertices, -1);
        std::vector<bool> visited(vertices, false);

        // Priority queue: pair<distance, node>
        std::priority_queue<std::pair<int, int>, 
                          std::vector<std::pair<int, int>>, 
                          std::greater<std::pair<int, int>>> pq;

        distance[source] = 0;
        pq.push({0, source});

        while (!pq.empty()) {
            int current = pq.top().second;
            int dist = pq.top().first;
            pq.pop();

            if (visited[current]) continue;
            visited[current] = true;

            if (current == target) break;

            for (int neighbor : graph.getAdjacent(current)) {
                int weight = 1; // Uniform weight for all edges
                int newDist = dist + weight;

                if (newDist < distance[neighbor]) {
                    distance[neighbor] = newDist;
                    parent[neighbor] = current;
                    pq.push({newDist, neighbor});
                }
            }
        }

        // Reconstruct path
        std::vector<int> path;
        if (distance[target] != std::numeric_limits<int>::max()) {
            int current = target;
            while (current != -1) {
                path.push_back(current);
                current = parent[current];
            }
            std::reverse(path.begin(), path.end());
        }

        return path;
    }

    // Get next move towards target
    static int getNextMove(const Graph& graph, int source, int target) {
        std::vector<int> path = findShortestPath(graph, source, target);
        if (path.size() > 1) {
            return path[1];
        }
        return source;
    }

    // Get shortest distance
    static int getShortestDistance(const Graph& graph, int source, int target) {
        std::vector<int> path = findShortestPath(graph, source, target);
        if (path.empty()) {
            return -1; // No path
        }
        return path.size() - 1;
    }
};

#endif // DIJKSTRA_H
