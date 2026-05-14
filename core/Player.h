#ifndef PLAYER_H
#define PLAYER_H

#include "../dsa/Stack.h"
#include "Maze.h"

class Player {
private:
    Position position;
    int currentNode;
    Stack<Position> moveHistory;
    int score;
    int lives;

public:
    Player(const Position& startPos, int startNode) 
        : position(startPos), currentNode(startNode), score(0), lives(3) {}

    // Move player to new position
    bool move(const Position& newPos, Maze& maze) {
        if (maze.isValidPath(newPos.row, newPos.col)) {
            // Save current position to history
            moveHistory.push(position);
            
            // Update position
            position = newPos;
            currentNode = maze.getNodeAt(newPos.row, newPos.col);
            
            // Check for pellet collection
            if (maze.hasPellet(position)) {
                maze.removePellet(position);
                score += 10;
            }
            
            return true;
        }
        return false;
    }

    // Move in direction (0=up, 1=down, 2=left, 3=right)
    bool moveDirection(int direction, Maze& maze) {
        Position newPos = position;
        
        switch(direction) {
            case 0: newPos.row--; break; // Up
            case 1: newPos.row++; break; // Down
            case 2: newPos.col--; break; // Left
            case 3: newPos.col++; break; // Right
        }
        
        if (newPos.col < 0) newPos.col = maze.getCols() - 1;
        else if (newPos.col >= maze.getCols()) newPos.col = 0;
        
        return move(newPos, maze);
    }

    // Undo last move
    bool undoMove(Maze& maze) {
        if (!moveHistory.isEmpty()) {
            position = moveHistory.pop();
            currentNode = maze.getNodeAt(position.row, position.col);
            return true;
        }
        return false;
    }

    // Getters
    Position getPosition() const { return position; }
    int getCurrentNode() const { return currentNode; }
    int getScore() const { return score; }
    int getLives() const { return lives; }
    int getMoveCount() const { return moveHistory.getSize(); }

    // Setters
    void setPosition(const Position& pos) { position = pos; }
    void setCurrentNode(int node) { currentNode = node; }
    void addScore(int points) { score += points; }
    void loseLife() { if (lives > 0) lives--; }
    void addLife() { lives++; }

    // Clear history
    void clearHistory() {
        moveHistory.clear();
    }

    // Check if player has moves to undo
    bool canUndo() const {
        return !moveHistory.isEmpty();
    }
};

#endif // PLAYER_H
