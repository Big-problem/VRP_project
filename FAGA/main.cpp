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
    for(int i=1;i<=10;i++)
    {
        cout<<i<<": ";
        FAGA ans(50,300,200,0.2);
        ans.run_algo();
        ans.test();
        cout<<"\n";
    }
    analyze();
    /*FAGA ans(50,500,200,0.2);
    ans.test();*/
    return 0;
}
