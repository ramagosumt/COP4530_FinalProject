#ifndef MYGRID_H
#define MYGRID_H

#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include "Node.h"
using namespace std;


/**
 * Class to store and manipulate the grid
 * Also contains function implementing all kinds of algorithm
*/
class Grid
{
    public:
    int N;                    // Stores the number of rows
    int M;                    // Stores the number of columns
    Node Start;               // Stores the Start State of Agent
    vector<Node> Goals;       // Stores the all the Goals nodes 
    Node** G;                 // Stores the all the nodes on grid as a 2D node array

    /**
     * Constructor
     * @param a No. of rows
     * @param b No.of Columns
    */
    Grid(int A, int B);
    
    /**
     * Prints out the board onto the terminal
    */
    void PrintGraph();

    /**
     * Helper function to check whether a cell exist 
     * or is not a wall
     * or has not been visited before
     * @param x x-coordinate / row number
     * @param y y-coordinate / column number
     * @return true if cell is valid false otherwise
    */
    bool IsValid(int X,int Y);

    /**
     * Helper function to check whether a cell exist 
     * or is not a wall
     * or can be relaxed for djikstra's algorithm
     * @param x x-coordinate / row number
     * @param y y-coordinate / column number
     * @return true if cell is valid false otherwise
    */
    bool IsValidToRelax(int X,int Y,Node* Curr);

    /**
     * Estimates the cost to travel to destination from source
     * @param src  Source node 
     * @param dest Destination node
    */
    int Cost(Node* Src, Node* Dest);
    
    /**
     * Prints out a path from source to destination
     * @param tail Node pointer to dest which can be traversed reversely to reach source
     * @param path String to store the path
    */
    void PrintPath(Node* Tail,stringstream& SS);

    /**
     * Resets the visit values of all nodes to false
    */
    void ResetVisit();
    /**
     * Depth First Search Algorithm
     * Expands a single child before expanding it's neighbour
     * Prints out the path to all destinations from source
     * @param curr stores the source or current node being expanded
     * Disadvantages: Output paths are least efficient sometimes compared to other algorithms
     * Advantages: Less memory usage
    */
    void DFS(Node* Curr);

    /**
     * Breadth First Search Algorithm
     * Expands all it's neighbours then moves onto their neghbours
     * Prints out the path to all destinations from source
     * @param curr stores the source or start node
     * Disadvantages: Requires a lot of memory
     * Advantages: More efficient than DFS in some cases
    */
    void BFS(Node* Curr);

    /**
     * Greedy Best First Search Algorithm
     * Uses Priority queue to find the node with smallest cost value/shortest distance to destination and traverses it
     * Prints out the path to destination
     * @param src source node
     * @param dest destination node
     * Disadvantages: Smallest cost value may not be efficient everytime.
     * Advantage: Better efficiency than BFS and DFS same memory usage as BFS
    */
    // void GBFS(Node* Src, Node* Dest);

    /**
     * A* search Algortihm
     * Uses Priority queue to find the node with shortest distance to destination and shortest distance from source to current node
     * Prints out the path to destination
     * @param src source node
     * @param dest destination node
     * Advantages: Highly efficient paths 
    */
    void AS(Node* Src, Node* Dest);

    /**
     * Djikstra's algorithm to find shortest path to every node from source
     * Uses priority queue to find the node with shortest distance from source
     * @param src Source node
     * Advantages: Can tell the shortest path to any Node.
    */
    void Dijkstra(Node* Src);
    
    private:

    //Comparator for the priority queue for djkistra's
    struct CompareDijkstra
    {
        bool operator()(const Node * a, const Node * b)
        {
            return a->Dist > b->Dist;
        }
    };

    //Comparator for the priority queue for AS
    struct CompareAS
    {
        bool operator()(const Node* a, const Node* b)
        {
            return (a->Cost + a->Dist) > (b->Cost + b->Dist);
        }
    }; 

    // Explore (and update) neighboring nodes for each algorithm   
    void DFSExploreNeighbor(Node* Curr, int X, int Y);
    void BFSExploreNeighbor(Node* Current, int X, int Y, queue<Node*>& SearchQueue);
    void DijExploreNeighbor(Node* current, int x, int y, priority_queue<Node*, vector<Node*>, CompareDijkstra>& priorityQueue);
    void ASExploreNeighbor(Node* Current, int X, int Y, Node* Destination, priority_queue<Node*, vector<Node*>, CompareAS>& PriorityQueue);

    // void CUS2(Node* Src);
};
#endif