#include "Node.h"
/**
 * Constructor initializes cost  and distance to INT_MAX marks visit as false and prev points to NULL 
*/
Node::Node(int a,int b,char c):X(a),Y(b),Col(c),Cost(-1),Dist(-1),Visit(false),Prev(nullptr){};
