#include "node.h"

class Route
{
private:
    vector<Node> nodes;
    int capacity;
    int total_nodes;
    double total_travel_time;
    friend class Solution;
    friend class FAGA;
public:
    Route();
    bool add_node(int,Node,int,double);
    bool legal(int,Node,int,double);
    double total_distance();
    void update(int);
    void remove_node(Node);
    void print();
    void remove();
    bool operator<(Route);
    bool operator==(Route);

    int get_total_nodes();
    void print_file(ofstream&);
};
