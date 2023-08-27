#include "FAGA.h"

using namespace std;

int main()
{
    // double shortest_distance = -1.0;
    // vector<Route> best_solution;
    // for(int i=1;i<=10;i++)
    // {
    //     cout<<i<<": ";
    //     FAGA ans(50,500,200,0.2);
    //     ans.run_algo2();
    //     ans.test();
    //     if(shortest_distance < 0 || shortest_distance > ans.get_total_distance_traveled()){
    //         best_solution = ans.get_solution();
    //         shortest_distance = ans.get_total_distance_traveled();
    //     }
    //     cout<<"\n";
    // }
    // analyze();

    // // Get the result file
    // ofstream outstream;
    // outstream.open("R101_25_result.txt");
    // if(!outstream.fail()){
    //     for(int i = 0; i < best_solution.size(); ++i) {
    //         best_solution[i].print_file(outstream);
    //     }
    // }
    // else cout << "Failed to open file!\n";

    // return 0;


    FAGA a(50,100,500,0.2);
    a.brian_test();
}
