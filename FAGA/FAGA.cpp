#include "FAGA.h"

int replace_quantity=10;
int fail_count=0;
vector<Solution> buffer;
Solution target;
Population final_answers;

vector<double> dist;
vector<int> K;
vector<double> RB;

int brian_property, brian_impossible;
int do_not_improve;

FAGA::FAGA(int z,int g,int c,double pm, int target_num)
{
    node_list=read_node();
    pop_size=z;
    sset=Population(z,c,node_list, target_num);
    capacity_limit=c;
    generations=g;
    tmax=node_list[0].due_time;
    mutation_probability=pm;

    final_answers.target = target_num;
}

FAGA::FAGA(int z,int g,int c,double pm, int target_num, string benchmark)
{
    node_list=read_node3(benchmark);
    pop_size=z;
    sset=Population(z,c,node_list, target_num);
    capacity_limit=c;
    generations=g;
    tmax=node_list[0].due_time;
    mutation_probability=pm;

    final_answers.target = target_num;
}

void FAGA::run_algo()
{
    cout<<"start\n";
    int crossover_time=sset.sol.size()/2;
    for(int i=1;i<=generations;i++)
    {
        vector<Solution> children;
        sset.attribute_calculator2();
        
        // sset.sort();

        for(int j=0;j<crossover_time;j++){
            Crossover(children);
        }
        //sset.sort();
        solution_replace(children);
        //cout<<i<<": Done\n";
    }
    sset.attribute_calculator2();
    sset.sort();
    // for(int i = 0; i < 5; ++i){
    //     // children.push_back(children_tmp[solution_quantity-1]); // 每次都保留前三好的
    //     cout << "Go: " <<i<<"\n";
    //     sset.sol[sset.total_solution-1-i].print();
    //     // tmp_solution_quantity++;
    // }
    ans=sset.sol[sset.total_solution-1];
    if(sset.target == 2) cout << "Single mutate: " <<single_route_mutate()<<"\n";
}

void FAGA::run_algo2()
{
    brian_property = 0;
    brian_impossible = 0;

    // cout<<"start\n";
    int solution_quantity = sset.total_solution; // 每個generation要產生這麼多組解

    // int abc = 0, def = 0, ghi = 0;

    // vector<Solution> children_tmp;
    for(int i=1;i<=generations;i++)
    {
        int tmp_solution_quantity = 0;
        vector<Solution> children;
        sset.attribute_calculator2();
        // sset.sort();
        // if(i > 1){
            // children.push_back(children_tmp[solution_quantity-1]); // 每次都保留前三好的
            // cout << "Go: " <<i<<"\n";
            // sset.sol[sset.total_solution-1].print();
            // tmp_solution_quantity++;
        // }
        double a=0.9;
        int no_other_sol = 0;
        while(tmp_solution_quantity < solution_quantity) {
            double method = random();
            if(tmp_solution_quantity <= solution_quantity-2 && method <= a) {
                Crossover(children);
                tmp_solution_quantity += 2;
                // def++;
            }
            else{ // 沒成功就直接取消
                if(Mutate2(children)){
                    tmp_solution_quantity++;
                }
                else if(tmp_solution_quantity == solution_quantity-1) { // do not find a better solution
                    no_other_sol++;
                    if(no_other_sol >= 100){
                        // children.push_back(sset.sol[sset.total_solution-1]); // 再放原本的最佳解
                        // children.push_back(children_tmp[solution_quantity-1]);
                        if(final_answers.total_solution == 0){
                            children.push_back(sset.sol[sset.total_solution-1]);
                        }
                        else children.push_back(final_answers.sol[final_answers.total_solution-1]);
                        // cout << "SAD!!\n";
                        tmp_solution_quantity++;
                    }
                }
                // abc++;
            }
        }
        // for(int j=0;j<crossover_time;j++){
        //     Crossover(children);
        // }
        //sset.sort();
        solution_replace(children);
        // children_tmp = children;
        // sort(children_tmp.begin(),children_tmp.end());
        // cout <<"TMP\n";
        // for(int i = 0;i  < 5; ++i) children_tmp[solution_quantity-1-i].print();
        // cout<<"TMP DONE\n";
        //cout<<i<<": Done\n";
    }
    sset.attribute_calculator2();
    sset.sort();
    ans=sset.sol[sset.total_solution-1];
    if(sset.target == 2) cout << "Single mutate: " <<single_route_mutate()<<"\n";
    // cout << "ccc: " << abc << ", def: "<<def<< ", ghi: "<<ghi<<"\n";
    // cout << "Brian property: " << brian_property <<", impossible: "<<brian_impossible <<"\n";
}

void FAGA::run_algo3()
{
    // final_answers.sol.clear();
    // final_answers.total_solution = 0;
    // final_answers.crossover_probability=nullptr;
    dist.clear();
    K.clear();
    RB.clear();
    // cout << "Start\n";
    int aa=0, bb=0, cc=0, dd=0, ee = 0;

    int solution_quantity = sset.total_solution; // 每個generation要產生這麼多組解
    Solution global_best_route_distance, global_best_car_num;
    global_best_route_distance.total_dist_travelled = 1000000000.0;
    global_best_car_num.total_routes = 1000000000;
    do_not_improve = 0;
    sset.attribute_calculator2();
    for(int i = 0; i < generations; i++)
    {
        int tmp_solution_quantity = 0;
        vector<Solution> children;
        children.clear();
        Solution local_best;
        local_best.total_dist_travelled = 1000000000.0;
        
        // sset.attribute_calculator2();

        // if(i == 200){
        //     for(int i = 0; i < 200; ++i) cout << sset.crossover_probability[i] << " ";
        // }
        // if(do_not_improve >= 15){ // 連續5次沒有優化 隨機生成最多50組解
        //     for(int j = 0; j < do_not_improve && j < 40; ++j){
                // Solution new_sol(final_answers.target);
                // new_sol.gen_solution(capacity_limit, node_list);
                // children.push_back(new_sol);
            
                // if(new_sol.total_dist_travelled < local_best.total_dist_travelled){
                //     local_best = new_sol;
                // }
        //         // ee++;
        //         tmp_solution_quantity++;
        //     }
        // }
        if(i > 0){ // 保留最佳的父代
            children.push_back(global_best_route_distance);
            children.push_back(global_best_car_num);
            tmp_solution_quantity += 2;
        }
        // double a=0.70, b=0.80, c=0.9; // Dataset 2
        double a=0.80, b=0.90, c=0.9; // Dataset 1
        int no_other_sol = 0;
        while(tmp_solution_quantity < solution_quantity) {
            double method = random();
            if(method <= a) { // Crossover
                if(i == 0) Crossover3(children);
                else Crossover3(children, global_best_route_distance, global_best_car_num);
                aa++;
                tmp_solution_quantity++;
            }
            // else if(/*i > 100 && */method > a && method <= b){ // new solution
            //     bb++;
            //     Solution new_sol(final_answers.target);
            //     new_sol.gen_solution(capacity_limit, node_list);
            //     children.push_back(new_sol);
            
            //     if(new_sol.total_dist_travelled < local_best.total_dist_travelled){
            //         local_best = new_sol;
            //     }
            //     tmp_solution_quantity++;
                // if(i == 149) cout << "dfdfdf: "<<tmp_solution_quantity << "\n";
                // else{
                //     Solution new_sol;
                //     new_sol.gen_solution(capacity_limit, node_list);
                //     children.push_back(new_sol);
                //
                //     if(new_sol.total_dist_travelled < local_best.total_dist_travelled){
                //         local_best = new_sol;
                //     }
                    // ee++;
                // }
            // }
            else if(method > a && method <= b){ // Original solution
                children.push_back(sset.sol[tmp_solution_quantity]);
                tmp_solution_quantity++;
                cc++;
            }
            else{
                int tmp_count = 0;
                if(i == 0){
                    while(tmp_count < 100 && !Mutate3(children)){ // Mutation
                        tmp_count++;
                    }
                }
                else{
                    while(tmp_count < 100 && !Mutate3(children, global_best_route_distance, global_best_car_num)){ // Mutation
                        tmp_count++;
                    }
                }
                if(tmp_count >= 100) continue;
                tmp_solution_quantity++;
                dd++;
            }
            // else if(local_best.total_dist_travelled != 1000000000.0 && method > b && method <= c){ // local_best
            //     children.push_back(local_best);
            //     cc++;
            //     tmp_solution_quantity++;
            // }
            // else if(i > 0 && method > c){ // global_best
            //     children.push_back(global_best);
            //     if(global_best.total_dist_travelled < local_best.total_dist_travelled) local_best = global_best;
            //     tmp_solution_quantity++;
            //     dd++;
            // }
        }
        // for(auto &kk:children) kk.print();
        solution_replace3(children, global_best_route_distance, global_best_car_num, i);
        sset.attribute_calculator2();
        // for(auto &kk : sset.sol) kk.print();
        cout << i << " Done\n";
    }
    // cout << aa << " " << bb << " " << cc << " " << dd << " " << ee << "\n";
    // cout << "A" << "\n";
    sset.attribute_calculator2();
    sset.sort();
    // cout << "B" << "\n";
    // cout << sset.sol.size() << " " << sset.total_solution << "\n";
    ans=sset.sol[sset.total_solution-1];
    // cout << "C" << "\n";
    // ans.print();
    // cout << "\n";
    if(sset.target == 2) cout << "Single mutate: " <<single_route_mutate()<<"\n";

}

void FAGA::Crossover(vector<Solution> &children) //從舊解中以Pc為權重挑出2個解，在從這2個解的路線集合中個別挑出1條路線，去除彼此的節點後生成新子代
{
    // vector<Solution> cross_pair=choices(sset.sol,sset.crossover_probability,nullptr,2); // 任挑兩解
    vector<Solution> cross_pair=sset.select_solution(2); // 任挑兩解

    Route r0=choice(cross_pair[0].routes),r1=choice(cross_pair[1].routes); // 兩解中各挑一路線

    r0.nodes.erase(r0.nodes.begin()),r1.nodes.erase(r1.nodes.begin()); // 去掉起點
    cross_delete(r0.nodes,cross_pair[1].routes);
    cross_delete(r1.nodes,cross_pair[0].routes);

    BCRC(r0.nodes,cross_pair[1]);
    // if(cross_pair[1].get_total_nodes() < 33) cout << "HELP\n";
    BCRC(r1.nodes,cross_pair[0]);
    // if(cross_pair[0].get_total_nodes() < 33) cout << "HELP\n";

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

    // while(!Mutate(cross_pair[0]));
    // while(!Mutate(cross_pair[1]));

    children.emplace_back(cross_pair[0]),children.emplace_back(cross_pair[1]);
}

void FAGA::Crossover3(vector<Solution> &children) //從舊解中以Pc為權重挑出2個解，在從這2個解的路線集合中個別挑出1條路線，去除彼此的節點後生成新子代
{
    // vector<Solution> cross_pair=choices(sset.sol,sset.crossover_probability,nullptr,2); // 任挑兩解
    double method = random();
    vector<Solution> cross_pair; // 任挑兩解
    if(method <= 0.8) cross_pair=sset.select_solution(2);
    else{
        cross_pair=sset.select_solution(1);
        Solution new_solution(final_answers.target);
        new_solution.gen_solution(capacity_limit, node_list);
        cross_pair.push_back(new_solution);
    }

    Route r0=choice(cross_pair[0].routes),r1=choice(cross_pair[1].routes); // 兩解中各挑一路線

    r0.nodes.erase(r0.nodes.begin()),r1.nodes.erase(r1.nodes.begin()); // 去掉起點
    cross_delete(r0.nodes,cross_pair[1].routes);
    cross_delete(r1.nodes,cross_pair[0].routes);

    BCRC(r0.nodes,cross_pair[1]);
    BCRC(r1.nodes,cross_pair[0]);

    if(cross_pair[0].total_dist_travelled < cross_pair[1].total_dist_travelled) children.emplace_back(cross_pair[0]);
    else children.emplace_back(cross_pair[1]);
}

void FAGA::Crossover3(vector<Solution> &children, Solution &global_best_route_distance, Solution  &global_best_car_num) //從舊解中以Pc為權重挑出2個解，在從這2個解的路線集合中個別挑出1條路線，去除彼此的節點後生成新子代
{
    // vector<Solution> cross_pair=choices(sset.sol,sset.crossover_probability,nullptr,2); // 任挑兩解
    double method = random();
    vector<Solution> cross_pair; // 任挑兩解
    if(method <= 0.8) cross_pair=sset.select_solution(2);
    else if(method <= 0.9){
        cross_pair=sset.select_solution(1);
        Solution new_solution(final_answers.target);
        new_solution.gen_solution(capacity_limit, node_list);
        cross_pair.push_back(new_solution);
    }
    else if(method <= 0.95){
        cross_pair=sset.select_solution(1);
        cross_pair.push_back(global_best_route_distance);
    }
    else{
        cross_pair=sset.select_solution(1);
        cross_pair.push_back(global_best_car_num);
    }
    

    Route r0=choice(cross_pair[0].routes),r1=choice(cross_pair[1].routes); // 兩解中各挑一路線

    r0.nodes.erase(r0.nodes.begin()),r1.nodes.erase(r1.nodes.begin()); // 去掉起點
    cross_delete(r0.nodes,cross_pair[1].routes);
    cross_delete(r1.nodes,cross_pair[0].routes);

    BCRC(r0.nodes,cross_pair[1]);
    BCRC(r1.nodes,cross_pair[0]);

    if(cross_pair[0].total_dist_travelled < cross_pair[1].total_dist_travelled){
        children.emplace_back(cross_pair[0]);
        // if(cross_pair[0].total_dist_travelled < local_best.total_dist_travelled){
        //     local_best = cross_pair[0];
        // }
    }
    else{
        children.emplace_back(cross_pair[1]);
        // if(cross_pair[1].total_dist_travelled < local_best.total_dist_travelled){
        //     local_best = cross_pair[1];
        // }
    }

    // if(cross_pair[0].total_dist_travelled < local_best.total_dist_travelled){
    //     local_best = cross_pair[0];
    // }
    // if(cross_pair[1].total_dist_travelled < local_best.total_dist_travelled){
    //     local_best = cross_pair[1];
    // }

    // children.emplace_back(cross_pair[0]),children.emplace_back(cross_pair[1]);
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
        int p0=find(v[0].nodes.begin(),v[0].nodes.end(),n0)-v[0].nodes.begin(), p1=find(v[1].nodes.begin(),v[1].nodes.end(),n1)-v[1].nodes.begin(); // 找n0, n1的位置
        if(!change(v[0],v[1],p0,p1)) return false; //檢查兩點位置是否可以互換
        auto p2=find(s.routes.begin(),s.routes.end(),v[0]), p3=find(s.routes.begin(),s.routes.end(),v[1]);
        p2->remove_node(n0),p3->remove_node(n1);
        p2->add_node(p0,n1,capacity_limit,tmax);
        p3->add_node(p1,n0,capacity_limit,tmax);
    }
    return true;
}

bool FAGA::Mutate2(vector<Solution> &children)
{
    Solution s = choice(sset.sol);
    // s.print();
    // if(decide(mutation_probability))
    // {
    vector<Route> v=choices(s.routes,nullptr,nullptr,2); // 挑兩條路線
    if(v[0]==v[1]) return false;

    vector<Node> c0=v[0].nodes,c1=v[1].nodes;
    c0.erase(c0.begin()),c1.erase(c1.begin()); //刪除起點
    if(!c0.size()||!c1.size() || (c0.size()==c1.size()&&c1.size()==1)) return false;

    Node n0=choice(c0),n1=choice(c1);
    int p0=find(v[0].nodes.begin(),v[0].nodes.end(),n0)-v[0].nodes.begin(), p1=find(v[1].nodes.begin(),v[1].nodes.end(),n1)-v[1].nodes.begin(); // 找n0, n1的位置
    if(!change(v[0],v[1],p0,p1)) return false; //檢查兩點位置是否可以互換
    auto p2=find(s.routes.begin(),s.routes.end(),v[0]), p3=find(s.routes.begin(),s.routes.end(),v[1]);
    p2->remove_node(n0),p3->remove_node(n1);
    p2->add_node(p0,n1,capacity_limit,tmax);
    p3->add_node(p1,n0,capacity_limit,tmax);
    // }
    children.push_back(s);
    // s.attribute_calculator();
    // s.print();
    return true;
}

bool FAGA::Mutate3(vector<Solution> &children)
{
    vector<Solution> cross_pair=sset.select_solution(1); // 任挑一解

    Solution s = cross_pair[0]; 

    vector<Route> v=choices(s.routes,nullptr,nullptr,2); // 挑兩條路線
    if(v[0]==v[1]) return false;

    vector<Node> c0=v[0].nodes,c1=v[1].nodes;
    c0.erase(c0.begin()),c1.erase(c1.begin()); //刪除起點
    if(!c0.size()||!c1.size() || (c0.size()==c1.size()&&c1.size()==1)) return false;

    Node n0=choice(c0),n1=choice(c1);
    int p0=find(v[0].nodes.begin(),v[0].nodes.end(),n0)-v[0].nodes.begin(), p1=find(v[1].nodes.begin(),v[1].nodes.end(),n1)-v[1].nodes.begin(); // 找n0, n1的位置
    if(!change(v[0],v[1],p0,p1)) return false; //檢查兩點位置是否可以互換
    auto p2=find(s.routes.begin(),s.routes.end(),v[0]), p3=find(s.routes.begin(),s.routes.end(),v[1]);
    p2->remove_node(n0),p3->remove_node(n1);
    p2->add_node(p0,n1,capacity_limit,tmax);
    p3->add_node(p1,n0,capacity_limit,tmax);

    s.attribute_calculator(); // Important!!!

    children.push_back(s);

    return true;
}

bool FAGA::Mutate3(vector<Solution> &children, Solution &global_best_route_distance, Solution  &global_best_car_num)
{
    // vector<Solution> cross_pair=choices(sset.sol,sset.crossover_probability,nullptr,1); // 任挑一解
    // vector<Solution> cross_pair=sset.select_solution(1); // 任挑一解
    double method = random();
    vector<Solution> cross_pair; // 任挑一解
    if(method <= 0.8) cross_pair=sset.select_solution(1);
    else if(method <= 0.9) cross_pair.push_back(global_best_route_distance);
    else cross_pair.push_back(global_best_car_num);

    Solution s = cross_pair[0]; 

    vector<Route> v=choices(s.routes,nullptr,nullptr,2); // 挑兩條路線
    if(v[0]==v[1]) return false;

    vector<Node> c0=v[0].nodes,c1=v[1].nodes;
    c0.erase(c0.begin()),c1.erase(c1.begin()); //刪除起點
    if(!c0.size()||!c1.size() || (c0.size()==c1.size()&&c1.size()==1)) return false;

    Node n0=choice(c0),n1=choice(c1);
    int p0=find(v[0].nodes.begin(),v[0].nodes.end(),n0)-v[0].nodes.begin(), p1=find(v[1].nodes.begin(),v[1].nodes.end(),n1)-v[1].nodes.begin(); // 找n0, n1的位置
    if(!change(v[0],v[1],p0,p1)) return false; //檢查兩點位置是否可以互換
    auto p2=find(s.routes.begin(),s.routes.end(),v[0]), p3=find(s.routes.begin(),s.routes.end(),v[1]);
    p2->remove_node(n0),p3->remove_node(n1);
    p2->add_node(p0,n1,capacity_limit,tmax);
    p3->add_node(p1,n0,capacity_limit,tmax);

    s.attribute_calculator(); // Important!!!

    children.push_back(s);
    
    // if(s.total_dist_travelled < local_best.total_dist_travelled){
    //     local_best = s;
    // }

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
    for(int i=0;i<len;i++)sset.sol.emplace_back(children[i]);
}

void FAGA::solution_replace3(const vector<Solution> &children, Solution &global_best_route_distance, Solution &global_best_car_num, const int &generations) //將新子代加入當前解並去除部分舊解
{
    int len=children.size();
    sset.sol.clear();
    for(int i=0;i<len;i++){
        sset.sol.emplace_back(children[i]);
        if(children[i].total_dist_travelled < global_best_route_distance.total_dist_travelled){
            global_best_route_distance = children[i];
        }
        if(children[i].total_routes < global_best_car_num.total_routes || (children[i].total_routes == global_best_car_num.total_routes && children[i].total_dist_travelled < global_best_car_num.total_dist_travelled)){
            global_best_car_num = children[i];
        }
    }


    cout << "global_route: ";
    global_best_route_distance.print();
    cout << "global_car_num: ";
    global_best_car_num.print();
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

    final_answers.add_solution(ans);
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

    cout << "Final answer: \n";
    final_answers.print_best();

}

void analyze3(const string &result)
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

    cout << "Final answer: \n";
    final_answers.print_best3(result);
}

double FAGA::get_total_distance_traveled()
{
    return ans.total_dist_travelled;
}

Solution FAGA::get_solution()
{
    return ans;
}


void FAGA::brian_test(){
    run_algo3();
    ans.print();
    cout << "BBBBBBBBBBBBBBBBBBBBBBBBBBB: " << single_route_mutate() << "\n";
    ans.print();
    cout << "TESTing...\n";
    Solution ss = ans;
    ss.print();
    // cout << "EEEEEEEEEEEEEEEEEEEEEEEEE: "<<sset.target<<"\n";
    // sset.attribute_calculator2();
    // sset.sort();
    // for(int i = 0; i < 30; ++i){
    //     // children.push_back(children_tmp[solution_quantity-1]); // 每次都保留前三好的
    //     cout << "Go: " <<i<<"\n";
    //     sset.sol[sset.total_solution-1-i].print();
    //     // tmp_solution_quantity++;
    // }

    // vector<Solution> children;
    
    // for(int i = 0; i < 20; ++i)single_route_mutate(children);
    // single_route_mutate(children);
    // cout << Mutate2(children);
    // Solution new_solution;
    // cout << "\n\n";
    // new_solution.gen_solution(capacity_limit, node_list);
    // new_solution.print();
}

int FAGA::single_route_mutate(vector<Solution> &children) { // 看要不要每條都permutation
    Solution child = choice(sset.sol); // 先選一組解
    // cout << "Initial soluttion: ";
    // child.print();
    Route mutate_route;
    int mutate_route_index = -1;
    int too_many_times = 0;
    while(mutate_route.total_nodes <= 2) { // 至少一路線node數 > 2, 不然會無限迴圈
        mutate_route_index = floor(random()*child.total_routes);
        mutate_route = child.routes[mutate_route_index]; // 挑要重新排列的路線 (node數 > 2)
        // cout << "Mutate route index: " << mutate_route_index << "\n";
        too_many_times++;
        if(too_many_times >= 100){ // 選不到合法的route, 直接放相同的解
            children.push_back(child);
            brian_impossible++;
            brian_property++;
            return 1;
        }
    }

    // cout << "The route been chosen: ";
    // mutate_route.print();
    // cout << mutate_route.total_distance() << "\n";

    int mutate_num = min(4, mutate_route.total_nodes-1); // 決定要重新排列幾個
    int lower = 1, upper = mutate_route.total_nodes - mutate_num;
    int start_index = floor(random()*upper) + lower; // 從start_index開始重排
    // cout << "Start index: " << start_index << ", " << "Mutate num: " << mutate_num << "\n";
    // cout << "Done\n";
    // mutate_route.print();
    // 對route.nodes做permutation
    Route mutate_ans, tmp;
    set<string> s;
    for(int i = 0; i < start_index; ++i) {
        tmp.add_node_at_last(mutate_route.nodes[i], capacity_limit, tmax);
    }
    // cout << "Initial tmp: ";
    // tmp.print();

    permutation(mutate_route, s, tmp, start_index, mutate_num, mutate_ans);
    
    // cout << "Mutate ans: ";
    // mutate_ans.print();
    // cout << mutate_ans.total_distance() << "\n";

    if(mutate_route.total_distance() <= mutate_ans.total_distance()){
        return 0;
    }

    child.routes[mutate_route_index] = mutate_ans;
    child.attribute_calculator();
    // cout << "Updated solutoion\n";
    // child.print();
    
    children.push_back(child);
    brian_property++;
    return 1;

}

int FAGA::single_route_mutate() { // 對最終解排列組合
    // Solution child = choice(sset.sol); // 先選一組解
    // cout << "Initial soluttion: ";
    // child.print();
    // Route mutate_route;
    // int mutate_route_index = -1;
    // int too_many_times = 0;
    // while(mutate_route.total_nodes <= 2) { // 至少一路線node數 > 2, 不然會無限迴圈
    //     mutate_route_index = floor(random()*ans.total_routes);
    //     mutate_route = ans.routes[mutate_route_index]; // 挑要重新排列的路線 (node數 > 2)
    //     // cout << "Mutate route index: " << mutate_route_index << "\n";
    //     too_many_times++;
    //     if(too_many_times >= 100){ // 選不到合法的route, 直接放相同的解
    //         // children.push_back(child);
    //         // brian_impossible++;
    //         // brian_property++;
    //         return 0;
    //     }
    // }

    Route mutate_route;
    int abcdefg = 0;
    for(int i = 0; i < ans.total_routes; ++i){ // 每條都permutation
        // cout << "Route " << i << " start!\n";
        if(ans.routes[i].total_nodes <= 2) continue; // 路線node數要 > 2
        
        mutate_route = ans.routes[i];
        int mutate_num = min(7, mutate_route.total_nodes-1); // 決定要重新排列幾個
        int lower = 1, upper = mutate_route.total_nodes - mutate_num;
        int start_index = floor(random()*upper) + lower; // 從start_index開始重排
        // cout << "Start index: " << start_index << ", " << "Mutate num: " << mutate_num << "\n";
        // cout << "Done\n";
        // mutate_route.print();
        // 對route.nodes做permutation
        Route mutate_ans, tmp;
        set<string> s;
        for(int i = 0; i < start_index; ++i) {
            tmp.add_node_at_last(mutate_route.nodes[i], capacity_limit, tmax);
        }
        // cout << "Initial tmp: ";
        // tmp.print();

        permutation(mutate_route, s, tmp, start_index, mutate_num, mutate_ans);
        
        // cout << "Mutate ans: ";
        // mutate_ans.print();
        // cout << mutate_ans.total_distance() << "\n";

        if(mutate_route.total_distance() <= mutate_ans.total_distance()) continue;
        // cout << "Route " << i << " improved!\n";
        ans.routes[i] = mutate_ans;
        abcdefg++;
        
        // cout << "Updated solutoion\n";
        // child.print();
        
        // children.push_back(child);
        // brian_property++;
        // return 1;
    }
    ans.attribute_calculator();
    return abcdefg;
    // cout << "The route been chosen: ";
    // mutate_route.print();
    // cout << mutate_route.total_distance() << "\n";

    // int mutate_num = min(7, mutate_route.total_nodes-1); // 決定要重新排列幾個
    // int lower = 1, upper = mutate_route.total_nodes - mutate_num;
    // int start_index = floor(random()*upper) + lower; // 從start_index開始重排
    // // cout << "Start index: " << start_index << ", " << "Mutate num: " << mutate_num << "\n";
    // // cout << "Done\n";
    // // mutate_route.print();
    // // 對route.nodes做permutation
    // Route mutate_ans, tmp;
    // set<string> s;
    // for(int i = 0; i < start_index; ++i) {
    //     tmp.add_node_at_last(mutate_route.nodes[i], capacity_limit, tmax);
    // }
    // // cout << "Initial tmp: ";
    // // tmp.print();

    // permutation(mutate_route, s, tmp, start_index, mutate_num, mutate_ans);
    
    // // cout << "Mutate ans: ";
    // // mutate_ans.print();
    // // cout << mutate_ans.total_distance() << "\n";

    // if(mutate_route.total_distance() <= mutate_ans.total_distance()){
    //     return 0;
    // }

    // ans.routes[mutate_route_index] = mutate_ans;
    // ans.attribute_calculator();
    // // cout << "Updated solutoion\n";
    // // child.print();
    
    // // children.push_back(child);
    // // brian_property++;
    // return 1;

}

void FAGA::permutation(Route &target_route, set<string> &s, Route tmp, int &start, int &count, Route &mutate_ans) {
    if(tmp.total_nodes == start+count) {
		for(int i = start+count; i < target_route.total_nodes; ++i){
            if(!tmp.add_node_at_last(target_route.nodes[i], capacity_limit, tmax)) return;
        }

        // 所有node都加完了
		// cout << "Tmp ans: ";
        // tmp.print();

        if(mutate_ans.total_travel_time == 0 || mutate_ans.total_distance() > tmp.total_distance()) {
            mutate_ans = tmp; // 新路線travel_distance短就替換
        }
		return;
	}
	// cout << "go\n";
	for(int i = start; i < start+count; ++i) {
		// cout << "in\n";
		if(s.find(target_route.nodes[i].address) == s.end()){
			// cout << "add\n";
            if(tmp.add_node_at_last(target_route.nodes[i], capacity_limit, tmax)) {
                s.insert(target_route.nodes[i].address);
                permutation(target_route, s, tmp, start, count, mutate_ans);
                s.erase(target_route.nodes[i].address);
                tmp.remove_node(target_route.nodes[i]);
            }
            // else{
            //     cout << "Illegal: ";
            //     tmp.print();
            //     target.nodes[i].print();
            //     cout << "\n";
            // }
		}
	}
}

void FAGA::clear_final_answer(){
    final_answers.sol.clear();
    final_answers.total_solution = 0;
    final_answers.crossover_probability=nullptr;
}
