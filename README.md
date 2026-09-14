# OS-assignment-6: Deadlock and Synchronization

## Overview

This assignment demonstrates deadlock detection, deadlock prevention, resource ordering, and semaphore-based synchronization in the xv6 operating system.

## Programs

* **deadlockdetect.c** – Demonstrates deadlock detection using a wait-for graph.
* **resourceorder.c** – Demonstrates deadlock creation through improper resource ordering and prevents it using a fixed resource ordering.
* **syncdeadlock.c** – Demonstrates synchronization and deadlock avoidance using semaphores and resource ordering.

## Kernel Modifications

* Added semaphore system calls: `sem_wait()` and `sem_signal()`.
* Updated system call numbers, declarations, and dispatch tables.
* Implemented semaphore operations in the xv6 kernel.

## Execution

To run the programs, start xv6 using:

```bash
make qemu
```

Then execute the required program from the xv6 shell:

```bash
$ deadlockdetect
$ resourceorder
$ syncdeadlock
```

## Conclusion

The assignment demonstrates how deadlocks occur when processes compete for resources and how synchronization mechanisms and consistent resource ordering can help prevent them.
