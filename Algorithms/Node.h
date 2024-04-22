#ifndef MYNODE_H
#define MYNODE_H

#ifndef NULL
#define NULL nullptr
#endif

using namespace std;
/**
 * Class Node to store each of the cells in the grid
*/
class Node
{
    public:
    int X;                                      // Stores the the row number starting from 0 
    int Y;                                      // Stores the the column number starting from 0
    unsigned int Cost;                          // Stores the cost to reach a goal
    unsigned int Dist;                          // Stores the distance travelled till now
    char Col;                                   // Colour coding of the cell indicating 'R' for agent 'W' walls 'G' for Goals 'O' otherwise
    bool Visit;                                 // Marks whether the cell has been visited or not
    Node *Prev;                                 // Pointer to store the predecessor of the current node.
    Node(int A = -1,int B =-1,char C='O');      // COnstructor
};
#endif