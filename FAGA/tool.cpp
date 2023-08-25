#include "tool.h"

bool check=false;

vector<string> split(const string s,char spliter) // "1;2;3;" -> {"1","2","3"}
{
    vector<string> tokens;
    stringstream ss(s);
    string token;
    while(getline(ss,token,spliter)) tokens.emplace_back(token);
    return tokens;
}

double rounding(double d)
{
    return (int)(d*100+0.5)/100.0;
}

double random()
{
    static random_device rd;
    static mt19937 gen(rd()+ chrono::system_clock::now().time_since_epoch().count());
    uniform_real_distribution<double> dis(0.0,1.0);
    return dis(gen);
}

bool decide(double p)
{
    return random()<p;
}

ll rand_below(ll n)
{
    return floor(random()*n);


    cout <<"Max_size: "<< max_size << "\n";
    if(n>=max_size) return floor(random()*n);
    if(!n) return 0;
    ll rem=max_size%n;
    double limit=(max_size-rem)/max_size;
    cout << n << " " << rem << " " << max_size << " " << limit << "\n";
    double r=random();
    while(r>=limit){
        cout << r << "\n";
        r=random();
    }
    return (ll)floor(r*max_size)%n;
}

ll bisect(double *a,double x,ll lo,ll hi)
{
    ll mid;
    while(lo<hi)
    {
        mid=(lo+hi)/2;
        if(x<a[mid]) hi=mid;
        else lo=mid+1;
    }
    return lo;
}
