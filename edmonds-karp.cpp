//References - CP3 (Halim, S. and Halim, F.)
#include<bits/stdc++.h>
using namespace std;

int INF = numeric_limits<int>::max();;
typedef vector<int> vi; typedef vector<vi> vvi;
typedef pair<int,int> ii; typedef map<ii,int> miii;

int augment(miii &edge_caps, vi &parent, int s, int v, int bottleneck){
    int u = parent[v]; int f = 0;
    if(v == s){f = bottleneck;}
    else if(u != -1){
        int new_bottleneck = min(bottleneck, edge_caps[make_pair(u,v)]);
        f = augment(edge_caps, parent, s, u, new_bottleneck);
        edge_caps[make_pair(u,v)] -= f; edge_caps[make_pair(v,u)] += f;
    }
    return f;
}

int edmondsKarp(vvi &res_graph, miii &edge_caps, int n, int s, int t){
    int max_flow = 0;
    while(true){
        vi distance(n, INF); distance[s] = 0;
        vi parent(n, -1);
        queue<int> q; q.push(s); //bfs
        while(!q.empty()){
            int u = q.front(); q.pop();
            if (u == t) break; //reached sink,stop BFS
            for(int it = 0; it<res_graph[u].size(); ++it){
                int v = res_graph[u][it];
                if(edge_caps[make_pair(u,v)] > 0 && distance[v] == INF){ //avlbl, unvisited
                    distance[v] = distance[u] + 1; q.push(v); parent[v] = u;
                }
            }
        }
        int push_flow = augment(edge_caps, parent, s, t, INF);
        if(push_flow == 0) break; //no s->t path
        max_flow += push_flow;
    }
    return max_flow;
}

int main(){ //usage demo - undirected flow network
    int n,m,s,t;
    cin >> n >> m >> s >> t;
    vvi res_graph(n,vi(0));

    miii edge_caps;

    for(int i = 0; i < m; ++i){
        int u, v, cap;
        cin >> u >> v >> cap;
        if(u!=v){
            res_graph[u].push_back(v); 
            res_graph[v].push_back(u);
            if(edge_caps.find(make_pair(u,v)) == edge_caps.end()){ //(u,v) or (v,u) not assigned
                edge_caps[make_pair(u,v)] = cap; 
                edge_caps[make_pair(v,u)] = cap; //init
            }
            else{
                edge_caps[make_pair(u,v)] += cap;
                edge_cape[make_pair(v,u)] += cap;
            }
        }
    }
    cout << edmondsKarp(res_graph, edge_caps, n, s, t);
    return 0;
}
