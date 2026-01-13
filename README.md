# C++ Concurrent Task Execution Runtime

## Overview

This project is a **bounded, multithreaded task execution runtime** implemented in C++.
The intention here is to mainly explore thread pools, synchronization, backpressure, and some sample runtime observability in C++.

Tasks represent file-based workloads (e.g., encryption/decryption) and are executed concurrently by a fixed set of worker threads.

---

## Key Features

- **Bounded task queue with backpressure**
  Producers block when the queue reaches a configurable maximum size, preventing unbounded growth under load.

- **Worker thread pool**
  A fixed number of worker threads execute tasks concurrently using a shared queue protected by mutexes and condition variables.

- **Safe task scheduling**
  Tasks are dequeued under mutual exclusion, ensuring each task is executed exactly once.

- **Runtime metrics**
  Atomic metrics track:

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
