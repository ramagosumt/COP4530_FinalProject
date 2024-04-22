#include<iostream>
#include<stdio.h>
#include<vector>

#include "Node.h"
#include "Grid.h"
#include "File.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        cout << "Usage : " << argv[0] << " <filename> <method>" << endl;
        return 0;
    }

    string METHOD = argv[2];

    Grid* GD = ParseFile(argv[1]);

    // Print the grid
    GD->PrintGraph();

    // Set the initial values                        
    GD->G[GD->Start.X][GD->Start.Y].Dist = 0;

    // Run the desired algorithm
    if (METHOD == "DFS") {
        cout << "Depth First Search\n";
        GD->G[GD->Start.X][GD->Start.Y].Visit = true;
        GD->DFS(&(GD->Start));
    }
    else if (METHOD == "BFS") {
        cout << "Breadth First Search\n";
        GD->BFS(&(GD->Start));
    }
    else if (METHOD == "Dijkstra") {
        cout << "Dijkstra Algorithm\n";
        GD->Dijkstra(&(GD->Start));
        for (int i = 0; i < GD->Goals.size(); i++)
        {
            int GX = GD->Goals[i].X;
            int GY = GD->Goals[i].Y;
            stringstream PATH;
            GD->PrintPath(&(GD->G[GX][GY]), PATH);
            cout << PATH.str() << endl;
        }
    }
    else if (METHOD == "AS") {
        cout << "A* Search\n";
        for (int i = 0; i < GD->Goals.size(); i++)
        {
            GD->AS(&(GD->Start), &(GD->Goals[i]));
            GD->ResetVisit();
        }
    }
    return 0;
}

