#include "ford_fulkerson.h"

using namespace std;

bool bfs(int rGraphe[V][V], int s, int t, int parent[])
{
    bool visite[V];
    memset(visite, 0, sizeof(visite));

    queue <int> q;
    q.push(s);
    visite[s] = true;
    parent[s] = -1;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        for (int v=0; v<V; v++)
        {
            if (visite[v]==false && rGraphe[u][v] > 0)
            {
                q.push(v);
                parent[v] = u;
                visite[v] = true;
            }
        }
    }

    return (visite[t] == true);
}

int fordFulkerson(int graphe[V][V], int s, int t)
{
    int u, v;

    int rGraphe[V][V];

    for (u = 0; u < V; u++)
        for (v = 0; v < V; v++)
             rGraphe[u][v] = graphe[u][v];

    int parent[V];

    int flotMax = 0;


    while (bfs(rGraphe, s, t, parent))
    {

        int cheminFlot = INT_MAX;
        for (v=t; v!=s; v=parent[v])
        {
            u = parent[v];
            cheminFlot = min(cheminFlot, rGraphe[u][v]);
        }

        for (v=t; v != s; v=parent[v])
        {
            u = parent[v];
            rGraphe[u][v] -= cheminFlot;
            rGraphe[v][u] += cheminFlot;
        }

        flotMax += cheminFlot;
    }

    return flotMax;
}

int main()
{
    int graphe[V][V] = { {0, 100, 100, 0, 0, 0, 0, 0, 0}, //t
                        {0, 0, 0, 60, 0, 30, 0, 0, 0}, //E1
                        {0, 0, 0, 50, 40, 0, 0, 0, 0}, //E2
                        {0, 0, 0, 0, 20, 30, 50, 40, 0}, //F1
                        {0, 0, 0, 0, 0, 0, 0, 50, 0}, //F2
                        {0, 0, 0, 0, 0, 0, 0, 0, 60}, //A1
                        {0, 0, 0, 0, 0, 0, 0, 0, 50}, //A2
                        {0, 0, 0, 0, 0, 0, 0, 0, 90}, //A3
                        {0, 0, 0, 0, 0, 0, 0, 0, 0} //s
                      };

    cout << "Le flot maximal est de : " << fordFulkerson(graphe, 0, 8)<<"\n";

    return 0;
}
