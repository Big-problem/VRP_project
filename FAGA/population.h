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
    void sort();
    void print();
};
