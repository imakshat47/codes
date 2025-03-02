#include<iostream>
#include<vector>
int MAX=1e5;
using namespace std;

int minDistance(vector<int> &dist, vector<bool> &visited,int V)
{

    // Initialize min value
    int min = MAX, min_index;

    for (int v = 0; v < V; v++)
        if (visited[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}


void printSPT(vector<int> &dist,int V)
{
    cout << "Vertex \t Distance from Source" << endl;
    for (int i = 0; i < V; i++)
        cout << i << " \t" << dist[i] << endl;
}


void Dijkstras(vector< vector<int>> &adjMtrx, int s,int v){
	vector<int> dist(v,MAX);
	vector<bool> visited(v,false);

	dist[s]=0;

	for(int i=0;i<v-1;++i){
		int u = minDistance(dist, visited, v);
		visited[u] = true;
		for(int i=0;i<v;++i){
			if(!visited[i] && adjMtrx[u][i] && 
			dist[i] != MAX &&
			(dist[u] + adjMtrx[u][i]) < dist[i])
				dist[i] = (dist[u] + adjMtrx[u][i]);
		}
	}

	printSPT(dist,v);
}

int main(){
	cout<<"No of Vertices: ";
	int v;cin>>v;

	vector< vector<int>> adjMtrx (v,vector<int> (v,0));

	cout<<"No of Edges: ";	
	int e;cin>>e;

	cout<<"Ënter "<<e<<" edges: "<<endl;	
	for(int i=0;i<e;++i){
		int v,u,wt;cin>>v>>u>>wt;
		adjMtrx[v][u] = wt;
		adjMtrx[u][v] = wt;
	}	
	
	cout<<"Enter source vertex: ";
	int s;cin>>s;
	
	Dijkstras(adjMtrx,s,v);
	
	return 0;
}