#include "population.h"

class FAGA
{
private:
    vector<Node> node_list;
    Population sset;
    int capacity_limit;
    int generations;
    int pop_size;
    double tmax;
    double mutation_probability;
    Solution ans;
public:
    FAGA(int,int,int,double);
    void run_algo();
    void run_algo2();
    void Crossover(vector<Solution>&);
    bool Mutate(Solution&);
    int single_route_mutate(vector<Solution>&);
    bool change(Route,Route,int,int);
    void solution_replace(const vector<Solution>&);
    void BCRC(const vector<Node>&,Solution,int,int,int,int);
    void BCRC(vector<Node>,Solution&);
    void test();
    void test2();
    static void cross_delete(const vector<Node>&,vector<Route>&);

    double get_total_distance_traveled();
    Solution get_solution();

    void brian_test();
    void permutation(Route&, set<string>&, Route, int&, int&, Route&);
};

void analyze();
void reset();
