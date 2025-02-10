#include<iostream>
#include<vector>

using namespace std;

class Graph{
	int V;
	int t = 0;
	vector<int> path;
	vector< vector<int> > adj;
	
	void DFS_util(int s, vector<bool>& visited){			
		visited[s]=true;		
		//t++;
		path.push_back(s);
		for(int i: adj[s]){			
			++t;
			if(!visited[i])
				DFS_util(i,visited);
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
	
	void DFS(int s){
		vector <bool> visited(V,false);
		DFS_util(s,visited);
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
	g.DFS(s);
	cout<<"Path: ";
	g.printPath();
	cout<<endl;
	cout<<endl<<"Total time complexity O(V+E)= "<<g.T();
	cout<<endl;
	cout<<endl<<"Auxiliary space complexity O(V+E)= "<<v+e;
	cout<<endl;
	
	return 0;
=======
#include<iostream>
#include<vector>

using namespace std;

class Graph{
	int V;
	int t = 0;
	vector<int> path;
	vector< vector<int> > adj;
	
	void DFS_util(int s, vector<bool>& visited){			
		visited[s]=true;		
		//t++;
		path.push_back(s);
		for(int i: adj[s]){			
			++t;
			if(!visited[i])
				DFS_util(i,visited);
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
	
	void DFS(int s){
		vector <bool> visited(V,false);
		DFS_util(s,visited);
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
	g.DFS(s);
	cout<<"Path: ";
	g.printPath();
	cout<<endl;
	cout<<endl<<"Total time complexity O(V+E)= "<<g.T();
	cout<<endl;
	cout<<endl<<"Auxiliary space complexity O(V+E)= "<<v+e;
	cout<<endl;
	
	return 0;
}