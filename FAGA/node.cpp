#include "node.h"

Node sp;
bool method=false;

Node::Node(string addr,pair<double,double> p,int q,double e,double l,double a,double w,double s)
{
    address=addr;
    position=p; // pair
    demand=q;
    ready_time=e;
    due_time=l;
    arrival_time=a;
    waiting_time=w;
    service_time=s;
}

Node::Node()
{
    return;
}

vector<Node> read_node()
{
    method=false;
    vector<Node> result;
    ifstream file;
    file.open("../benchmark/RC1/RC107_100.txt");
    string line;
    if(!file.fail())
    {
        while(file>>line)
        {
            vector<string> v=split(line,';');
            Node tmp(v[0],make_pair(atof(v[1].data()),atof(v[2].data())),atoi(v[3].data()),atof(v[4].data()),atof(v[5].data()),0.0,0.0,atof(v[6].data()));
            result.emplace_back(tmp);
        }
    }
    file.close();
    sp=result[0];
    return result;
}

vector<Node> read_node3(const string& benchmark)
{
    method=false;
    vector<Node> result;
    ifstream file;
    file.open(benchmark);
    string line;
    if(!file.fail())
    {
        while(file>>line)
        {
            vector<string> v=split(line,';');
            Node tmp(v[0],make_pair(atof(v[1].data()),atof(v[2].data())),atoi(v[3].data()),atof(v[4].data()),atof(v[5].data()),0.0,0.0,atof(v[6].data()));
            result.emplace_back(tmp);
        }
    }
    file.close();
    sp=result[0];
    return result;
}

double Node::get_travel_distance(const Node na,const Node nb)
{
    if(method) // 經緯度
    {
        double a,b,radLat1=Radian(na.position.first),radLat2=Radian(nb.position.first);
        a=radLat1-radLat2,b=Radian(na.position.second)-Radian(nb.position.second);
        double s=2*asin(sqrt(pow(sin(a/2),2)+cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));
        return s*EARTH_RADIUS*allowance_rate;
    }
    // (x, y)座標
    double x=(na.position.first-nb.position.first),y=(na.position.second-nb.position.second);
    return sqrt(x*x+y*y);
}

double Node::get_travel_time(const Node a,const Node b)
{
    if(method) return get_travel_distance(a,b)/60; // 經緯度
    // (x, y)座標
    return get_travel_distance(a,b);
}

double Radian(double degree)
{
    return degree*Pi/180;
}

void Node::Node_setting(Node &a,Node &b) // 設定b的arrival_time 跟 waiting time
{
    b.arrival_time=Node::get_travel_time(a,b)+a.arrival_time+a.waiting_time+a.service_time;
    b.waiting_time=max(0.0,b.ready_time-b.arrival_time);
}

bool Node::operator==(Node b) //比較地址、經緯度、需求量......
{
    return address==b.address&&position==b.position&&demand==b.demand&&ready_time==b.ready_time&&due_time==b.due_time;
}

bool Node::operator!=(Node b)
{
    return !operator==(b);
}

bool Node::operator<(Node b)
{
    return Node::get_travel_distance(sp,*this)<Node::get_travel_distance(sp,b);
}

void Node::print_nodes(vector<Node> &v)
{
    for(auto &i:v) i.print();//cout<<i.address+"\n";
}

void Node::print()
{
    cout<<address;//<<": "<<rounding(ready_time)<<" "<<rounding(due_time)<<"\n";//demand<<" "<<rounding(arrival_time)<<" "<<rounding(waiting_time)<<" "<<rounding(service_time)<<"\n";
}

void Node::test()
{
    address="123";
}

string Node::get_address() {
    return address;
}
