#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define LOCK1 0
#define LOCK2 1

#define DELAY 100000


void delay()
{
    volatile int i;

    for (i = 0; i < DELAY; i++)
        ;
}


// ==================================================
// BAD ORDERING
// ==================================================

void process_A_bad(int ready_fd, int go_fd)
{
    char x;

    sem_wait(LOCK1);

    // Tell parent: A now holds Lock1
    write(ready_fd, &x, 1);

    // Wait until parent allows us to request Lock2
    read(go_fd, &x, 1);

    // Lock2 is held by B, so A blocks here
    sem_wait(LOCK2);

    exit(0);
}


void process_B_bad(int ready_fd, int go_fd)
{
    char x;

    sem_wait(LOCK2);

    // Tell parent: B now holds Lock2
    write(ready_fd, &x, 1);

    // Wait until parent allows us to request Lock1
    read(go_fd, &x, 1);

    // Lock1 is held by A, so B blocks here
    sem_wait(LOCK1);

    exit(0);
}


void run_bad_order()
{
    int a_ready[2];
    int b_ready[2];

    int a_go[2];
    int b_go[2];

    int pid1;
    int pid2;

    char x = 'x';

    pipe(a_ready);
    pipe(b_ready);

    pipe(a_go);
    pipe(b_go);

    printf("\n");
    printf("========================================\n");
    printf(" BAD RESOURCE ORDERING\n");
    printf("========================================\n");

    printf("Process A: Lock1 -> Lock2\n");
    printf("Process B: Lock2 -> Lock1\n");

    pid1 = fork();

    if (pid1 == 0)
    {
        close(a_ready[0]);
        close(a_go[1]);

        process_A_bad(a_ready[1], a_go[0]);
    }

    pid2 = fork();

    if (pid2 == 0)
    {
        close(b_ready[0]);
        close(b_go[1]);

        process_B_bad(b_ready[1], b_go[0]);
    }

    /*
     * Wait until A has Lock1.
     */
    read(a_ready[0], &x, 1);

    /*
     * Wait until B has Lock2.
     */
    read(b_ready[0], &x, 1);

    /*
     * Both processes now hold their first lock.
     */
    printf("\n");
    printf("DEADLOCK CREATED:\n");
    printf("A holds Lock1 and waits for Lock2\n");
    printf("B holds Lock2 and waits for Lock1\n");
    printf("Circular wait: A -> B -> A\n");

    /*
     * Now allow both processes to request
     * their second lock.
     */
    write(a_go[1], &x, 1);
    write(b_go[1], &x, 1);

    /*
     * Both children are now permanently blocked.
     */
}


// ==================================================
// FIXED ORDERING
// ==================================================

void process_good(char *name)
{
    printf("%s: requesting Lock1\n", name);
    sem_wait(LOCK1);

    printf("%s: acquired Lock1\n", name);

    delay();

    printf("%s: requesting Lock2\n", name);
    sem_wait(LOCK2);

    printf("%s: acquired Lock2\n", name);

    printf("%s: doing work\n", name);

    delay();

    sem_signal(LOCK2);
    sem_signal(LOCK1);

    printf("%s: completed\n", name);

    exit(0);
}


void run_good_order()
{
    int pid1;
    int pid2;

    printf("\n");
    printf("========================================\n");
    printf(" FIXED RESOURCE ORDERING\n");
    printf("========================================\n");

    printf("Both processes use: Lock1 -> Lock2\n");

    pid1 = fork();

    if (pid1 == 0)
        process_good("Process A");

    pid2 = fork();

    if (pid2 == 0)
        process_good("Process B");

    wait(0);
    wait(0);

    printf("Fixed ordering test completed successfully.\n");
}


// ==================================================
// MAIN
// ==================================================

int main()
{
    printf("========================================\n");
    printf(" RESOURCE ORDERING DEADLOCK DEMO\n");
    printf("========================================\n");

    run_bad_order();

    exit(0);
}
