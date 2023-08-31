#include "Solution.h"

class Population
{
private:
    vector<Solution> sol;
    double *crossover_probability;
    int total_solution;
    friend class FAGA;
public:
    Population(int,int,const vector<Node>&);
    Population();
    void attribute_calculator();
    void attribute_calculator2(int);
    void sort();
    void print();

    void add_solution(Solution&);
    void print_best();
};
