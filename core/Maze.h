#ifndef MAZE_H
#define MAZE_H

#include "../dsa/Graph.h"
#include "../dsa/LinkedList.h"
#include <vector>
#include <unordered_map>

struct Position {
    int row;
    int col;

    Position(int r = 0, int c = 0) : row(r), col(c) {}

    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
};

class Maze {
private:
    std::vector<std::vector<int>> grid;
    Graph* graph;
    int rows;
    int cols;
    std::unordered_map<int, Position> nodeToPos;  // Node ID -> Position
    std::unordered_map<int, int> posToNode;       // Position hash -> Node ID
    LinkedList<Position> pellets;                 // Pellet positions
    LinkedList<Position> powerPellets;            // Power pellet positions (fruits)
    int nodeCount;

    // Hash function for position
    int hashPosition(int row, int col) const {
        return row * cols + col;
    }

public:
    Maze(int r, int c) : rows(r), cols(c), nodeCount(0) {
        grid.resize(rows, std::vector<int>(cols, 1)); // Initialize with walls
        graph = nullptr;
    }

    ~Maze() {
        delete graph;
    }

    // Set cell value (0 = path, 1 = wall)
    void setCell(int row, int col, int value) {
        if (row >= 0 && row < rows && col >= 0 && col < cols) {
            grid[row][col] = value;
        }
    }

    // Get cell value
    int getCell(int row, int col) const {
        if (row >= 0 && row < rows && col >= 0 && col < cols) {
            return grid[row][col];
        }
        return 1; // Wall
    }

    // Build graph from grid
    void buildGraph() {
        // Count path cells
        nodeCount = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j] == 0) { // Path cell
                    nodeCount++;
                }
            }
        }

        // Create graph
        delete graph; // Clean up old graph if exists
        graph = new Graph(nodeCount);

        // Assign node IDs to path cells
        int nodeId = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j] == 0) {
                    nodeToPos[nodeId] = Position(i, j);
                    posToNode[hashPosition(i, j)] = nodeId;
                    graph->setNodePosition(nodeId, i, j);
                    nodeId++;
                }
            }
        }

        // Add edges between adjacent path cells
        int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Up, Down, Left, Right

        for (const auto& pair : nodeToPos) {
            int node = pair.first;
            Position pos = pair.second;

            for (int d = 0; d < 4; d++) {
                int newRow = pos.row + directions[d][0];
                int newCol = pos.col + directions[d][1];

                if (getCell(newRow, newCol) == 0) { // Adjacent path cell
                    int neighborNode = posToNode[hashPosition(newRow, newCol)];
                    graph->addEdge(node, neighborNode);
                }
            }
        }
    }

    // Initialize pellets on all path cells
    void initializePellets() {
        pellets.clear();
        powerPellets.clear();
        
        // Add power pellets at corners (like classic Pac-Man)
        std::vector<Position> powerPelletPos = {
            Position(1, 1),   // Top-left
            Position(1, 13),  // Top-right
            Position(13, 1),  // Bottom-left
            Position(13, 13)  // Bottom-right
        };
        
        for (const auto& pair : nodeToPos) {
            Position pos = pair.second;
            bool isPowerPellet = false;
            
            // Check if this position is a power pellet
            for (const Position& pp : powerPelletPos) {
                if (pos.row == pp.row && pos.col == pp.col) {
                    powerPellets.insertAtEnd(pos);
                    isPowerPellet = true;
                    break;
                }
            }
            
            // Regular pellets everywhere else
            if (!isPowerPellet) {
                pellets.insertAtEnd(pos);
            }
        }
    }

    // Get node ID from position
    int getNodeAt(int row, int col) const {
        int hash = hashPosition(row, col);
        auto it = posToNode.find(hash);
        if (it != posToNode.end()) {
            return it->second;
        }
        return -1; // Invalid
    }

    // Get position from node ID
    Position getPositionFromNode(int nodeId) const {
        auto it = nodeToPos.find(nodeId);
        if (it != nodeToPos.end()) {
            return it->second;
        }
        return Position(-1, -1);
    }

    // Remove pellet at position
    bool removePellet(const Position& pos) {
        return pellets.remove(pos);
    }

    // Check if pellet exists at position
    bool hasPellet(const Position& pos) const {
        return pellets.search(pos);
    }

    // Get pellets list
    LinkedList<Position>& getPellets() {
        return pellets;
    }
    
    LinkedList<Position>& getPowerPellets() {
        return powerPellets;
    }

    // Get graph
    Graph* getGraph() {
        return graph;
    }

    // Get dimensions
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    int getNodeCount() const { return nodeCount; }

    // Check if position is valid path
    bool isValidPath(int row, int col) const {
        return getCell(row, col) == 0;
    }

    // Load predefined maze
    void loadDefaultMaze() {
        // 15x15 TRUE CLASSIC Pac-Man maze (based on original game)
        std::vector<std::vector<int>> defaultMaze = {
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,1,1,0,1,1,1,1,1,0,1,1,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,1,1,0,1,0,1,0,1,0,1,1,0,1},
            {1,0,0,0,0,1,0,0,0,1,0,0,0,0,1},
            {1,1,1,1,0,1,1,0,1,1,0,1,1,1,1},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},  // Open tunnel
            {1,1,1,1,0,1,1,0,1,1,0,1,1,1,1},
            {1,0,0,0,0,1,0,0,0,1,0,0,0,0,1},
            {1,0,1,1,0,1,0,1,0,1,0,1,1,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,1,1,0,1,1,1,1,1,0,1,1,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
        };

        for (int i = 0; i < rows && i < (int)defaultMaze.size(); i++) {
            for (int j = 0; j < cols && j < (int)defaultMaze[i].size(); j++) {
                grid[i][j] = defaultMaze[i][j];
            }
        }

        buildGraph();
        initializePellets();
    }
};

#endif // MAZE_H
