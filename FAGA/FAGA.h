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
    FAGA(int,int,int,double, int);
    FAGA(int,int,int,double, int, string);
    void run_algo();
    void run_algo2();
    void run_algo3();
    void Crossover(vector<Solution>&);
    void Crossover3(vector<Solution>&);
    void Crossover3(vector<Solution>&, Solution&, Solution&);
    bool Mutate(Solution&);
    bool Mutate2(vector<Solution>&);
    bool Mutate3(vector<Solution>&);
    bool Mutate3(vector<Solution>&, Solution&, Solution&);
    int single_route_mutate(vector<Solution>&);
    int single_route_mutate();
    bool change(Route,Route,int,int);
    void solution_replace(const vector<Solution>&);
    void solution_replace3(const vector<Solution>&, Solution&, Solution&, const int&);
    void BCRC(const vector<Node>&,Solution,int,int,int,int);
    void BCRC(vector<Node>,Solution&);
    void test();
    void test2();
    static void cross_delete(const vector<Node>&,vector<Route>&);

    double get_total_distance_traveled();
    Solution get_solution();

    void brian_test();
    void permutation(Route&, set<string>&, Route, int&, int&, Route&);

    void clear_final_answer();
};

void analyze();
void analyze3(const string&);
void reset();
