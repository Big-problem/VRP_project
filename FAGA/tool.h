#include<bits/stdc++.h>
#define ll long long
#define BPF 53

using namespace std;

vector<string> split(const string,char);
double rounding(double);
double random();
bool decide(double);
ll bisect(double*,double,ll,ll);
ll rand_below(ll);

const double allowance_rate=1.2; //�e��v
const double RECIP_BPF=pow(2,-BPF);
const ll max_size=1<<BPF;
extern bool check;

template<typename T>
T choice(const vector<T> &v)  //�H���D��]�����^
{
    return v[rand_below(v.size())];
}

template<typename T>
T &rchoice(vector<T> &v)  //�H���D��]�Ѧҡ^
{
    return v[rand_below(v.size())];
}

template<typename T>
vector<T> choices(const vector<T> &v,double *weights,double *cum_weights,ll k) //�ھ��v���H���D��k�ӡA�S���v����T�N���v��
{
    vector<T> result;
    ll n=v.size();
    if(!cum_weights)
    {
        if(!weights)
        {
            for(ll i=0;i<k;i++) result.emplace_back(v[floor(random()*n)]);
            return result;
        }
        cum_weights=new double[n];
        cum_weights[0]=weights[0];
        for(ll i=1;i<n;i++) cum_weights[i]=weights[i]+cum_weights[i-1];
    }
    double total=cum_weights[n-1];
    for(ll i=0;i<k;i++) result.emplace_back(v[bisect(cum_weights,random()*total,0,n-1)]);
    return result;
}

template<typename T>
void Remove(vector<T> &v,T a)
{
    auto p=find(v.begin(),v.end(),a);
    v.erase(p);
}
