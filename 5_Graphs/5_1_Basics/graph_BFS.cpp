#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1e5 + 5;

vector<int> adj[MAXN];
bool vis[MAXN];
int dist[MAXN]; // Guarda a distância da origem até o nó

void bfs(int start) {
    queue<int> q;
    
    q.push(start);
    vis[start] = true;
    dist[start] = 0;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (int v : adj[u]) {
            if (!vis[v]) {
                vis[v] = true;
                dist[v] = dist[u] + 1; // A distância é a do pai + 1
                q.push(v);
            }
        }
    }
}