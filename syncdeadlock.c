#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define PRINTER 0
#define SCANNER 1
#define DISK    2

#define CYCLES 3
#define DELAY 1000000


// Busy-wait to simulate work
void delay()
{
    volatile int i;

    for (i = 0; i < DELAY; i++)
        ;
}


// Get the name of a resource
char *resource_name(int resource)
{
    if (resource == PRINTER)
        return "Printer";

    if (resource == SCANNER)
        return "Scanner";

    return "Disk";
}


// Perform one cycle for a process
void do_work(int process, int first, int second)
{
    printf("P%d: requesting %s\n",
           process,
           resource_name(first));

    sem_wait(first);

    printf("P%d: granted %s\n",
           process,
           resource_name(first));

    printf("P%d: requesting %s\n",
           process,
           resource_name(second));

    sem_wait(second);

    printf("P%d: granted %s\n",
           process,
           resource_name(second));

    printf("P%d: starting work\n", process);

    delay();

    printf("P%d: releasing %s\n",
           process,
           resource_name(second));

    sem_signal(second);

    printf("P%d: releasing %s\n",
           process,
           resource_name(first));

    sem_signal(first);

    printf("P%d: cycle completed\n", process);
}


// Process function
void process_function(int process, int first, int second)
{
    for (int cycle = 1; cycle <= CYCLES; cycle++) {

        printf("\nP%d: ===== Cycle %d =====\n",
               process,
               cycle);

        do_work(process, first, second);

        /*
         * Small delay before the next cycle so that
         * other processes get an opportunity to run.
         */
        delay();
    }

    printf("\nP%d: ALL CYCLES COMPLETED\n", process);

    exit(0);
}


int main()
{
    int pid;

    printf("========================================\n");
    printf(" COMBINED SYNCHRONIZATION & DEADLOCK\n");
    printf(" AVOIDANCE DEMONSTRATION\n");
    printf("========================================\n");

    printf("\nResource instances:\n");
    printf("Printer = 2\n");
    printf("Scanner = 1\n");
    printf("Disk    = 2\n");

    printf("\nDeadlock prevention strategy:\n");
    printf("RESOURCE ORDERING\n");
    printf("Global order: Printer < Scanner < Disk\n");

    /*
     * P0 needs Printer + Scanner
     */
    pid = fork();

    if (pid == 0)
        process_function(0, PRINTER, SCANNER);


    /*
     * P1 needs Printer + Disk
     */
    pid = fork();

    if (pid == 0)
        process_function(1, PRINTER, DISK);


    /*
     * P2 needs Scanner + Disk
     */
    pid = fork();

    if (pid == 0)
        process_function(2, SCANNER, DISK);


    /*
     * P3 needs Printer + Scanner
     */
    pid = fork();

    if (pid == 0)
        process_function(3, PRINTER, SCANNER);


    /*
     * P4 needs Printer + Disk
     */
    pid = fork();

    if (pid == 0)
        process_function(4, PRINTER, DISK);


    /*
     * Wait for all five children.
     */
    for (int i = 0; i < 5; i++)
        wait(0);


    printf("\n========================================\n");
    printf(" ALL 5 PROCESSES COMPLETED\n");
    printf(" NO DEADLOCK OCCURRED\n");
    printf("========================================\n");

    exit(0);
}
