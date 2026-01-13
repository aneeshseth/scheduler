# C++ Concurrent Task Execution Runtime

## Overview

This project is a bounded, multithreaded task execution runtime implemented in C++.
The intention here is to mainly explore thread pools, synchronization, backpressure, and some sample runtime observability in C++.

Tasks represent file-based workloads (e.g., encryption/decryption) and are executed concurrently by a fixed set of worker threads.

---


https://github.com/user-attachments/assets/022677c6-69e1-4516-940f-9e848ebc9e62

This video shows multiple worker threads concurrently picking up tasks from the bounded queue, with overlapping execution across distinct thread IDs.
This shows parallel task execution, backpressure at the queue boundary (seen via the metrics showing the max queue depth being 3, however number of tasks in queue being much larger), and correct task completion under concurrent load.


## Key Features

- Producers block when the queue reaches a configurable maximum size, preventing unbounded growth under load.

- A fixed number of worker threads execute tasks concurrently using a shared queue protected by mutexes and condition variables.

- Tasks are dequeued under mutual exclusion, ensuring each task is executed exactly once.

- Atomic metrics track:
    - tasks submitted
    - tasks completed
    - maximum observed queue depth

---

## Architecture

```
Main Thread
    |
    v
Bounded Task Queue
    |
    v
Worker Threads (Thread Pool)
    |
    v
Task Execution (File-based workload)
```

---

## Observability

The runtime shows some basic metrics after execution completes. Ideally these metrics would go to a time series DB like prometheus so that the data is exposed via an API.

---

## Future Improvements

- Per-resource (file-level) mutual exclusion
- Configurable worker count via CLI flags
- Process-based execution mode for fault isolation
