# OS-assignment-6: Deadlock and Synchronization

## Overview

This assignment demonstrates deadlock detection, deadlock prevention, resource allocation, and synchronization in the xv6 operating system.

## Programs

* **bankers.c** – Implements Banker's Algorithm to determine safe states, generate safe sequences, and handle resource requests.
* **deadlockdetect.c** – Demonstrates deadlock detection using a wait-for graph and cycle detection.
* **resourceorder.c** – Demonstrates deadlock creation through improper resource ordering and prevents it using a fixed resource ordering.
* **syncdeadlock.c** – Demonstrates synchronization and deadlock avoidance using semaphores and resource ordering with five processes and three resource types.

## Kernel Modifications

* Added semaphore system calls: `sem_wait()` and `sem_signal()`.
* Updated system call numbers, declarations, and dispatch tables.
* Implemented semaphore operations in the xv6 kernel.
* Configured resource instances for the synchronization demonstration.

## Execution

Start xv6 using:

```bash
make qemu
```

Run the required programs from the xv6 shell:

```bash
$ bankers
$ deadlockdetect
$ resourceorder
$ syncdeadlock
```

## Conclusion

The assignment demonstrates how deadlocks can occur when processes compete for resources and how Banker's Algorithm, deadlock detection, resource ordering, and synchronization mechanisms help manage and prevent deadlocks.

