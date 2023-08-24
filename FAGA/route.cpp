#include "route.h"

Route::Route()
{
    capacity=0;
    total_nodes=0;
    total_travel_time=0.0;
    nodes.clear();
}

double Route::total_distance()
{
    double result=Node::get_travel_distance(nodes[0],*(nodes.end()-1));
    for(int i=0;i<total_nodes-1;i++) result+=Node::get_travel_distance(nodes[i],nodes[i+1]);
    return result;
}

bool Route::add_node(int index,Node node_to_add,int c,double t) //插入位置、插入節點、負重上限、時間限制、回傳結果
{
    if(nodes.empty()) //起點特判
    {
        nodes.emplace_back(node_to_add);
        total_nodes++;
        return true;
    }
    index=index>total_nodes?total_nodes:index;
    Node::Node_setting(nodes[index-1],node_to_add);
    if(legal(index,node_to_add,c,t)) //判斷是否合法
    {
        nodes.insert(nodes.begin()+index,node_to_add);
        total_nodes++;
        capacity+=node_to_add.demand;
        update(index);
        total_travel_time=nodes[total_nodes-1].arrival_time+nodes[total_nodes-1].waiting_time+nodes[total_nodes-1].service_time+Node::get_travel_time(nodes[0],nodes[total_nodes-1]);
        return true;
    }
    return false;
}

bool Route::legal(int index,Node node_to_add,int c,double t) //複製路線資訊後檢查是否違反限制式
{
    vector<Node> tmp=nodes;
    tmp.insert(tmp.begin()+index,node_to_add);
    int len=tmp.size();
    for(int i=0;i<len;i++)
    {
        if(i>=index-1&&i<=len-2) Node::Node_setting(tmp[i],tmp[i+1]);
        if(tmp[i].arrival_time>tmp[i].due_time)
        {
            return false;
        }
    }
    if(capacity+node_to_add.demand>c||tmp[len-1].arrival_time+tmp[len-1].waiting_time+tmp[len-1].service_time>t-Node::get_travel_time(tmp[0],tmp[len-1]))
    {
        //if(check) cout<<capacity<<" "<<node_to_add.demand<<"\n";
        return false;
    }
    return true;
}

void Route::update(int index) //更新節點時間資訊
{
    for(auto i=nodes.begin();i!=nodes.end()-1;i++) Node::Node_setting(*i,*(i+1));
}

void Route::remove_node(Node node_to_remove)
{
    auto p=find(nodes.begin(),nodes.end(),node_to_remove);
    if(p==nodes.end()||p==nodes.begin()) return; //找不到或刪除起點
    nodes.erase(p);
    capacity-=node_to_remove.demand;
    total_nodes--;
    update(p-nodes.begin());
    total_travel_time=nodes[total_nodes-1].arrival_time+nodes[total_nodes-1].waiting_time+nodes[total_nodes-1].service_time+Node::get_travel_time(nodes[0],nodes[total_nodes-1]);
}

void Route::print()
{
    /*for(int i=0;i<total_nodes;i++)
    {
        if(i) cout<<"->";
        nodes[i].print();
    }
    cout<<"\n";*/
    //for(int i=0;i<total_nodes-1;i++) cout<<Node::get_travel_distance(nodes[i],nodes[i+1])<<"\n";
    cout<<"\ntotal nodes= "<<total_nodes<<", total travel time= "<<total_travel_time<<", capacity= "<<capacity<<"\n";
}

void Route::remove() //測試用
{
    remove_node(nodes[2]);
    remove_node(nodes[3]);
}

bool Route::operator<(Route b) //排序用
{
    return capacity<b.capacity;
}

bool Route::operator==(Route b)
{
    if(total_nodes!=b.total_nodes) return false;
    for(int i=0;i<total_nodes;i++)
    {
        if(nodes[i]!=b.nodes[i])
        {
            return false;
        }
    }
    return capacity==b.capacity&&total_nodes==b.total_nodes&&total_travel_time==b.total_travel_time;
}
