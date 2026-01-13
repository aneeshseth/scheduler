#include "process_mgmt.hpp"
#include "../file_handling/io.hpp"
#include "../encryption_decryption/cryption.hpp"

ProcessMgmt::ProcessMgmt() {
    workers.emplace_back(&ProcessMgmt::worker, this);
    workers.emplace_back(&ProcessMgmt::worker, this);
}

void ProcessMgmt::printMetrics() const {
    std::cout << "\n=== Metrics ===\n";
    std::cout << "Tasks submitted: " << metrics.tasksSubmitted.load() << "\n";
    std::cout << "Tasks completed: " << metrics.tasksCompleted.load() << "\n";
    std::cout << "Max queue depth: " << metrics.maxQueueDepth.load() << "\n";
}

void ProcessMgmt::submitToQueue(Task task) {
    std::unique_lock<std::mutex> lock(mtx);

    spaceAvailable.wait(lock, [&] {
        return queue.size() < MAX_QUEUE_SIZE;
    });

    queue.push(std::move(task));
    metrics.tasksSubmitted++;

    cv.notify_one(); 

    size_t currentDepth = queue.size();
    metrics.maxQueueDepth.store(
        std::max(metrics.maxQueueDepth.load(), currentDepth)
    );
}

void ProcessMgmt::worker() {
    while (true) {
        Task task("", Action::ENCRYPT);

        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [&] {
                return stop || !queue.empty();
            });

            if (stop && queue.empty())
                return;

            task = queue.front();
            queue.pop();
            spaceAvailable.notify_one();
        }

        int result = executeCryption(task.file_path, task.action);
        if (result == 0) {
            metrics.tasksCompleted++;
        }
    }
}

ProcessMgmt::~ProcessMgmt() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        stop = true;
    }
    cv.notify_all();

    for (auto& t : workers)
        t.join();
}
