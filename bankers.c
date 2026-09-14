#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define N 5
#define M 3

// Allocation matrix
int Allocation[N][M] = {
    {0, 1, 0},
    {2, 0, 0},
    {3, 0, 2},
    {2, 1, 1},
    {0, 0, 2}
};

// Maximum demand matrix
int Max[N][M] = {
    {7, 5, 3},
    {3, 2, 2},
    {9, 0, 2},
    {4, 2, 2},
    {5, 3, 3}
};

// Available resources
int Available[M] = {3, 3, 2};

// Need = Max - Allocation
int Need[N][M];


// Calculate Need matrix
void calculate_need()
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            Need[i][j] = Max[i][j] - Allocation[i][j];
        }
    }
}


// Safety Algorithm
int is_safe(int safe_sequence[])
{
    int Work[M];
    int Finish[N];

    // Work = Available
    for (int j = 0; j < M; j++) {
        Work[j] = Available[j];
    }

    // Initially no process is finished
    for (int i = 0; i < N; i++) {
        Finish[i] = 0;
    }

    int count = 0;

    while (count < N) {

        int found = 0;

        for (int i = 0; i < N; i++) {

            // Skip already finished process
            if (Finish[i])
                continue;

            int possible = 1;

            // Check Need[i] <= Work
            for (int j = 0; j < M; j++) {

                if (Need[i][j] > Work[j]) {
                    possible = 0;
                    break;
                }
            }

            // Process can finish
            if (possible) {

                // Release its allocated resources
                for (int j = 0; j < M; j++) {
                    Work[j] += Allocation[i][j];
                }

                Finish[i] = 1;

                safe_sequence[count] = i;
                count++;

                found = 1;
            }
        }

        // No process could finish
        if (!found)
            return 0;
    }

    return 1;
}


// Print safe sequence
void print_sequence(int sequence[])
{
    printf("Safe sequence: ");

    for (int i = 0; i < N; i++) {

        printf("P%d", sequence[i]);

        if (i != N - 1)
            printf(" -> ");
    }

    printf("\n");
}


// Resource Request Algorithm
int request_resources(int process, int request[])
{
    printf("\nP%d requests: [%d %d %d]\n",
           process,
           request[0],
           request[1],
           request[2]);


    // Step 1: Request <= Need
    for (int j = 0; j < M; j++) {

        if (request[j] > Need[process][j]) {

            printf("Request denied - exceeds process Need\n");

            return 0;
        }
    }


    // Step 2: Request <= Available
    for (int j = 0; j < M; j++) {

        if (request[j] > Available[j]) {

            printf("Request denied - resources unavailable\n");

            return 0;
        }
    }


    // Step 3: Pretend to allocate resources
    for (int j = 0; j < M; j++) {

        Available[j] -= request[j];

        Allocation[process][j] += request[j];

        Need[process][j] -= request[j];
    }


    // Step 4: Check safety
    int sequence[N];

    if (is_safe(sequence)) {

        printf("Request granted.\n");

        print_sequence(sequence);

        return 1;
    }


    // Step 5: Rollback if unsafe
    for (int j = 0; j < M; j++) {

        Available[j] += request[j];

        Allocation[process][j] -= request[j];

        Need[process][j] += request[j];
    }

    printf("Request denied - would lead to unsafe state\n");

    return 0;
}


int main()
{
    int sequence[N];

    printf("===== BANKER'S ALGORITHM =====\n");


    // Calculate Need matrix
    calculate_need();


    // Check initial safety
    printf("\nInitial system safety check:\n");

    if (is_safe(sequence)) {

        printf("System is in a SAFE state.\n");

        print_sequence(sequence);

    } else {

        printf("System is in an UNSAFE state.\n");
    }


    // Scenario 1
    // Safe request by P1
    int request1[M] = {1, 0, 2};

    printf("\n--- Scenario 1: Safe Request ---\n");

    request_resources(1, request1);


    // Scenario 2
    // Request that makes the system unsafe
    int request2[M] = {0, 3, 0};

    printf("\n--- Scenario 2: Unsafe Request ---\n");

    request_resources(4, request2);


    exit(0);
}

