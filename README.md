# Pathfinding Visualizer

### Unreal Engine 5 | C++ | A* and Dijkstra Visualization Framework

This project is an interactive pathfinding visualization tool developed in Unreal Engine 5 as part of COP 4530 (Data Structures & Algorithms).

The application allows users to generate grid-based environments, place obstacles, and visualize the execution of pathfinding algorithms in real time. Rather than focusing solely on algorithm implementation, the project emphasizes visualization, user interaction, and system architecture for educational and debugging purposes.

---

# Programming Portfolio Notes

This repository is submitted as a programming portfolio sample focused on algorithm implementation, visualization, and interactive tool development in Unreal Engine 5.

The project demonstrates my ability to combine classical computer science concepts with real-time visualization systems and custom user interfaces.

For code reviewers, I recommend starting with the **Architecture Highlights** and **Code Review Guide** sections.

---

# Why This Project

I selected this project because it demonstrates how I approach algorithm-heavy problems through both implementation and visualization.

Rather than simply producing algorithm output, I designed a framework that allows users to observe how pathfinding algorithms explore, evaluate, and select routes through a grid-based environment.

The project demonstrates:

- A* implementation
- Dijkstra implementation
- Grid graph generation
- Obstacle management
- Interactive visualization
- Unreal Engine gameplay architecture
- Runtime UI generation
- Data structure integration

---

# Core Architecture

The project is organized into several systems:

```text
Grid Generation
Obstacle Placement
Pathfinding Algorithms
Visualization Layer
User Interface Layer
```

Major source folders:

```text
Actors/
    Grid
    GridTile
    Obstacles

Frameworks/
    GameMode
    PlayerController

Pawn/
    MainPawn

UserInterfaces/
    Algorithm Selection
    Grid Visualization
    Tooltips

Data/
    Pathfinding Data Structures
```

---

# Architecture Highlights

## Grid-Based Environment

The environment is generated from reusable Grid and GridTile actors.

Each tile maintains its own state and can be queried by pathfinding systems during execution.

Benefits:

- Modular design
- Easy obstacle placement
- Clear graph representation

---

## Pathfinding Algorithms

The project implements:

### A*

Uses heuristic-based search to prioritize promising paths and reduce unnecessary exploration.

### Dijkstra

Uses cost-based graph traversal to guarantee the shortest path while exploring all viable nodes.

Benefits:

- Direct comparison between algorithms
- Visualization of search behavior
- Educational value

---

## Runtime Visualization

Algorithm progress is visualized directly within the game world.

Users can observe:

- Visited nodes
- Frontier expansion
- Obstacle avoidance
- Final path generation

Benefits:

- Easier debugging
- Improved understanding of algorithm behavior
- Interactive learning tool

---

## User Interface Integration

The visualization framework is paired with custom Unreal Engine widgets.

Features include:

- Algorithm selection
- Tile information tooltips
- Grid interaction

Benefits:

- Improved usability
- Clear feedback
- Interactive experimentation

---

# Code Review Guide

Recommended files to inspect first:

---

## Grid System

### Files

- `Actors/Grid.h`
- `Actors/Grid.cpp`
- `Actors/GridTile.h`
- `Actors/GridTile.cpp`

### Concepts Demonstrated

- Grid generation
- Spatial organization
- Node representation
- Graph construction

---

## Pathfinding Data Structures

### Files

- `Data/PathfindingDataStructs.h`

### Concepts Demonstrated

- Search node storage
- Path reconstruction
- Algorithm bookkeeping

---

## Framework

### Files

- `Frameworks/AStarGameModeBase.h`
- `Frameworks/AStarGameModeBase.cpp`
- `Frameworks/AStarPlayerController.h`
- `Frameworks/AStarPlayerController.cpp`

### Concepts Demonstrated

- Algorithm orchestration
- Runtime execution
- User interaction management

---

## Obstacles

### Files

- `Actors/BaseObstacle.h`
- `Actors/BaseObstacle.cpp`
- `Actors/GrassObstacle.*`
- `Actors/TableObstacle.*`

### Concepts Demonstrated

- Traversal restrictions
- Dynamic environment configuration
- Obstacle-aware pathfinding

---

## User Interface

### Files

- `UserInterfaces/AlgorithmsSelectionWidget.h`
- `UserInterfaces/AlgorithmsSelectionWidget.cpp`
- `UserInterfaces/GridTileWidget.h`
- `UserInterfaces/GridTileWidget.cpp`

### Concepts Demonstrated

- Runtime UI generation
- User input
- Visualization feedback

---

# Build Instructions

Requirements:

- Unreal Engine 5
- Visual Studio 2022

Setup:

1. Clone repository
2. Right-click `.uproject`
3. Select **Generate Visual Studio Project Files**
4. Open solution
5. Build Development Editor
6. Launch project

---

# Features

| Feature | Status |
|----------|----------|
| Grid Generation | Complete |
| Obstacle Placement | Complete |
| A* Pathfinding | Complete |
| Dijkstra Pathfinding | Complete |
| Runtime Visualization | Complete |
| UI Integration | Complete |

---

# Scope Note

This project was developed as part of a university algorithms course. While inspired by educational software, the implementation focuses on combining classical pathfinding algorithms with interactive visualization inside a real-time game engine.

---

Developed by **@ramagosumt**
