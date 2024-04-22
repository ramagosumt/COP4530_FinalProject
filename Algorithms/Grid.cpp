#include "Grid.h"

Grid::Grid(int A,int B)
{
    N = A;  // Assign the number of rows
    M = B;  // Assign the number of columns
    G = new Node*[N];   // Allocate memory for the grid
    
    // intialize grid with default values
    for(int i = 0; i < N; ++i){
        G[i] = new Node[M];     // Allocate memory for each row
        // Initialize each Node in the row
        for(int j = 0; j < M; ++j) {
            G[i][j].X=i;    // Set X coordinates
            G[i][j].Y=j;    // Set Y coordinates
        }
    }   
}


void Grid::PrintGraph()
{
    // Iterate over each row in the grid
    for(int i=0 ;i<N ;i++)
    {
        // Iterate over each column in the row
        for(int j=0 ;j<M;j++)cout<<G[i][j].Col<<" "; // Print the label attribute of the Node
        cout<<endl;     // Move to the next line after printing each row
    }
}


bool Grid::IsValid(int X,int Y)
{
    // check if the cell has proper coordinates
    if (X < 0 || X >= N || Y < 0 || Y >= M) return false;
    // check whether the cell is a wall or has been visited before
    if (G[X][Y].Col=='W' || G[X][Y].Visit == true) return false;
    
    return true;
}


bool Grid::IsValidToRelax(int X, int Y, Node* Curr)
{
    // check if the cell has proper coordinates or is a wall
    if (X < 0 || X >= N || Y < 0 || Y >= M || G[X][Y].Col=='W') return false;

    // check the distance
    if (G[X][Y].Dist <= Curr->Dist + 1) return false;

    return true;
}


void Grid::ResetVisit(){
    // #pragma omp parallel for
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<M;j++){
            G[i][j].Visit = false;
            G[i][j].Cost = -1;
            G[i][j].Dist = -1;
            G[i][j].Prev = nullptr;
        }
    }
}


int Grid::Cost(Node* Src, Node* Dest)
{
    // Retrieve coordinates of source node
    int Src_XCoord = Src->X; int Src_YCoord = Src->Y;

    // Retrieve coordinates of goal node
    int Dest_XCoord = Dest->X; int Dest_YCoord = Dest->Y;

    // Estimate the final distance between source node and goal node
    return abs(Src_XCoord - Dest_XCoord) + abs(Src_YCoord - Dest_YCoord);
}


void Grid::PrintPath(Node* Tail, stringstream& SS)
{
    if(Tail->Prev == nullptr) return; 
    
    // Recursive call to print the path up to the previous node of Tail
    PrintPath(Tail->Prev, SS);

    // Calculate the differences in X and Y coordinates between the current node and its previous node
    int dx = Tail->Prev->X - Tail->X ;
    int dy = Tail->Prev->Y - Tail->Y ;

    // If the previous node of Tail is the source node, print its coordinates
    if(Tail->Prev->Col=='R') SS << "Start : ( "<<Tail->Prev->X<<", "<<Tail->Prev->Y<<")\t ";
    
    // Determine the direction of movement based on the differences in coordinates and append it to SS
    if(dx == -1) SS << "down; ";
    else if(dx == 1) SS <<"up; ";
    else if (dy == 1) SS << "left; ";
    else if (dy == -1) SS << "right; ";

    // If the current node is a goal node, print its coordinates
    if(Tail->Col=='G') SS << "Goal : ( " << Tail->X <<", " << Tail->Y <<") ";
}


void Grid::DFS(Node* Curr)
{
    // Mark the current node as visited
    Curr->Visit = true;

    // Check if the current node is the goal node
    if(Curr->Col == 'G')
    {
        // If it is the goal node, print the path and return
        stringstream Path;
        PrintPath(Curr, Path);
        cout << Path.str() << endl;
        return;
    } 
    
    // Explore the neigboring nodes in four directions
    DFSExploreNeighbor(Curr, Curr->X - 1, Curr->Y);
    DFSExploreNeighbor(Curr, Curr->X, Curr->Y - 1);
    DFSExploreNeighbor(Curr, Curr->X + 1, Curr->Y);
    DFSExploreNeighbor(Curr, Curr->X, Curr->Y + 1);

    // Mark the current node as not visited
    Curr->Visit = false;
}


// Function to explore a neighboring node
void Grid::DFSExploreNeighbor(Node* Curr, int X, int Y)
{
    // Check if the neighboring node is valid
    if (IsValid(X, Y))
    {
        // If it is valid, update the previous pointer, explore recursively, and backtrack
        G[X][Y].Prev = Curr;
        DFS(&G[X][Y]);
        G[X][Y].Prev = nullptr;
    }
}


void Grid::BFSExploreNeighbor(Node* Current, int X, int Y, queue<Node*>& SearchQueue)
{
    if (IsValid(X, Y))
    {
        // If the neighbor is valid, update its previous pointer, mark it as visited, and enqueue it
        G[X][Y].Prev = Current;
        G[X][Y].Visit = true;
        SearchQueue.push(&G[X][Y]);
    }
}


void Grid::BFS(Node* Curr)
{
    // Create a queue for BFS traversal
    queue<Node*> SearchQueue;
    
    // Mark the starting node as visited and enqueue it
    Curr->Visit = true;
    SearchQueue.push(Curr);
    
    // BFS traversal loop
    while (!SearchQueue.empty())
    {
        // Dequeue the current node
        Node* Current = SearchQueue.front();
        SearchQueue.pop();

        // Check if the current node is the goal node
        if (Current->Col == 'G')
        {
            // If the goal node is found, print the path
            stringstream Path;
            PrintPath(Current, Path);
            cout << Path.str() << endl;
        }

        // Explore and enqueue neighboring nodes
        BFSExploreNeighbor(Current, Current->X - 1, Current->Y, SearchQueue);
        BFSExploreNeighbor(Current, Current->X, Current->Y - 1, SearchQueue);
        BFSExploreNeighbor(Current, Current->X + 1, Current->Y, SearchQueue);
        BFSExploreNeighbor(Current, Current->X, Current->Y + 1, SearchQueue);
    }
}


void Grid::AS(Node* Source, Node* Destination)
{
    priority_queue<Node*, vector<Node*>, CompareAS> PriorityQueue;

    // Set initial values for the source node
    Source->Cost = Cost(Source, Destination);
    Source->Dist = 0; // Distance from source is 0
    Source->Visit = true; // Mark source node as visited
    PriorityQueue.push(Source); // Push source node onto the priority queue

    // Continue until the priority queue is empty or destination is reached
    while (!PriorityQueue.empty()) {

        // Retrieve the node with the lowest estimated total cost from the priority queue
        Node* Current = PriorityQueue.top();
        PriorityQueue.pop();

        // Check if current node is the destination
        if (Current->X == Destination->X && Current->Y == Destination->Y) {
            // If destination is reached, print the path and exit the loop
            stringstream Path;
            PrintPath(Current, Path);
            cout << Path.str() << endl;
            break;
        }

        // Extract coordinates of the current node
        int CurrentX = Current->X;
        int CurrentY = Current->Y;

        // Explore the neighbors of the current node and update them (if necessary)
        ASExploreNeighbor(Current, CurrentX - 1, CurrentY, Destination, PriorityQueue);
        ASExploreNeighbor(Current, CurrentX, CurrentY - 1, Destination, PriorityQueue);
        ASExploreNeighbor(Current, CurrentX + 1, CurrentY, Destination, PriorityQueue);
        ASExploreNeighbor(Current, CurrentX, CurrentY + 1, Destination, PriorityQueue);
    }
}


void Grid::ASExploreNeighbor(Node* Current, int X, int Y, Node* Destination, priority_queue<Node*, vector<Node*>, CompareAS>& PriorityQueue)
{
    // Check if the neighbor node at a valid coordinate (X, Y)
    if (IsValid(X, Y)) {
        // Retrieve the neighbor node
        Node* Neighbor = &G[X][Y];

        // Calculate new distance and cost for the neighbor node
        int NewDistance = Current->Dist + 1;
        int NewCost = NewDistance + Cost(Neighbor, Destination);

        // If neighbor node has not been visited or new cost is lower than previous cost
        if (!Neighbor->Visit || NewCost < Neighbor->Cost) {
            Neighbor->Prev = Current;   // Set predecessor of neighbor to current node
            Neighbor->Dist = NewDistance;   // Update distance to neighbor
            Neighbor->Cost = NewCost;   // Update total cost to reach neighbor
            Neighbor->Visit = true; // Mark neighbor as visited
            PriorityQueue.push(Neighbor);
        }
    }
}


void Grid::Dijkstra(Node* Source)
{
    // Initialize priority queue to store nodes based on their distances
    priority_queue<Node*, vector<Node*>, CompareDijkstra> PriorityQueue;
    
    // Initialize distances of all nodes to infinity
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            G[i][j].Dist = INT_MAX;
        }
    }

    // Set the distances of source node to 0
    Source->Dist = 0;
    PriorityQueue.push(Source);
    
    // Continue until the priority queue is empty or destination is reached
    while (!PriorityQueue.empty())
    {
        // Retrieve the node with the smallest distance from the source node
        Node* Current = PriorityQueue.top();
        PriorityQueue.pop();

        // Extract coordinates of the current node
        int CurrentX = Current->X;
        int CurrentY = Current->Y;


        // Explore the neighbors, and update them if necessary
        DijExploreNeighbor(Current, CurrentX - 1, CurrentY, PriorityQueue);
        DijExploreNeighbor(Current, CurrentX, CurrentY - 1, PriorityQueue);
        DijExploreNeighbor(Current, CurrentX + 1, CurrentY, PriorityQueue);
        DijExploreNeighbor(Current, CurrentX, CurrentY + 1, PriorityQueue);
    }
}


void Grid::DijExploreNeighbor(Node* Current, int X, int Y, priority_queue<Node*, vector<Node*>, CompareDijkstra>& PriorityQueue)
{
    // Check if the neighbor node at coordinates (X, Y) can be relaxed
    if (IsValidToRelax(X, Y, Current))
    {   
        // Retrieve the neighbor node
        Node* Neighbor = &G[X][Y];

        // Calculate the new distance from the source to the neighbor node
        int NewDistance = Current->Dist + 1;

        //  Update the shortest path information for the neighbor node
        Neighbor->Prev = Current;
        Neighbor->Dist = NewDistance;
        PriorityQueue.push(Neighbor); // Push the neighbor node into the priority queue for further exploration
    }
}
