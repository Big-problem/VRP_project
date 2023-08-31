#include "population.h"

Population::Population(int z,int c,const vector<Node> &v)
{
    sol.clear();
    total_solution=z;
    sol=gen_population(z,c,v);
    crossover_probability=nullptr;
}

Population::Population()
{
    sol.clear();
    total_solution=0;
    crossover_probability=nullptr;
}

void Population::attribute_calculator() //計算適應度和Pc
{
    double dmax,dmin,RBmax,RBmin,a,b,c,total_AFV=0.0;
    int Kmax=INT_MIN,Kmin=INT_MAX;
    dmax=dmin=sol[0].total_dist_travelled;
    RBmax=RBmin=sol[0].route_balance;
    for(auto &i:sol)
    {
        dmax=max(dmax,i.total_dist_travelled);
        dmin=min(dmin,i.total_dist_travelled);
        Kmax=max(Kmax,i.total_routes);
        Kmin=min(Kmin,i.total_routes);
        RBmax=max(RBmax,i.route_balance);
        RBmin=min(RBmin,i.route_balance);
    }
    a=dmax-dmin,b=Kmax-Kmin,c=RBmax-RBmin;
    for(auto &i:sol)
    {
        i.F1v=(dmax-i.total_dist_travelled)/a;
        i.F2v=b?(Kmax-i.total_routes+0.0)/b:1.0;
        i.F3v=(RBmax-i.route_balance)/c;
        i.AFV=(i.F1v+i.F2v+i.F3v)/3;
        total_AFV+=i.AFV;
    }
    if(crossover_probability) delete []crossover_probability;
    crossover_probability=new double[total_solution];
    for(int i=0;i<total_solution;i++) crossover_probability[i]=sol[i].AFV/total_AFV;
}

void Population::sort() //以適應度排序解
{
    /*try
    {
        std::sort(sol.begin(),sol.end());
    }
    catch(const std::bad_alloc &e)
    {
        //print();
        exit(3);
    }*/
    std::sort(sol.begin(),sol.end());
}

void Population::print()
{
    for(auto &i:sol)
    {
        i.print();
        cout<<i.AFV<<" "<<i.F3v<<" "<<i.F1v<<" "<<i.F2v<<"\n";
    }
    cout<<"\n";
}

void Population::add_solution(Solution &s)
{
    sol.push_back(s);
    total_solution++;
}

void Population::print_best()
{
    attribute_calculator();
    sort();
    Solution best_solution = sol[total_solution-1];
    best_solution.print();

    // Get the result file
    ofstream outstream;
    outstream.open("../result/C1/C101_25_result.txt");
    if(!outstream.fail()){
        for(int i = 0; i < best_solution.routes.size(); ++i) {
            best_solution.routes[i].print_file(outstream);
        }
        outstream << "\n";
        outstream << "Total distance traveled: " << best_solution.total_dist_travelled << ", Number of vehicle: " << best_solution.total_routes << ", route balance: " << best_solution.route_balance << "\n";
    }
    else cout << "Failed to open file!\n";
}
