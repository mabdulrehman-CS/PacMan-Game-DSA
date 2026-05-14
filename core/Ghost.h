#ifndef GHOST_H
#define GHOST_H

#include "Maze.h"
#include "Player.h"
#include "../algorithms/BFS.h"
#include "../algorithms/Dijkstra.h"
#include <string>

enum class GhostAI {
    BFS_AI,      // Uses BFS for pathfinding
    DIJKSTRA_AI, // Uses Dijkstra for pathfinding
    RANDOM       // Random movement
};

class Ghost {
private:
    Position position;
    int currentNode;
    GhostAI aiType;
    float speed;
    std::string name;
    bool isActive;
    bool isScared;          // True when ghost is vulnerable
    int scaredTimer;        // Countdown timer for scared mode

public:
    Ghost(const Position& startPos, int startNode, GhostAI ai, const std::string& ghostName = "Ghost") 
        : position(startPos), currentNode(startNode), aiType(ai), 
          speed(1.0f), name(ghostName), isActive(true), isScared(false), scaredTimer(0) {}

    // Update ghost position based on AI
    void update(const Player& player, Maze& maze) {
        if (!isActive) return;

        // Update scared timer
        if (isScared && scaredTimer > 0) {
            scaredTimer--;
            if (scaredTimer == 0) {
                isScared = false;
            }
        }

        Graph* graph = maze.getGraph();
        if (graph == nullptr) return;

        int targetNode = player.getCurrentNode();
        int nextNode = currentNode;

        // When scared, run away from player (use opposite direction logic)
        if (isScared) {
            nextNode = getRandomMove(*graph); // Simple scared behavior
        } else {
            // Choose pathfinding algorithm based on AI type
            switch (aiType) {
                case GhostAI::BFS_AI:
                    nextNode = BFS::getNextMove(*graph, currentNode, targetNode);
                    break;
                
                case GhostAI::DIJKSTRA_AI:
                    nextNode = Dijkstra::getNextMove(*graph, currentNode, targetNode);
                    break;
                
                case GhostAI::RANDOM:
                    nextNode = getRandomMove(*graph);
                    break;
            }
        }

        // Update position
        if (nextNode != currentNode) {
            currentNode = nextNode;
            position = maze.getPositionFromNode(nextNode);
        }
    }

    // Get random adjacent node
    int getRandomMove(const Graph& graph) {
        const std::list<int>& neighbors = graph.getAdjacent(currentNode);
        if (neighbors.empty()) {
            return currentNode;
        }
        
        // Simple random selection
        int index = rand() % neighbors.size();
        auto it = neighbors.begin();
        std::advance(it, index);
        return *it;
    }

    // Check collision with player
    bool collidesWith(const Player& player) const {
        return position.row == player.getPosition().row && 
               position.col == player.getPosition().col;
    }

    // Scared mode control
    void setScared(int duration) {
        isScared = true;
        scaredTimer = duration;
    }
    
    bool getIsScared() const { return isScared; }
    int getScaredTimer() const { return scaredTimer; }

    // Getters
    Position getPosition() const { return position; }
    int getCurrentNode() const { return currentNode; }
    GhostAI getAIType() const { return aiType; }
    std::string getName() const { return name; }
    bool getIsActive() const { return isActive; }
    float getSpeed() const { return speed; }

    // Setters
    void setPosition(const Position& pos) { position = pos; }
    void setCurrentNode(int node) { currentNode = node; }
    void setAIType(GhostAI ai) { aiType = ai; }
    void setActive(bool active) { isActive = active; }
    void setSpeed(float s) { speed = s; }

    // Reset to starting position
    void reset(const Position& startPos, int startNode) {
        position = startPos;
        currentNode = startNode;
        isActive = true;
        isScared = false;
        scaredTimer = 0;
    }
};

#endif // GHOST_H
