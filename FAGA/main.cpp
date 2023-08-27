#include "FAGA.h"

using namespace std;

int main()
{
   /* vector<int> v;
    for(int i=0;i<10;i++)
    {
        v.emplace_back(i);
    }*/
    //vector<Node> v=read_node();
    //Node::print_nodes(v);
    /*Population a(3,600,1000000.0,v);
    a.attribute_calculator();
    a.print();
    a.sort();
    a.print();*/
    /*Solution s;
    s.gen_solution(200,1000000.0,v);
    s.print();*/
    /*Route a;
    for(int i=0;i<5;i++) a.add_node(i,v[i],2000,100000.0);
    a.print();
    a.remove();
    cout<<"\n";
    a.print();*/
    //system("FAGA.exe");

    // Start
    double shortest_distance = -1.0;
    vector<Route> best_solution;
    for(int i=1;i<=10;i++)
    {
        cout<<i<<": ";
        FAGA ans(50,500,200,0.2);
        ans.run_algo();
        ans.test();
        if(shortest_distance < 0 || shortest_distance > ans.get_total_distance_traveled()){
            best_solution = ans.get_solution();
            shortest_distance = ans.get_total_distance_traveled();
        }
        cout<<"\n";
    }
    analyze();

    // Get the result file
    ofstream outstream;
    outstream.open("R101_25_result.txt");
    if(!outstream.fail()){
        for(int i = 0; i < best_solution.size(); ++i) {
            best_solution[i].print_file(outstream);
        }
    }
    else cout << "Failed to open file!\n";

    /*FAGA ans(50,500,200,0.2);
    ans.test();*/
    return 0;
}
