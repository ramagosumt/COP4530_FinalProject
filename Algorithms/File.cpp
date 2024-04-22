#include "File.h"

/**
 * Function that parses a line and returns all the space separated numbers as vectors
 * @param line character array line to be parsed
 * @param len length of the line to be parsed
 * @return integer vector storing all the numbers in the line.
*/
vector <int> ParseLine(char* Line,int Len)
{
    // a vector of all numerical values within a line of the text file
    vector<int> Numbers;
    int i = 0;
    int Sum = 0;  // each numerical value within a line
    bool Start = false;

    while(i < Len)    //loop till the line is not finished
    {
        if(Line[i]>='0' && Line[i]<='9')    // check if the character is a digit 
        {
            if(!Start) Start = true;        // a flag indicating this is a number
            Sum *= 10;                      // i.e. 13 = (0*10 + (1)) * 10 + (3) 
            Sum += Line[i]-'0';
        }

        if(!(Line[i]>='0' && Line[i]<='9') && Start) //if character is not a number then ignore 
        {
            Numbers.push_back(Sum);
            Start = false;
            Sum = 0;    // reset for the next available numerical value
        }

        if(Line[i]=='\n')break;     // finish parsing one line and move to the next one

        i++;    // move onto the next character
    }
    
    return Numbers; 
}

/**
 * Function that parses a file line by line and initializes a grid object
 * @param filename stores the filename to be openend
 * @return Grid object containing all the data regarding the grid
*/
Grid* ParseFile(char* Filename)
{
    // Check if the file exists or not
    FILE* Fptr = fopen(Filename, "r");          //opening the file in read mode 
    if(Fptr == nullptr)
    {
        cout << "File "<< Filename << " not found!\n";
        return nullptr;                             
    }

    char Line[MAX_ARG_LENGTH];
    char LineG[MAX_LINE_LENGTH];
    int NCoord, MCoord; // Row - Column Coordinates

    // --------------------- N & M ------------------------------------

    //checking and parsing the line for N and M values
    if(fgets(Line, MAX_ARG_LENGTH, Fptr) == nullptr)               
    {
        cout << "N and M values are missing!" << endl;
        return nullptr;   
    }

    // Extract N and M values
    vector<int> argV = ParseLine(Line, MAX_ARG_LENGTH);
    int N = argV[0]; int M = argV[1];
    Grid *Gd = new Grid(N,M);

    // --------------------- Start ------------------------------------

    // Parsing for Start values
    if(fgets(Line, MAX_ARG_LENGTH, Fptr) == nullptr)
    {
        cout << "Initial state missing!" << endl;
        return nullptr;   
    }
    // Extract Start value
    argV = ParseLine(Line, MAX_ARG_LENGTH);
    NCoord = argV[1]; MCoord = argV[0]; // Coordinates of the starting cell
    // Gd->Start = Node(argV[1], argV[0],'R');
    Gd->Start = Node(NCoord, MCoord,'R');   // Register the starting cell value
    // Gd->G[argV[1]][argV[0]].Col = 'R';
    Gd->G[NCoord][MCoord].Col = 'R';    // Parsing the grid for the starting cell

    // --------------------- Goals ------------------------------------

    // Parsing the goal states into a node vector
    if(fgets(LineG, MAX_LINE_LENGTH, Fptr) == nullptr)
    {
        cout << "Goal state missing!" << endl;
        return nullptr;   
    }
    // Extract Goal value
    argV = ParseLine(LineG, MAX_LINE_LENGTH);
    for(int i = 0; i < argV.size(); i+= 2) {
        NCoord = argV[i + 1]; MCoord = argV[i];
        // Gd->Goals.push_back(Node(argV[i + 1],argV[i],'G'));
        Gd->Goals.push_back(Node(NCoord,MCoord,'G'));
        // Gd->G[argV[i+1]][argV[i]].Col = 'G';
        Gd->G[NCoord][MCoord].Col = 'G';
    }

    // --------------------- Walls ------------------------------------
    while(fgets(Line, MAX_ARG_LENGTH, Fptr) != nullptr)
    {
        argV = ParseLine(Line, MAX_ARG_LENGTH);
        for(int i=0;i<argV[3];i++){
            for(int j = 0; j < argV[2]; j++){
                Gd->G[argV[1]+i][argV[0]+j].Col='W';    // parsing the grid for Wall values
            }
        }
    }
    
    return Gd;  // Return the properly parsed grid

}
