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

void FAGA::Crossover(vector<Solution> &children) //�q�¸Ѥ��HPc���v���D�X2�ӸѡA�b�q�o2�ӸѪ����u���X���ӧO�D�X1�����u�A�h���������`�I��ͦ��s�l�N
{
    vector<Solution> cross_pair=choices(sset.sol,sset.crossover_probability,nullptr,2); // ���D���

    Route r0=choice(cross_pair[0].routes),r1=choice(cross_pair[1].routes); // ��Ѥ��U�D�@���u

    r0.nodes.erase(r0.nodes.begin()),r1.nodes.erase(r1.nodes.begin()); // �h���_�I
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
    std::sort(buffer.begin(),buffer.end(),Solution::better); //�Ƨǧ�XRB�̤p����
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

void FAGA::BCRC(const vector<Node> &nodes,Solution cps,int ri,int ni,int pi,int inserted) //�ͦ��s��(Old version)
{
    if(fail_count>=1000) return;
    if(inserted==nodes.size()) //�Ҧ��I�����J
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
    if(ri>=cps.routes.size()||ni>=nodes.size()||pi>cps.routes[ri].nodes.size()) return; //index�W�X�d��
    if(cps.routes[ri].add_node(pi,nodes[ni],capacity_limit,tmax)) //�p�G���\�N�I���J
    {
        BCRC(nodes,cps,ri,ni+1,1,inserted+1);
        cps.routes[ri].remove_node(nodes[ni]); //�٭쪬�A
    }
    else fail_count++;
    BCRC(nodes,cps,ri,ni,pi+1,inserted); //�󴫴��J��m
    //if(ri+1<cps.routes.size()&&nodes[ni].demand+cps.routes[ri+1].capacity<=capacity_limit)
    BCRC(nodes,cps,ri+1,ni,1,inserted); //�󴫴��J���u
}

void FAGA::BCRC(vector<Node> nodes,Solution &cps) // (�n�ɦ^��node, �@�ո�)
{
    while(!nodes.empty())
    {
        Node &n=rchoice(nodes); //�H��������J�`�I
        vector<tuple<double,int,int>> tmp;
        for(int i=0;i<cps.total_routes;i++) //�C�Ӹ��u
        {
            int jl=cps.routes[i].nodes.size();
            for(int j=1;j<=jl;j++) //�C�Ӵ��J�I
            {
                if(cps.routes[i].add_node(j,n,capacity_limit,tmax))
                {
                    cps.attribute_calculator(); //�p��total_dist_travelled
                    tmp.emplace_back(cps.total_dist_travelled,i,j); //����total_dist_travelled�B�������u�B���Ӧ�m
                    cps.routes[i].remove_node(n); //�_��
                }
            }
        }
        if(tmp.empty()) //�S��m�i��->�}�s���u
        {
            cps.add_new_route(node_list[0],capacity_limit,tmax);
            cps.routes[cps.total_routes-1].add_node(1,n,capacity_limit,tmax);
        }
        else
        {
            pair<int,int> pii=best_pos(tmp); //���total_dist_travelled�̤p����m
            cps.routes[pii.first].add_node(pii.second,n,capacity_limit,tmax); //���J
        }
        Remove(nodes,n);
    }
    cps.attribute_calculator();
    cps.route_clear(); //�����u�Ѱ_�I�����u
}

void FAGA::cross_delete(const vector<Node> &v,vector<Route> &r) // �i�H�ΪŶ����ɶ�
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
        vector<Route> v=choices(s.routes,nullptr,nullptr,2); // �D������u
        if(v[0]==v[1]) return false;

        vector<Node> c0=v[0].nodes,c1=v[1].nodes;
        c0.erase(c0.begin()),c1.erase(c1.begin()); //�R���_�I
        if(!c0.size()||!c1.size()) return false;

        Node n0=choice(c0),n1=choice(c1);
        int p0=find(v[0].nodes.begin(),v[0].nodes.end(),n0)-v[0].nodes.begin(), p1=find(v[1].nodes.begin(),v[1].nodes.end(),n1)-v[1].nodes.begin();
        if(!change(v[0],v[1],p0,p1)) return false; //�ˬd���I��m�O�_�i�H����
        auto p2=find(s.routes.begin(),s.routes.end(),v[0]), p3=find(s.routes.begin(),s.routes.end(),v[1]);
        p2->remove_node(n0),p3->remove_node(n1);
        p2->add_node(p0,n1,capacity_limit,tmax);
        p3->add_node(p1,n0,capacity_limit,tmax);
    }
    return true;
}

bool FAGA::change(Route r1,Route r2,int i1,int i2) //�ˬd���I�O�_�i�H����
{
    Node n1=*(r1.nodes.begin()+i1),n2=*(r2.nodes.begin()+i2);
    r1.remove_node(n1),r2.remove_node(n2);
    check=true;
    if(!r1.add_node(i1,n2,capacity_limit,tmax) || !r2.add_node(i2,n1,capacity_limit,tmax)) return false;
    return true;
}

void FAGA::solution_replace(const vector<Solution> &children) //�N�s�l�N�[�J��e�Ѩåh�������¸�
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
