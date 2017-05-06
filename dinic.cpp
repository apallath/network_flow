//References - MAXimal (e-maxx.ru/algo/dinic), slides by Chih-Hsuan Kuo (https://www.slideshare.net/KuoE0/acmicpc-dinics-algorithm)
#include<bits/stdc++.h>
using namespace std;

long INF = numeric_limits<long>::max();;
typedef vector<long> vi; typedef vector<vi> vvi;
typedef pair<long,long> ii; typedef map<ii,long> miii;

//assign levels
long BFS(vvi &res_graph, miii &res_cap, vi &level, long s, long t){
    fill(level.begin(), level.end(), 0);
    level[s] = 1;   
    queue<long> q; q.push(s);   

    while(!q.empty()){
        long u = q.front(); q.pop();
        for(long it = 0; it<res_graph[u].size(); ++it){
            long v = res_graph[u][it];

            if(res_cap[make_pair(u,v)]  > 0 && level[v] == 0){ //avlbl, unvisited
                level[v] = level[u] + 1; 
                q.push(v); 
            }
        }
    }
    return level[t] != 0;
}

//sending flow along a path
long DFS(vvi &res_graph, miii &res_cap, vi &level, long s, long t, long u, long bottleneck){
    if(bottleneck == 0){ return 0; }
    if(u==t){ return bottleneck; }

    for(long it = 0; it<res_graph[u].size(); ++it){
        long v = res_graph[u][it];
        if(level[v] == level[u] + 1 && res_cap[make_pair(u,v)] > 0){
            long cur_bottle = min(bottleneck, res_cap[make_pair(u,v)]);
            long push_val = DFS(res_graph, res_cap, level, s, t, v, cur_bottle);

            if(push_val != 0){ //update residual capacities
                res_cap[make_pair(u,v)] -= push_val;
                res_cap[make_pair(v,u)] += push_val;
                return push_val;                
            }
        }
    }
    return 0;
}

long Dinic(vvi &res_graph, long n, miii &res_cap, long s, long t){
    long flow = 0;
    vi level(n,0);
    while(BFS(res_graph, res_cap, level, s, t)){
        while(long cur_flow = DFS(res_graph, res_cap, level, s, t, s, INF)){
            flow += cur_flow;
        }
    }     
    return flow;
}

int main(){ //usage demo - undirected flow network
    long n,m,s=0,t;
    cin >> n >> m;
    t = n-1;
    vvi res_graph(n,vi(0));

    miii res_cap;

    for(long i = 0; i < m; ++i){
        long u, v, cap;
        cin >> u >> v >> cap;
        u--; v--;
        if(u!=v){
            res_graph[u].push_back(v); 
            res_graph[v].push_back(u);

            if(res_cap.find(make_pair(u,v)) == res_cap.end()){ //(u,v) or (v,u) not assigned
                res_cap[make_pair(u,v)] = cap; 
                res_cap[make_pair(v,u)] = cap;
            }
            else{
                res_cap[make_pair(u,v)] += cap;
                res_cap[make_pair(v,u)] += cap;
            }
        }
    }
    cout << Dinic(res_graph, n, res_cap, s, t) << endl;
    return 0;
}
