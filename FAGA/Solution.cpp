#include "Solution.h"

Solution::Solution() // constructor
{
    routes.clear();
    total_dist_travelled=0.0;
    total_routes=0;
}

Solution::Solution(int target_num) // constructor
{
    routes.clear();
    total_dist_travelled=0.0;
    total_routes=0;
    target = target_num;
}

void Solution::add_new_route(Node start,int c,double t) // 新增路線到解答
{
    Route new_route;
    new_route.add_node(0,start,c,t);
    routes.emplace_back(new_route);
    total_routes++;
}

void Solution::attribute_calculator() //計算D(total distance travelled by all vehicles)、RB(route balance)
{
    double dist, dmax = total_dist_travelled = 0.0, dmin = 10000000000.0;
    for(auto &route:routes)
    {
        if(route.nodes.size()==1) continue;
        dist=route.total_distance();
        dmax=max(dmax,dist);
        dmin=min(dmin,dist);
        total_dist_travelled+=dist;
    }
    route_balance=dmax-dmin;
}

void Solution::gen_solution(int c,const vector<Node> &node_list) //生成初始解
{
    vector<Node> customer_list = node_list;
    bool add_route;
    customer_list.erase(customer_list.begin()); //去掉起點

    add_new_route(node_list[0],c,node_list[0].due_time);
    /*Node node_to_add=choice(customer_list);
    Remove(customer_list,node_to_add);
    routes[0].add_node(1,node_to_add,c,node_list[0].due_time);
    for(auto &node:customer_list)
    {
        add_route=true;
        for(auto &route:routes)
        {
            if(route.add_node(route.total_nodes,node,c,node_list[0].due_time))
            {
                add_route=false;
                break;
            }
        }
        if(add_route)
        {
            add_new_route(node_list[0],c,node_list[0].due_time);
            routes[total_routes-1].add_node(1,node,c,node_list[0].due_time);
        }
    }*/
    int n=node_list.size()-1; //所有節點隨機挑選
    for(int i=0;i<n;i++)
    {
        add_route=true;
        Node node_to_add=choice(customer_list);
        for(auto &route:routes)
        {
            if(route.add_node(route.total_nodes,node_to_add,c,node_list[0].due_time))
            {
                add_route=false;
                break;
            }
        }
        if(add_route)
        {
            add_new_route(node_list[0],c,node_list[0].due_time);
            routes[total_routes-1].add_node(1,node_to_add,c,node_list[0].due_time);
        }
        Remove(customer_list,node_to_add);
    }
    attribute_calculator();
}

vector<Solution> gen_population(int z,int c,const vector<Node> &v) // generate z numbers of initial solutions
{
    vector<Solution> result(z);
    for(int i=0;i<z;i++)
    {
        result[i]=Solution();
        result[i].gen_solution(c,v);
    }
    return result;
}

vector<Solution> gen_population2(int z,int c,const vector<Node> &v, int target) // generate z numbers of initial solutions
{
    vector<Solution> result(z);
    for(int i=0;i<z;i++)
    {
        result[i]=Solution(target);
        result[i].gen_solution(c,v);
        // result[i].print();
    }
    // cout << 123456789<<"\n";
    // for(auto i:result) i.print();
    return result;
}

void Solution::print()
{
    cout<<"\n";
    for(int i=0;i<total_routes;i++)
    {
        // routes[i].print();
        //cout<<routes.size()<<" "<<total_routes<<"\n";
        //cout<<"\n";
    }
    cout<<"AFV: "<<AFV<<", total dist= "<<total_dist_travelled<<", total routes= "<<total_routes<<", RB= "<<route_balance<<"\n";
}

bool Solution::operator<(Solution b)
{
    //return AFV<b.AFV || F3v<b.F3v || F1v<b.F1v || F2v<b.F2v;
    if(target == 3){
        if(AFV==b.AFV&&F3v==b.F3v&&F1v==b.F1v) return F2v<b.F2v;
        if(AFV==b.AFV&&F1v==b.F1v) return F3v<b.F3v;
        if(AFV==b.AFV) return F1v<b.F1v;
        return AFV<b.AFV;
    }
    else if(target == 2){
        if(AFV==b.AFV&&F1v==b.F1v) return F2v<b.F2v;
        if(AFV==b.AFV) return F1v<b.F1v;
        return AFV<b.AFV;
    }
    return AFV<b.AFV;
}

bool Solution::better(const Solution &a,const Solution&b) //判斷a是否比b好 (應該都註解掉了所以沒改)
{
    if(a.route_balance==b.route_balance&&a.total_dist_travelled==b.total_dist_travelled) return a.total_routes<b.total_routes;
    if(a.route_balance==b.route_balance) return a.total_dist_travelled<b.total_dist_travelled;
    return a.route_balance<b.route_balance;
}

// Solution &Solution::operator=(const Solution &b)
// {
//     routes=b.routes;
//     total_dist_travelled=b.total_dist_travelled;
//     total_routes=b.total_routes;
//     F1v=b.F1v,F2v=b.F2v,F3v=b.F3v,AFV=b.AFV;
//     route_balance=b.route_balance;
// }

void Solution::sort() //以總負重排序路線
{
    std::sort(routes.begin(),routes.end());
}

void Solution::route_clear()
{
    vector<int> dummy;
    for(int i=0;i<total_routes;i++)
    {
        if(routes[i].nodes.size()==1) dummy.emplace_back(i); // Find routes with only one node
    }
    int len=dummy.size();
    sort();
    for(int i=0;i<len;i++) routes.erase(routes.begin()); // Delete routes with only one node
    total_routes-=len;
}

pair<int,int> best_pos(const vector<tuple<double,int,int>> &tmp)
{
    int ans=0,len=tmp.size();
    for(int i=1;i<len;i++)
    {
        if(get<0>(tmp[ans]) > get<0>(tmp[i])) ans=i; // tuple用get來取值, 找total distance最短的
    }
    return make_pair(get<1>(tmp[ans]),get<2>(tmp[ans])); // 回傳達到最短total distance要插入的位置
}

void Solution::test()
{
    cout<<AFV<<" "<<F1v<<" "<<F2v<<" "<<F3v<<"\n";
}

int Solution::get_total_nodes(){
    int ret = 0;
    for(auto &i : routes){
        ret += i.get_total_nodes();
    }
    return ret;
}
