#include <bits/stdc++.h>
using namespace std;

#define MAXN 2005

const int c = 1000000007; //10^9 + 7 seems to be a popular prime :)

typedef vector<int> vi; typedef vector<vi> vvi;

class CandyCupRunningCompetition
{
public:
    vvi adjList;
    int n;

    //modular exponentiation - return a * b mod c
    int mod_exp(int x, int y){
        int ans = 1;

        while(y > 0){
            if(y % 2 == 1){
                ans = (ans * x) % c;
            }
            y/=2;
            x = x*x % c;            
        }     
        return ans;
    }

    void dfs(int u, vi &visited){
        for(int i = 0; i < adjList[u].size(); i++){
            int v = adjList[u][i];
            if(visited[v] == 0){
                visited[v] = 1;
                dfs(v, visited);
            }
        }    
    }

    bool connected(){
        //DFS for connectivity
        vi visited(n, 0);
        dfs(0, visited);
        if(visited[n-1] == 0){
            return false;
        }    
        return true;
                       
    }
    
	int findMaximum(int N, vector <int> A, vector <int> B)
	{   
        /*  Idea - calculate minimum cut instead of maximum flow.
            Add edges one by one in decreasing order of i.
            If addition of edge i connects the source to the sink, that edge must be removed.
            If not, that edge must be kept - removal of that edge is more expensive than the removal of all the preceding edges
            Using this greedy strategy, we calculate the minimum cut, the value of which is equal to the maximum flow
            At each stage, we append 3 ^ i mod c to the cut value (mod c). To do this, we use modular exponentiation.            
        */ 
        n = N;
        adjList.assign(N,vi(0));
    
        int cut = 0;
        
        for(int i = A.size() - 1; i >= 0; i--){
            adjList[A[i]].push_back(B[i]);
            adjList[B[i]].push_back(A[i]);
            //check connectivity
            if(connected()){
                //no choice but to add edge i to the cut set i.e. delete it from the graph
                adjList[A[i]].pop_back();
                adjList[B[i]].pop_back(); 
                //add value to value of cut
                cut = (cut + mod_exp(3,i)) % c;
            }  
            //else edge i remains
        }
		return cut; //max flow = min cut
	}
};

