#include "Solution.h"

class Population
{
private:
    vector<Solution> sol;
    double *crossover_probability;
    int total_solution;
    int target;
    friend class FAGA;
public:
    Population(int,int,const vector<Node>&, int);
    Population();
    void attribute_calculator();
    void attribute_calculator2();
    void sort();
    void print();

    void add_solution(Solution&);
    void print_best();
};
