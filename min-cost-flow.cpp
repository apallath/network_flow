#include<bits/stdc++.h>

using namespace std;

//problem limits
#define MAX_V 101
#define MAX_E 20001

//type definitions
typedef long long l;
typedef pair<l,l> ll;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<l> vl;
typedef vector<vl> vvl;

const l INF = numeric_limits<l>::max() >> 1;

//global variables
int n, m;
int s, t;
vvi adj; //adjacency list
l cap[MAX_V][MAX_V]; //edge capacities
l cost[MAX_V][MAX_V]; //edge costs
l flow[MAX_V][MAX_V]; //flow on edges
l d[MAX_V]; //shortest path distance to each edge
l pi[MAX_V]; //potential function
int parent[MAX_V]; //for dijkstra
l bottle[MAX_V]; //bottleneck on shortest path till a particular node

//init, before each iteration
void init(){
    adj.assign(n,vi(0));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cap[i][j] = 0;
            cost[i][j] = INF;
            flow[i][j] = 0;
        }
        pi[i] = 0;
    }   
}

//calculate initial node potentials using bellman-ford algorithm
int bellman_ford(){ //correct
    for(int i = 0; i < n; i++){
        pi[i] = INF;
    }
    pi[0] = 0; //source potential
    for(int i = 0; i < n-1; i++){
        for(int u = 0; u < n; u++){
            if(pi[u] < INF){
                for(int j = 0; j < adj[u].size(); j++){
                    int v = adj[u][j];

                    if(pi[u] + cost[u][v] < pi[v]){
                        pi[v] = pi[u] + cost[u][v];
                    }    
                }   
            }
        }        
    }
    return 0;
}


bool dijkstra(){
    //re-init
    for(int i = 0; i < n; i++){
        parent[i] = -1;
        bottle[i] = INF;
        d[i] = INF;
    }

    set<pair<l, int> > q;

    d[s] = 0;    
    q.insert(make_pair(d[s], s));
    
    while(!q.empty()){
        int u = q.begin()->second; //extract min distance node  
        q.erase(q.begin()); 

        for(int i = 0; i < adj[u].size(); i++){
            int v = adj[u][i];
     
            l rcost = cost[u][v] + pi[u] - pi[v]; //reduced cost

            if(cap[u][v] - flow[u][v] > 0 && d[u] + rcost < d[v]){ 
                d[v] = d[u] + rcost; //update distance
                parent[v] = u;                
                bottle[v] = min(cap[u][v] - flow[u][v], bottle[u]); //update bottleneck
                q.insert(make_pair(d[v], v)); //insert into queue                               
            }    
        }        
    }
    return d[t] != INF; //is the sink reachable?
}

ll mcmf(){
    l val_flow = 0, val_cost = 0;

    bellman_ford(); //calculate initial node potentials

    while(dijkstra()){
        //augment flow  
        l bneck = bottle[t]; //bottleneck of s->t path
        int u, v = t;
        while(v != s){
            u = parent[v];
            flow[u][v] += bneck;
            flow[v][u] -= bneck;
            cost[u][v] = -cost[u][v];
            cost[v][u] = -cost[v][u];
            v = u;
        }        
        val_flow += bneck;
        val_cost += bneck * (d[t] - pi[s] + t[pi]);
    }

    //update potentials - set pi = pi + d
    for(int i = 0; i < n; i++){
        if(d[i] != INF){
            pi[i] = pi[i] + d[i];
        }
        else{
            pi[i] = INF;
        }
    }

    return make_pair(val_flow, val_cost);
}

//Testing - UVa 10594 Data Flow

int main(){
    while(scanf("%d %d", &n, &m) == 2){
        n++;
        s = 0; t = n-1;
        init();

        for(int i = 0; i < m; i++){
            int from, to; l cst;
            //input from, to, edge cost
            scanf("%d %d %Ld", &from, &to, &cst);          
            //bidirectional edges  
            adj[from].push_back(to); adj[to].push_back(from);
            cost[from][to] = cst; cost[to][from] = cst;
        }
        l D,K; //D = flow desired, K = edge capacity
        scanf("%Ld %Ld", &D, &K);

        for(int u = 0; u < n; u++){
            for(int j = 0; j < adj[u].size(); j++){
                int v = adj[u][j];
                //bidirectional edges
                cap[u][v] = K; cap[v][u] = K;
            }
        }

        adj[0].push_back(1); adj[1].push_back(0);
        cost[0][1] = 0; cost[1][0] = 0;
        cap[0][1] = D; cap[0][1] = D;

        //compute max flow, min cost
        ll op = mcmf();
      
        if(op.first == D){
            cout << op.second << endl;
        }
        else{
            cout << "Impossible." << endl;
        }
    }
}
