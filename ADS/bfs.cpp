#include<iostream>
#include<vector>
#include<queue>
using namespace std;

class Graph{
	int V;
	int t = 0;
	vector<int> path;
	vector< vector<int> > adj;
	
	void BFS_util(int s, vector<bool>& visited){			
		queue<int> q;
		visited[s]=true;
		q.push(s);
		while(!q.empty()){
			int curr = q.front();
			q.pop();
			path.push_back(curr);
			
			for(int i:adj[curr]){
				++t;
				if(!visited[i]){
					visited[i]=true;
					q.push(i);
				}				
			}
		}
	}
	
	public:
	Graph(int v){
		V=v;
		adj.resize(v);
		//path.resize(v);
	}
	
	void addEdge(int v, int u){
		adj[v].push_back(u);
		adj[u].push_back(v);
	}
	
	void BFS(int s){
		vector <bool> visited(V,false);
		BFS_util(s,visited);
	}
	
	void printPath(){
		for(int i:path){			
			cout<<i<<" ";
		}
	}
	
	int T(){return t;}
	
};

int main(){
	cout<<"No of Vertices: ";
	int v;cin>>v;

	Graph g(v);
	cout<<"No of Edges: ";
	
	int e;cin>>e;
	cout<<"Ënter "<<e<<" edges: "<<endl;
	for(int i=0;i<e;++i){
		int v,u;cin>>v>>u;

		g.addEdge(v,u);
	}	
	
	cout<<"Enter source vertex: ";
	int s;cin>>s;
	g.BFS(s);
	cout<<"Path: ";
	g.printPath();
	cout<<endl;
	cout<<endl<<"Total time complexity O(V+E)= "<<g.T();
	cout<<endl;
	cout<<endl<<"Auxiliary space complexity O(V)= "<<v;
	cout<<endl;
	
	return 0;
}