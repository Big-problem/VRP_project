#include "tool.h"
#define EARTH_RADIUS 6378.137
#define Pi 3.1415926535897932384626433832795

class Node
{
private:
    string address;
    pair<double,double> position;
    int demand;
    double ready_time;
    double due_time;
    double arrival_time;
    double waiting_time;
    double service_time;
    friend class Route;
    friend class Solution;
    friend class FAGA;
public:
    Node();
    Node(string,pair<double,double>,int,double,double,double,double,double);
    static double get_travel_time(const Node,const Node);
    static double get_travel_distance(const Node,const Node);
    static void Node_setting(Node&,Node&);
    static void print_nodes(vector<Node>&);
    void print();
    void test();
    bool operator==(Node);
    bool operator!=(Node);
    bool operator<(Node);

    string get_address();
};

vector<Node> read_node(); //讀文字檔
double Radian(double); //度轉弧度
