/*
 * 2-Approximation Algorithm for Metric TSP
 * -----------------------------------------
 * Problem: A delivery boy must visit 15 locations where the
 *          triangle inequality holds (metric space).
 *
 * Algorithm Steps:
 *   1. Compute pairwise distance matrix (Euclidean — satisfies triangle inequality)
 *   2. Find a Minimum Spanning Tree (MST) using Prim's Algorithm
 *   3. Perform a DFS Preorder Traversal of the MST
 *   4. Output the traversal order as the Hamiltonian cycle (tour)
 *
 * Guarantee: Tour cost <= 2 * OPT
 *   - MST cost <= OPT  (any TSP tour contains a spanning tree)
 *   - DFS traversal <= 2 * MST cost (each edge traversed at most twice)
 *   - Triangle inequality lets us shortcut revisits without increasing cost
 *
 * Time Complexity: O(n^2) for Prim's + O(n) for DFS = O(n^2)
 */

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define N 15          /* Number of delivery locations */
#define INF 1e9       /* Infinity for initialization */

/* -----------------------------------------------------------------------
 * City coordinates (x, y) — represents locations in the city grid
 * In a real scenario, these come from GPS/map data.
 * ----------------------------------------------------------------------- */
double x[N] = {2, 5, 8, 1, 9, 4, 7, 3, 6, 0, 10, 5, 8, 2, 6};
double y[N] = {3, 7, 2, 6, 5, 1, 9, 4, 8, 2,  3, 5, 7, 9, 0};

/* Distance matrix */
double dist[N][N];

/* MST adjacency list */
int adj[N][N];     /* adj[u] holds neighbors of u in MST */
int deg[N];        /* degree of each node in MST */

/* DFS state */
bool visited[N];
int tour[N + 1];   /* +1 to return to start */
int tour_idx;

/* -----------------------------------------------------------------------
 * Compute Euclidean distance between all pairs of cities.
 * Euclidean distance satisfies the triangle inequality:
 *   dist(a, c) <= dist(a, b) + dist(b, c)
 * ----------------------------------------------------------------------- */
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

/* -----------------------------------------------------------------------
 * Prim's Algorithm to build the Minimum Spanning Tree (MST).
 *
 * Key idea: Greedily pick the cheapest edge that connects an
 * unvisited node to the current tree.
 *
 * Why MST? Its cost is a lower bound on OPT:
 *   OPT tour - any one edge = spanning tree, so MST <= OPT
 * ----------------------------------------------------------------------- */
void build_mst() {
    printf("=== Step 2: Building MST using Prim's Algorithm ===\n");

    double key[N];        /* Minimum edge weight to reach node i */
    int parent[N];        /* Parent of node i in MST */
    bool in_mst[N];       /* Whether node i is already in MST */

    for (int i = 0; i < N; i++) {
        key[i] = INF;
        in_mst[i] = false;
        parent[i] = -1;
    }

    key[0] = 0.0;  /* Start building MST from city 0 */

    double mst_cost = 0.0;

    for (int count = 0; count < N; count++) {
        /* Pick the unvisited node with minimum key */
        int u = -1;
        for (int v = 0; v < N; v++) {
            if (!in_mst[v] && (u == -1 || key[v] < key[u]))
                u = v;
        }

        in_mst[u] = true;

        if (parent[u] != -1) {
            /* Add edge (parent[u], u) to MST adjacency list */
            int p = parent[u];
            adj[p][deg[p]++] = u;
            adj[u][deg[u]++] = p;
            mst_cost += dist[p][u];
            printf("  MST Edge: City %2d -- City %2d  (dist = %.2f)\n",
                   p + 1, u + 1, dist[p][u]);
        }

        /* Update keys for neighbors of u */
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

/* -----------------------------------------------------------------------
 * DFS Preorder Traversal of the MST.
 *
 * Records cities in the order they are first visited.
 * This gives the approximate TSP tour order.
 *
 * The triangle inequality guarantees that taking shortcuts
 * (skipping already-visited nodes) does not increase cost.
 * ----------------------------------------------------------------------- */
void dfs(int u) {
    visited[u] = true;
    tour[tour_idx++] = u;

    for (int i = 0; i < deg[u]; i++) {
        int v = adj[u][i];
        if (!visited[v]) {
            dfs(v);
        }
    }
}

/* -----------------------------------------------------------------------
 * Compute the total cost of the resulting tour.
 * ----------------------------------------------------------------------- */
double compute_tour_cost() {
    double total = 0.0;
    for (int i = 0; i < N; i++) {
        total += dist[tour[i]][tour[i + 1]];
    }
    return total;
}

/* -----------------------------------------------------------------------
 * Main driver
 * ----------------------------------------------------------------------- */
int main() {
    printf("=======================================================\n");
    printf("   2-Approximation Algorithm for Metric TSP\n");
    printf("   %d Delivery Locations — Triangle Inequality Holds\n", N);
    printf("=======================================================\n\n");

    /* Initialize MST adjacency structure */
    memset(adj, 0, sizeof(adj));
    memset(deg, 0, sizeof(deg));
    memset(visited, false, sizeof(visited));
    tour_idx = 0;

    /* Step 1: Compute distances */
    compute_distances();

    /* Step 2: Build MST */
    build_mst();

    /* Step 3: DFS Preorder Traversal */
    printf("=== Step 3: DFS Preorder Traversal of MST ===\n");
    dfs(0);
    tour[N] = tour[0];   /* Return to starting city to complete the cycle */

    printf("Visit order: ");
    for (int i = 0; i <= N; i++) {
        printf("City %d", tour[i] + 1);
        if (i < N) printf(" -> ");
    }
    printf("\n\n");

    /* Step 4: Compute and display tour cost */
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
 *
 * ALGORITHM ANALYSIS:
 *   Time Complexity  : O(n^2)  — Prim's dominates
 *   Space Complexity : O(n^2)  — distance matrix
 *   Approximation    : <= 2 * OPT (proven via triangle inequality)
 *
 * WHY 2-APPROXIMATION WORKS:
 *   1. MST cost  <= OPT  (tour minus one edge is a spanning tree)
 *   2. DFS tour  <= 2 * MST  (each MST edge traversed at most twice)
 *   3. Shortcuts <= DFS tour (triangle inequality — shortcutting never increases cost)
 *   Therefore:  Approx Tour <= 2 * OPT
 */
