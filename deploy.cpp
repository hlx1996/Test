#include "deploy.h"
#include <stdio.h>
#include <cstring>
#include <queue>
#include <string>

// TODO: Modify this
#define MAX_M 50000
#define MAX_N 2000
#define MEMSET_63 1061109567
#define INF 2147483647
#define BIG_
using namespace std;

class Graph {
private:
    // Basic Graph
    struct edge {
        int v, c, w, next;
    } e[MAX_M];
    int point[MAX_N];
    int tot;
    // For SPFA && MCFP
    bool v[MAX_N];
    int p[MAX_N], pre[MAX_N], pp[MAX_N];
    int maxflow, mincost;

public:
    Graph() {
        tot = 0;
        memset(point, 255, sizeof(point));
    }

    int addEdge(int u, int v, int c, int w) {
        e[tot] = edge{v, c, w, point[u]};
        point[u] = tot++;
        e[tot] = edge{u, 0, -w, point[v]};
        point[v] = tot++;
        return tot;
    }

    // 0 ~ n - 1 is the real vertex in the graph
    bool SPFA(int s, int t, int n) {
        queue<int> q;
        memset(v, 0, sizeof(bool) * n);
        memset(p, 63, sizeof(int) * n);
        p[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            v[u] = false;
            for (int i = point[u]; i != -1; i = e[i].next) {
                if (e[i].c > 0 && p[u] + e[i].w < p[e[i].v]) {
                    p[e[i].v] = p[u] + e[i].w;
                    pre[e[i].v] = u;
                    pp[e[i].v] = i;
                    if (!v[e[i].v]) {
                        v[e[i].v] = true;
                        q.push(e[i].v);
                    }
                }
            }
        }
        return p[t] != MEMSET_63;
    }


    bool MCFP(int s, int t, int n) {
        maxflow = mincost = 0;
        while (SPFA(s, t, n)) {
            int d = INF;
            for (int i = t; i != s; i = pre[i])
                d = min(d, e[pp[i]].c);
            maxflow += d;
            mincost += d * p[t];
            for (int i = t; i != s; i = pre[i]) {
                e[pp[i]].c -= d;
                e[pp[i] ^ 1].c += d;
            }
        }
        // TODO: Modify this
        return 1;
    }

    int getMaxflow() const {
        return maxflow;
    }

    int getMincost() const {
        return mincost;
    }

    int getNumOfUsed(int n) {
        int j = tot - 1, ans = 0;
        for (int i = 0; i < n; i++, j -= 4) {
            ans += e[j].c;
        }
        return ans;
    }
};


void deploy_server(char *topo[MAX_EDGE_NUM], int line_num, char *filename) {
    int n, m, k, serverCost, u, v, c, w, s, t, cost, N, must;
    string str;
    Graph G;


    // Input Data
    sscanf(topo[0], "%d %d %d", &n, &m, &k);
    s = n;
    t = n + 1;
    N = n + 2;

    sscanf(topo[2], "%d", &serverCost);
    for (int i = 0; i < m; i++) {
        sscanf(topo[i + 4], "%d %d %d %d", &u, &v, &c, &w);
        G.addEdge(u, v, c, w);
        G.addEdge(v, u, c, w);

    }

    must = 0;
    for (int i = 0; i < k; i++) {
        sscanf(topo[i + m + 5], "%d %d %d", &u, &v, &c);
        G.addEdge(v, t, c, 0);
        // TODO: Mark U
        must += c;
    }

    cost = serverCost;
    for (int i = 0; i < n; i++) {
        G.addEdge(s, i, INF, 0);
        G.addEdge(s, i, 1, -cost);
    }

    G.MCFP(s, t, N);
    printf("%d\n%d + %d * %d = %d\n", G.getMaxflow(), G.getMincost(), G.getNumOfUsed(n), cost,
           G.getMincost() + G.getNumOfUsed(n) * cost);

    write_result(str.c_str(), filename);
}
