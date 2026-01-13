#ifndef PROCESS_MGMT_HPP
#define PROCESS_MGMT_HPP

#include "task.hpp"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>

static constexpr size_t MAX_QUEUE_SIZE = 3;

struct Metrics {
    std::atomic<size_t> tasksSubmitted{0};
    std::atomic<size_t> tasksCompleted{0};
    std::atomic<size_t> maxQueueDepth{0};
};

class ProcessMgmt {
public:
    ProcessMgmt();
    ~ProcessMgmt();
    void printMetrics() const; 


    void submitToQueue(Task task);

private:
    void worker();

    std::queue<Task> queue;
    std::mutex mtx;
    std::condition_variable cv;
    std::condition_variable spaceAvailable;
    bool stop = false;

    std::vector<std::thread> workers;

    Metrics metrics;
};

#endif
