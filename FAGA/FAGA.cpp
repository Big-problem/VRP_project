#include "FAGA.h"

int replace_quantity=10;
int fail_count=0;
vector<Solution> buffer;
Solution target;

vector<double> dist;
vector<int> K;
vector<double> RB;

FAGA::FAGA(int z,int g,int c,double pm)
{
    node_list=read_node();
    pop_size=z;
    sset=Population(z,c,node_list);
    capacity_limit=c;
    generations=g;
    tmax=node_list[0].due_time;
    mutation_probability=pm;
}

void FAGA::run_algo()
{
    cout<<"start\n";
    int crossover_time=sset.sol.size()/2;
    for(int i=1;i<=generations;i++)
    {
        vector<Solution> children;
        sset.attribute_calculator();
        for(int j=0;j<crossover_time;j++){
            Crossover(children);
        }
        //sset.sort();
        solution_replace(children);
        //cout<<i<<": Done\n";
    }
    sset.attribute_calculator();
    sset.sort();
    ans=sset.sol[sset.total_solution-1];
}

void FAGA::Crossover(vector<Solution> &children) //從舊解中以Pc為權重挑出2個解，在從這2個解的路線集合中個別挑出1條路線，去除彼此的節點後生成新子代
{
    vector<Solution> cross_pair=choices(sset.sol,sset.crossover_probability,nullptr,2); // 任挑兩解

    Route r0=choice(cross_pair[0].routes),r1=choice(cross_pair[1].routes); // 兩解中各挑一路線

    r0.nodes.erase(r0.nodes.begin()),r1.nodes.erase(r1.nodes.begin()); // 去掉起點
    cross_delete(r0.nodes,cross_pair[1].routes);
    cross_delete(r1.nodes,cross_pair[0].routes);

    BCRC(r0.nodes,cross_pair[1]);
    if(cross_pair[1].get_total_nodes() < 33) cout << "HELP\n";
    BCRC(r1.nodes,cross_pair[0]);
    if(cross_pair[0].get_total_nodes() < 33) cout << "HELP\n";

    /*cross_pair[0].sort(),cross_pair[1].sort();
    reset();
    BCRC(r0.nodes,cross_pair[1],0,0,1,0);
    if(buffer.empty()) return false;
    std::sort(buffer.begin(),buffer.end(),Solution::better); //排序找出RB最小的解
    Solution a=buffer[0],b;
    reset();
    BCRC(r1.nodes,cross_pair[0],0,0,1,0);
    if(buffer.empty()) return false;
    std::sort(buffer.begin(),buffer.end(),Solution::better);
    b=buffer[0];
    while(!Mutate(a));
    while(!Mutate(b));
    children.emplace_back(a),children.emplace_back(b);*/

    //while(!Mutate(cross_pair[0]));
    //while(!Mutate(cross_pair[1]));

    children.emplace_back(cross_pair[0]),children.emplace_back(cross_pair[1]);
}

void FAGA::BCRC(const vector<Node> &nodes,Solution cps,int ri,int ni,int pi,int inserted) //生成新解(Old version)
{
    if(fail_count>=1000) return;
    if(inserted==nodes.size()) //所有點都插入
    {
        cps.attribute_calculator();
        if(Solution::better(cps,target))
        {
            buffer.emplace_back(cps);
            fail_count=0;
            //cout<<cps.route_balance<<" "<<cps.total_dist_travelled<<" "<<target.route_balance<<" "<<target.total_dist_travelled<<"\n";
        }
        else fail_count++;
        return;
    }
    if(ri>=cps.routes.size()||ni>=nodes.size()||pi>cps.routes[ri].nodes.size()) return; //index超出範圍
    if(cps.routes[ri].add_node(pi,nodes[ni],capacity_limit,tmax)) //如果成功將點插入
    {
        BCRC(nodes,cps,ri,ni+1,1,inserted+1);
        cps.routes[ri].remove_node(nodes[ni]); //還原狀態
    }
    else fail_count++;
    BCRC(nodes,cps,ri,ni,pi+1,inserted); //更換插入位置
    //if(ri+1<cps.routes.size()&&nodes[ni].demand+cps.routes[ri+1].capacity<=capacity_limit)
    BCRC(nodes,cps,ri+1,ni,1,inserted); //更換插入路線
}

void FAGA::BCRC(vector<Node> nodes,Solution &cps) // (要補回的node, 一組解)
{
    while(!nodes.empty())
    {
        Node &n=rchoice(nodes); //隨機選取插入節點
        vector<tuple<double,int,int>> tmp;
        for(int i=0;i<cps.total_routes;i++) //每個路線
        {
            int jl=cps.routes[i].nodes.size();
            for(int j=1;j<=jl;j++) //每個插入點
            {
                if(cps.routes[i].add_node(j,n,capacity_limit,tmax))
                {
                    cps.attribute_calculator(); //計算total_dist_travelled
                    tmp.emplace_back(cps.total_dist_travelled,i,j); //紀錄total_dist_travelled、哪條路線、哪個位置
                    cps.routes[i].remove_node(n); //復原
                }
            }
        }
        if(tmp.empty()) //沒位置可插->開新路線
        {
            cps.add_new_route(node_list[0],capacity_limit,tmax);
            cps.routes[cps.total_routes-1].add_node(1,n,capacity_limit,tmax);
        }
        else
        {
            pair<int,int> pii=best_pos(tmp); //找到total_dist_travelled最小的位置
            cps.routes[pii.first].add_node(pii.second,n,capacity_limit,tmax); //插入
        }
        Remove(nodes,n);
    }
    cps.attribute_calculator();
    cps.route_clear(); //移除只剩起點的路線
}

void FAGA::cross_delete(const vector<Node> &v,vector<Route> &r) // 可以用空間換時間
{
    for(auto &node:v)
    {
        for(auto &route:r)
        {
            if(find(route.nodes.begin(),route.nodes.end(),node)!=route.nodes.end())
            {
                route.remove_node(node);
                break;
            }
        }
    }
}

bool FAGA::Mutate(Solution &s)
{
    if(decide(mutation_probability))
    {
        vector<Route> v=choices(s.routes,nullptr,nullptr,2); // 挑兩條路線
        if(v[0]==v[1]) return false;

        vector<Node> c0=v[0].nodes,c1=v[1].nodes;
        c0.erase(c0.begin()),c1.erase(c1.begin()); //刪除起點
        if(!c0.size()||!c1.size()) return false;

        Node n0=choice(c0),n1=choice(c1);
        int p0=find(v[0].nodes.begin(),v[0].nodes.end(),n0)-v[0].nodes.begin(), p1=find(v[1].nodes.begin(),v[1].nodes.end(),n1)-v[1].nodes.begin();
        if(!change(v[0],v[1],p0,p1)) return false; //檢查兩點位置是否可以互換
        auto p2=find(s.routes.begin(),s.routes.end(),v[0]), p3=find(s.routes.begin(),s.routes.end(),v[1]);
        p2->remove_node(n0),p3->remove_node(n1);
        p2->add_node(p0,n1,capacity_limit,tmax);
        p3->add_node(p1,n0,capacity_limit,tmax);
    }
    return true;
}

bool FAGA::change(Route r1,Route r2,int i1,int i2) //檢查兩點是否可以互換
{
    Node n1=*(r1.nodes.begin()+i1),n2=*(r2.nodes.begin()+i2);
    r1.remove_node(n1),r2.remove_node(n2);
    check=true;
    if(!r1.add_node(i1,n2,capacity_limit,tmax) || !r2.add_node(i2,n1,capacity_limit,tmax)) return false;
    return true;
}

void FAGA::solution_replace(const vector<Solution> &children) //將新子代加入當前解並去除部分舊解
{
    int len=children.size();
    //for(int i=0;i<len;i++) sset.sol.erase(sset.sol.begin());
    sset.sol.clear();
    for(int i=0;i<len;i++) sset.sol.emplace_back(children[i]);
}

void reset()
{
    buffer.clear();
    fail_count=0;
}

void FAGA::test()
{
    /*for(int i=1;i<=100;i++)
    {
        sset=Population(pop_size,capacity_limit,node_list);
        run_algo();
        cout<<i<<": ";
        ans.print();
        cout<<"\n";
    }*/
    dist.emplace_back(ans.total_dist_travelled);
    K.emplace_back(ans.total_routes);
    RB.emplace_back(ans.route_balance);
    ans.print();
}

void FAGA::test2()
{
    for(Route &r:ans.routes)
    {
        int len=r.nodes.size();
        for(int i=0;i<len;i++)
        {
            cout<<r.nodes[i].address<<": arrival = "<<rounding(r.nodes[i].arrival_time)<<", waiting = "<<rounding(r.nodes[i].waiting_time)<<", service = "<<rounding(r.nodes[i].service_time)<<", to next = ";
            if(i==len-1) cout<<rounding(Node::get_travel_time(r.nodes[i],r.nodes[0]));
            else cout<<rounding(Node::get_travel_time(r.nodes[i],r.nodes[i+1]));
            cout<<", ready = "<<r.nodes[i].ready_time<<", due = "<<r.nodes[i].due_time;
            cout<<"\n";
        }
        cout<<"\n";
    }
}

void analyze()
{
    double avd=0.0,avrb=0.0;
    int avk=0,len=dist.size();
    sort(dist.begin(),dist.end());
    sort(RB.begin(),RB.end());
    sort(K.begin(),K.end());
    for(int i=0;i<len;i++)
    {
        avd+=dist[i];
        avrb+=RB[i];
        avk+=K[i];
    }
    avd/=len,avrb/=len,avk/=len;
    cout<<"best:\n";
    cout<<"D = "<<dist[0]<<", K = "<<K[0]<<", RB = "<<RB[0]<<"\n\n";
    cout<<"average:\n";
    cout<<"D = "<<avd<<", K = "<<avk<<", RB = "<<avrb<<"\n\n\n\n";
}

void FAGA::get_result_file() {
    ofstream outstream;
    outstream.open("R101_25_result.txt");
    if(!outstream.fail()) {
        ans.get_solution_file(outstream);
    }
    else cout << "Failed to open file!\n";
}

double FAGA::get_total_distance_traveled()
{
    return ans.total_dist_travelled;
}

vector<Route> FAGA::get_solution()
{
    return ans.routes;
}
