#include "FAGA.h"

using namespace std;

int main()
{
    // double shortest_distance = -1.0;
    // Solution best_solution;


    vector<string> benchmark;
    benchmark.push_back("../benchmark/R108_100.txt");
    benchmark.push_back("../benchmark/R109_100.txt");
    benchmark.push_back("../benchmark/R110_100.txt");
    benchmark.push_back("../benchmark/R111_100.txt");
    benchmark.push_back("../benchmark/R112_100.txt");
    // // benchmark.push_back("../benchmark/RC1/RC107_100.txt");
    
    vector<string> result;
    result.push_back("../result/R1/R108_100.txt");
    result.push_back("../result/R1/R109_100.txt");
    result.push_back("../result/R1/R110_100.txt");
    result.push_back("../result/R1/R111_100.txt");
    result.push_back("../result/R1/R112_100.txt");
    // result.push_back("../result/RC1/RC107_100_result.txt");

    for(int j = 0; j < 5; ++j){
        for(int i=1;i<=5;i++)
        {
            cout<<i<<": ";
            FAGA ans(300,350,200,0.2, 3, benchmark[j]);
            // FAGA ans(300,350,1000,0.2, 2);
            if(i == 1) ans.clear_final_answer();
            ans.run_algo3();
            ans.test();
            // if(shortest_distance < 0 || shortest_distance > ans.get_total_distance_traveled()){
            //     best_solution = ans.get_solution();
            //     shortest_distance = ans.get_total_distance_traveled();
            // }
            cout<<"\n";
        }

        // analyze();
        analyze3(result[j]);
    }


    // best_solution.print(); 

    // Get the result file
    // ofstream outstream;
    // outstream.open("R101_25_result.txt");
    // if(!outstream.fail()){
    //     for(int i = 0; i < best_solution.size(); ++i) {
            // best_solution[i].print_file(outstream);
    //     }
    // }
    // else cout << "Failed to open file!\n";

    // return 0;


    // FAGA a(300,350,200,0.2, 2);
    // a.brian_test();
}
