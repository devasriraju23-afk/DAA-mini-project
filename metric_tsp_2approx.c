

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define N 15          /* Number of delivery locations */
#define INF 1e9       /* Infinity for initialization */


double x[N] = {2, 5, 8, 1, 9, 4, 7, 3, 6, 0, 10, 5, 8, 2, 6};
double y[N] = {3, 7, 2, 6, 5, 1, 9, 4, 8, 2,  3, 5, 7, 9, 0};


double dist[N][N];


int adj[N][N];     
int deg[N];       

bool visited[N];
int tour[N + 1];  
int tour_idx;


void compute_distances() {
    printf("=== Step 1: Computing Distance Matrix ===\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double dx = x[i] - x[j];
            double dy = y[i] - y[j];
            dist[i][j] = sqrt(dx * dx + dy * dy);
        }
    }
    printf("Distance matrix computed for %d cities.\n\n", N);
}


void build_mst() {
    printf("=== Step 2: Building MST using Prim's Algorithm ===\n");

    double key[N];        
    int parent[N];        
    bool in_mst[N];       

    for (int i = 0; i < N; i++) {
        key[i] = INF;
        in_mst[i] = false;
        parent[i] = -1;
    }

    key[0] = 0.0; 

    double mst_cost = 0.0;

    for (int count = 0; count < N; count++) {
       
        int u = -1;
        for (int v = 0; v < N; v++) {
            if (!in_mst[v] && (u == -1 || key[v] < key[u]))
                u = v;
        }

        in_mst[u] = true;

        if (parent[u] != -1) {
           
            int p = parent[u];
            adj[p][deg[p]++] = u;
            adj[u][deg[u]++] = p;
            mst_cost += dist[p][u];
            printf("  MST Edge: City %2d -- City %2d  (dist = %.2f)\n",
                   p + 1, u + 1, dist[p][u]);
        }

        
        for (int v = 0; v < N; v++) {
            if (!in_mst[v] && dist[u][v] < key[v]) {
                key[v] = dist[u][v];
                parent[v] = u;
            }
        }
    }

    printf("\nMST Total Cost: %.4f\n", mst_cost);
    printf("(This is a lower bound on the optimal TSP tour)\n\n");
}


    visited[u] = true;
    tour[tour_idx++] = u;

    for (int i = 0; i < deg[u]; i++) {
        int v = adj[u][i];
        if (!visited[v]) {
            dfs(v);
        }
    }
}


double compute_tour_cost() {
    double total = 0.0;
    for (int i = 0; i < N; i++) {
        total += dist[tour[i]][tour[i + 1]];
    }
    return total;
}


int main() {
    printf("=======================================================\n");
    printf("   2-Approximation Algorithm for Metric TSP\n");
    printf("   %d Delivery Locations — Triangle Inequality Holds\n", N);
    printf("=======================================================\n\n");

   
    memset(adj, 0, sizeof(adj));
    memset(deg, 0, sizeof(deg));
    memset(visited, false, sizeof(visited));
    tour_idx = 0;

   
    compute_distances();

    
    build_mst();

   
    printf("=== Step 3: DFS Preorder Traversal of MST ===\n");
    dfs(0);
    tour[N] = tour[0];  

    printf("Visit order: ");
    for (int i = 0; i <= N; i++) {
        printf("City %d", tour[i] + 1);
        if (i < N) printf(" -> ");
    }
    printf("\n\n");

   
    printf("=== Step 4: Final Approximate Tour ===\n");
    printf("Route taken by the delivery boy:\n");
    for (int i = 0; i < N; i++) {
        printf("  City %2d (%.1f, %.1f) -> City %2d (%.1f, %.1f)  dist = %.4f\n",
               tour[i] + 1, x[tour[i]], y[tour[i]],
               tour[i + 1] + 1, x[tour[i + 1]], y[tour[i + 1]],
               dist[tour[i]][tour[i + 1]]);
    }

    double tour_cost = compute_tour_cost();
    printf("\nApproximate Tour Total Cost : %.4f\n", tour_cost);
    printf("Guarantee                  : Tour Cost <= 2 * OPT\n");
    printf("=======================================================\n");

    return 0;
}

/*
 * HOW TO COMPILE AND RUN:
 *   gcc -o tsp metric_tsp_2approx.c -lm
 *   ./tsp
*/
 */
