#include "route.h"

class Solution
{
private:
    vector<Route> routes;
    double total_dist_travelled;
    double route_balance;
    int target;
    double F1v;
    double F2v;
    double F3v;
    double AFV;
    int total_routes;
    friend class Population;
    friend class FAGA;
    // friend void BCRC(const vector<Node>&,Solution,int,int,int,int,int,double);
public:
    Solution();
    Solution(int);
    void add_new_route(Node,int,double);
    void gen_solution(int,const vector<Node>&);
    void attribute_calculator();
    void print();
    void test();
    void sort();
    void route_clear();
    bool operator<(Solution);
    //Solution& operator=(const Solution&);
    static bool better(const Solution&,const Solution&);

    int get_total_nodes();
};

pair<int,int> best_pos(const vector<tuple<double,int,int>>&);
vector<Solution> gen_population(int,int,const vector<Node>&);

vector<Solution> gen_population2(int,int,const vector<Node>&, int);
