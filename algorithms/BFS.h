#ifndef BFS_H
#define BFS_H

#include "../dsa/Graph.h"
#include "../dsa/Queue.h"
#include <vector>
#include <unordered_map>
#include <algorithm>

class BFS {
public:
    // Perform BFS and return shortest path from source to target
    static std::vector<int> findShortestPath(const Graph& graph, int source, int target) {
        if (source == target) {
            return {source};
        }

        int vertices = graph.getVertices();
        std::vector<bool> visited(vertices, false);
        std::vector<int> parent(vertices, -1);
        Queue<int> queue;

        visited[source] = true;
        queue.enqueue(source);

        bool found = false;

        while (!queue.isEmpty() && !found) {
            int current = queue.dequeue();

            for (int neighbor : graph.getAdjacent(current)) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    parent[neighbor] = current;
                    queue.enqueue(neighbor);

                    if (neighbor == target) {
                        found = true;
                        break;
                    }
                }
            }
        }

        // Reconstruct path
        std::vector<int> path;
        if (found) {
            int current = target;
            while (current != -1) {
                path.push_back(current);
                current = parent[current];
            }
            std::reverse(path.begin(), path.end());
        }

        return path;
    }

    // Get next move towards target (returns next node in path)
    static int getNextMove(const Graph& graph, int source, int target) {
        std::vector<int> path = findShortestPath(graph, source, target);
        if (path.size() > 1) {
            return path[1]; // Return next node in path
        }
        return source; // Stay in place if no path
    }

    // BFS traversal (returns all reachable nodes)
    static std::vector<int> traverse(const Graph& graph, int source) {
        int vertices = graph.getVertices();
        std::vector<bool> visited(vertices, false);
        std::vector<int> result;
        Queue<int> queue;

        visited[source] = true;
        queue.enqueue(source);

        while (!queue.isEmpty()) {
            int current = queue.dequeue();
            result.push_back(current);

            for (int neighbor : graph.getAdjacent(current)) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    queue.enqueue(neighbor);
                }
            }
        }

        return result;
    }
};

#endif // BFS_H
