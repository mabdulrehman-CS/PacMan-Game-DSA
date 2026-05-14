# 🎮 AlgoMan: DSA-Driven Pac-Man (Console Edition)

A high-performance, professional Pac-Man implementation in C++ that demonstrates core Data Structures and Algorithms in a terminal environment. Built for the terminal but designed like a modern game.

![C++](https://img.shields.io/badge/C++-17-orange)
![Status](https://img.shields.io/badge/status-complete-success)
![Platform](https://img.shields.io/badge/platform-Windows-blue)

---

## ⚡ Quick Start

**Play the game:** Double-click `PLAY_NOW.bat`  
**Rebuild source:** Double-click `rebuild.bat`  

---

## 🎯 Project Overview

**AlgoMan** is an educational game project that proves DSA concepts power real-time interactive systems. Every mechanic in this console game is driven by a custom-implemented data structure or algorithm:

- **Maze Navigation**: Adjacency List Graph
- **Ghost AI**: BFS & Dijkstra's Algorithm
- **Move History**: Manual Stack (for Undo)
- **Pellet Management**: Singly Linked List
- **Game Loops**: Optimized Win32 API rendering

---

## 🧠 DSA Implementation Details

| Component | Data Structure / Algorithm | File |
|-----------|---------------------------|------|
| **Maze** | **Graph (Adjacency List)** | `dsa/Graph.h` |
| **Pathfinding** | **BFS** (Shortest Path) | `algorithms/BFS.h` |
| **Advanced AI** | **Dijkstra's Algorithm** | `algorithms/Dijkstra.h` |
| **Undo System** | **Stack** (Linked List based) | `dsa/Stack.h` |
| **Event Buffer** | **Queue** | `dsa/Queue.h` |
| **Pellet Store** | **Linked List** | `dsa/LinkedList.h` |

---

## ✨ Features

- ✅ **Professional UI**: Custom logo, interactive menus, and post-game scoreboards.
- ✅ **Flicker-Free Graphics**: Uses Win32 API `SetConsoleCursorPosition` and buffer management for smooth 60fps-like experience.
- ✅ **Rich Visuals**: Full Unicode/UTF-8 support featuring Emojis (👻, 🍓, 💀).
- ✅ **Ghost AI Personalities**:
  - **Blinky/Inky**: Chasing using BFS.
  - **Pinky**: Optimal pathfinding using Dijkstra.
  - **Clyde**: Random movement logic.
- ✅ **Dynamic Undo**: Press **U** to undo your moves in real-time using a Stack.
- ✅ **Power-ups**: Eat strawberries (🍓) to make ghosts scared (💀) and eatable.

---

## 🎮 How to Play

### Controls
| Key | Action |
|-----|--------|
| **W/A/S/D** or **Arrows** | Move Player (`C`) |
| **U** | Undo last move (Stack Pop) |
| **P** | Pause Game |
| **Q** | Quit to Menu |

### Symbols
- 🟡 `C` : You (The Player)
- 👻 : Ghosts (Colored)
- 💀 : Scared Ghost
- 🍓 : Power Pellet (+50 pts)
- `.` : Regular Pellet (+10 pts)
- 🟦 : Blue Walls (Graph boundaries)

---

## 🔨 Build Instructions

### Requirements
- **Compiler**: MinGW-w64 (g++ with C++17 support)
- **OS**: Windows (Required for Win32 Console API)

### Build Command
The project is built as a static executable to ensure portability:
```bash
g++ -std=c++17 src/main_console.cpp -o AlgoMan_Console.exe -static
```
Or simply run `rebuild.bat`.

---

## 📁 Project Architecture

```
AlgoMan/
├── 🎮 EXECUTABLE
│   └── AlgoMan_Console.exe      # Main game engine
├── 🚀 SCRIPTS
│   ├── PLAY_NOW.bat             # User-friendly launcher
│   └── rebuild.bat              # Compilation script
├── 💾 SOURCE CODE
│   └── src/
│       └── main_console.cpp     # UI and Game Loop logic
├── 🧮 DATA STRUCTURES
│   └── dsa/
│       ├── Graph.h              # Adjacency List Maze
│       ├── Stack.h              # Undo functionality
│       ├── Queue.h              # Event handling
│       └── LinkedList.h         # Pellet management
├── 🔍 ALGORITHMS
│   └── algorithms/
│       ├── BFS.h                # Standard Pathfinding
│       └── Dijkstra.h           # Advanced AI logic
└── ⚙️ CORE COMPONENTS
    └── core/
        ├── Maze.h               # Grid-to-Graph converter
        ├── Player.h             # Movement & Score logic
        └── Ghost.h              # AI state machine
```

---

## 📄 License
Created for educational purposes in DSA course.
