#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define N 4

int graph[N][N];

int visited[N];
int in_stack[N];
int path[N];
int path_len;


// Clear the wait-for graph
void clear_graph()
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            graph[i][j] = 0;
        }
    }
}


// Print the wait-for graph
void print_graph()
{
    printf("Wait-for graph:\n");

    for (int i = 0; i < N; i++) {

        for (int j = 0; j < N; j++) {

            if (graph[i][j]) {
                printf("P%d -> P%d\n", i, j);
            }
        }
    }
}


// DFS cycle detection
int dfs(int u)
{
    visited[u] = 1;
    in_stack[u] = 1;

    path[path_len] = u;
    path_len++;

    for (int v = 0; v < N; v++) {

        if (!graph[u][v])
            continue;

        // Visit unvisited process
        if (!visited[v]) {

            if (dfs(v))
                return 1;

        }

        // Back edge means a cycle
        else if (in_stack[v]) {

            int start = 0;

            while (start < path_len &&
                   path[start] != v) {
                start++;
            }

            printf("Deadlock cycle: ");

            for (int i = start; i < path_len; i++) {
                printf("P%d -> ", path[i]);
            }

            printf("P%d\n", v);

            return 1;
        }
    }

    path_len--;
    in_stack[u] = 0;

    return 0;
}


// Detect deadlock in graph
int detect_deadlock()
{
    for (int i = 0; i < N; i++) {
        visited[i] = 0;
        in_stack[i] = 0;
    }

    path_len = 0;

    for (int i = 0; i < N; i++) {

        if (!visited[i]) {

            if (dfs(i))
                return 1;
        }
    }

    return 0;
}


// Scenario 1: No deadlock
void scenario_no_deadlock()
{
    printf("\n===== SCENARIO 1: NO DEADLOCK =====\n");

    clear_graph();

    /*
       P0 waits for P1
       P1 waits for P2
       P2 waits for P3
       P3 waits for nobody
    */

    graph[0][1] = 1;
    graph[1][2] = 1;
    graph[2][3] = 1;

    print_graph();

    if (detect_deadlock()) {
        printf("Deadlock detected.\n");
    }
    else {
        printf("No deadlock detected.\n");
    }
}


// Scenario 2: Deadlock
void scenario_deadlock()
{
    printf("\n===== SCENARIO 2: DEADLOCK =====\n");

    clear_graph();

    /*
       Circular wait:
       P0 -> P1
       P1 -> P2
       P2 -> P0
    */

    graph[0][1] = 1;
    graph[1][2] = 1;
    graph[2][0] = 1;

    print_graph();

    if (detect_deadlock()) {
        printf("Deadlock detected.\n");
    }
    else {
        printf("No deadlock detected.\n");
    }
}


int main()
{
    printf("===== DEADLOCK DETECTION USING WAIT-FOR GRAPH =====\n");

    scenario_no_deadlock();

    scenario_deadlock();

    exit(0);
}
