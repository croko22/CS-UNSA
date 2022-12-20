#include<bits/stdc++.h>
using namespace std;
 
class Graph
{
    int V;
    vector<list<int>> adj;  
public:
    Graph(int V);  
    void addEdge(int v, int w);
    vector<int> BFS(int s); 
};
 
Graph::Graph(int V)
{
    this->V = V;
    adj.resize(V);
}
 
void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w); 
}
 
vector<int> Graph::BFS(int s)
{
    vector<int> ans;
    vector<bool> visited;
    visited.resize(V,false);
 
    list<int> queue;
 
    visited[s] = true;
    queue.push_back(s);
 
    while(!queue.empty())
    {
        s = queue.front();
        cout << s << " ";
        ans.push_back(s);
        queue.pop_front();
        for (auto adjecent: adj[s])
        {
            if (!visited[adjecent])
            {
                visited[adjecent] = true;
                queue.push_back(adjecent);
            }
        }
    }
    return ans;
}

bool isomorphic(vector<int> &v1, vector<int> &v2){
    if(v1.size() != v2.size()) return false;
    unordered_map<int,int> m;
    for(int i = 0; i < v1.size(); i++){
        if(m.find(v1[i]) == m.end()){
            m[v1[i]] = v2[i];
        }else{
            if(m[v1[i]] != v2[i]) return false;
        }
    }
    return true;
}

bool subgraphIso(vector<int> &v1, vector<int> &v2, vector<vector<int>> &adj1, vector<vector<int>> &adj2){
    if(v1.size() != v2.size()) return false;
    unordered_map<int,int> m;
    for(int i = 0; i < v1.size(); i++){
        if(m.find(v1[i]) == m.end()){
            m[v1[i]] = v2[i];
        }else{
            if(m[v1[i]] != v2[i]) return false;
        }
    }
    for(int i = 0; i < adj1.size(); i++){
        for(int j = 0; j < adj1[i].size(); j++){
            if(m.find(adj1[i][j]) == m.end()) return false;
            if(!isomorphic(adj2[i],adj2[m[adj1[i][j]]])) return false;
        }
    }
    return true;
}
 
int main(){
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(2, 0);
    g.addEdge(2, 3);
    g.addEdge(3, 3);

    Graph g2(2);
    g2.addEdge(0, 1);
    g2.addEdge(0, 2);
    
    vector<int> v1 = g.BFS(2);
    vector<int> v2 = g2.BFS(0);
    cout << endl;
    cout << "G2 esta incluido en G: " << isomorphic(v1,v2) << endl;
 
    return 0;
}