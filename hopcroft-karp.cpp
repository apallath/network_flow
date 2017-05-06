//References - CP3 (Halim, S. and Halim, F.)
#include<bits/stdc++.h>
using namespace std;

typedef vector<int> vi; typedef vector<vi> vvi;
int INF = numeric_limits<int>::max(); int NIL = -1; //special vertex

bool BFS(vvi &adjList, vi &L, vi &match, vi &dist){
    queue<int> q;
    for(int i = 0; i < L.size(); i++){
        int u = L[i];
        if(match[u] == NIL){
            dist[u+1] = 0;
            q.push(u);
        }else{
            dist[u+1] = INF;
        }
    }
    dist[NIL+1] = INF;
    while(!q.empty()){
        int u = q.front(); q.pop();
        if(u!=NIL){
            for(int j = 0; j < adjList[u].size(); j++){
                int v = adjList[u][j];
                if(dist[match[v]+1] == INF){
                    dist[match[v]+1] = dist[u+1] + 1;
                    q.push(match[v]);
                }
            }
        }
    }
    return (dist[NIL+1]!=INF);
}
bool DFS(vvi &adjList, vi &match, vi &dist, int u){
    if(u!=NIL){
        for(vi::iterator it = adjList[u].begin(); it < adjList[u].end(); ++it){
            int v = *it;
            if(dist[match[v]+1] == dist[u+1]+1){
                if(DFS(adjList, match, dist, match[v])){
                    match[v] = u;
                    match[u] = v;
                    return true;
                }
            }
        }
        dist[u+1] = INF;
        return false;
    }
    return true;
}
int main(){
    //read input - SPOJ MATCHING
    int nl, nr, m;
    cin >> nl >> nr >> m; //number of vertices on left, number of vertices on right, number of edges
    vvi adjList(nl+nr,vi(0)); vi dist(nl+1); vi L(0);
    for(int i = 0; i < m; ++i){
        int u,v;
        cin >> u >> v; //vertices are numbered from 1 on BOTH left and right
        u--, v--; //indexing
        L.push_back(u);
        adjList[u].push_back(v+nl);
    }
    //initialization
    vi match(nl+nr, NIL); int matching = 0;
    //HK
    while(BFS(adjList, L, match, dist)){
        for(vi::iterator it = L.begin(); it < L.end(); ++it){
            int i = *it;
            if(match[i] == NIL && DFS(adjList, match, dist, i)){
                matching++;
            }
        }
    }
    cout << matching << endl;
    return 0;
}
