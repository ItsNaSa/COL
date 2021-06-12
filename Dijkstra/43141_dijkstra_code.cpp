#include<iostream>
#include<vector>
#include<climits>     // Used for INT_MAX

using namespace std;

#define vertex 7      // It is the total no of verteices in the graph
// A method to find the vertex with minimum distance which is not yet included in visited
int minimumDist(vector<int> dist, vector<bool> visited) {
	int min=INT_MAX,index;                 // initialize min with the maximum possible value as infinity does not exist 
	for(int v=0;v<dist.size();v++){
		if(visited[v]==false && dist[v]<=min) {
			min=dist[v];
			index=v;
		}
	}
	return index;
}
// Method to implement shortest path algorithm
void dijkstra(vector<vector<int>> g,int src) {
	int m = g.size();
	int graph[m][m];
	for(int i=0;i<m;i++){
		for(int j=0;j<m;j++){
			graph[i][j] = g[i][j];
		}
	}
	vector<int> dist;
	vector<bool> visited;
	// Initialize distance of all the vertex to INFINITY 
	// Set the visited as false
	for(int i=0;i<m;i++) {
		dist.push_back(INT_MAX);
		visited.push_back(false);	
	}

	dist[src]=0;                                   // Initialize the distance of the source vertec to zero
	for(int c=0;c<m;c++)                           
	{
		int u=minimumDist(dist,visited);              // u is any vertex that is not yet included in visited and has minimum distance
		visited[u]=true;                              // If the vertex with minimum distance found include it to visited
		for(int v=0;v<m;v++)                  
		// Update dist[v] if not in visited and their is a path from src to v through u that has distance minimum than current value of dist[v]
		{
			if(!visited[v] && graph[u][v] && dist[u]!=INT_MAX && dist[u]+graph[u][v]<dist[v]){
				dist[v]=dist[u]+graph[u][v];
			}
		}
	}
	cout<<"Vertex\t\tDistance from source"<<endl;
	for(int i=0;i<m;i++)                       // will print the vertex with their distance from the source to the console 
	{
		char c=65+i;
		cout<<c<<"\t\t"<<dist[i]<<endl;
	}
}
int main()
{
	int m;
	cout<<"Enter number of nodes in graph : ";
	cin>>m;
	vector<vector<int>> graph( m , vector<int> (m, 0));
	for(int i=0;i<m;i++){
		for(int j=0;j<m;j++){
				int x;
				cout<<"\tEnter ["<<i<<"]["<<j<<"] : ";
				cin>>x;
				graph[i][j]=x;
		}
		cout<<"\n";
	}
	dijkstra(graph,0);
	return 0;	                        
}

