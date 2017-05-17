#include<bits/stdc++.h>

using namespace std;

typedef vector<int> vi; 
typedef vector<vi> vvi;
typedef pair<int,int> ii;

int INF = numeric_limits<int>::max(); int NIL = -1; //special vertex

class FoxAndGo3
{
public:    
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
        
    //Max Bipartite Matching = Min Vertex Cover
	int maxEmptyCells(vector <string> board)
	{       
        int nl = 0, nr = 0, m; //number of vertices on left (empty squares), number of vertices on right (white pieces), number of edges

        vector<pair<int,ii> > edgelist;
        map<ii, int> rvertexmap;

        //parse input into graph
        int N = board.size();

        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                if(board[i][j] == '.'){
                    //look at neighbours
                    int a = i, b = j+1;
                    if(a >= 0 && a < N && b >= 0 && b < N && board[a][b] == 'o'){
                        edgelist.push_back(make_pair(nl, make_pair(a,b)));                       
                    }
                    a = i+1, b = j; 
                    if(a >= 0 && a < N && b >= 0 && b < N && board[a][b] == 'o'){
                        edgelist.push_back(make_pair(nl, make_pair(a,b)));
                    }
                    a = i-1, b = j;
                    if(a >= 0 && a < N && b >= 0 && b < N && board[a][b] == 'o'){
                        edgelist.push_back(make_pair(nl, make_pair(a,b)));
                    }
                    a = i, b = j-1;
                    if(a >= 0 && a < N && b >= 0 && b < N && board[a][b] == 'o'){
                        edgelist.push_back(make_pair(nl, make_pair(a,b)));
                    }  
                    nl += 1;                  
                }
                else if(board[i][j] == 'o'){
                    rvertexmap[make_pair(i,j)] = nr;
                    nr += 1;                    
                }
                //else do nothing
            }
        }
        
        vvi adjList(nl+nr,vi(0)); vi dist(nl+1); vi L(0);
        m = edgelist.size();

        for(int i = 0; i < m; ++i){
            int u,v;
            u = edgelist[i].first;
            v = rvertexmap[edgelist[i].second];
            L.push_back(u);

            adjList[u].push_back(v+nl);
        }

        //initialization
        vi match(nl+nr, NIL); 
        
        int matching = 0;

        //Hopcroft-Karp Algorithm
        while(BFS(adjList, L, match, dist)){
            for(vi::iterator it = L.begin(); it < L.end(); ++it){
                int i = *it;
                if(match[i] == NIL && DFS(adjList, match, dist, i)){
                    matching++;
                }
            }
        }

        //matching contains the slots that have been filled
		return nl + nr - matching;
	}
};

