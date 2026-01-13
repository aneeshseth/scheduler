#include "../core/process_mgmt.hpp"
#include "../core/task.hpp"
#include <iostream>

#include "../core/process_mgmt.hpp"
#include "../core/task.hpp"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    ProcessMgmt pm;


    pm.submitToQueue(Task("a.txt", Action::ENCRYPT));
    pm.submitToQueue(Task("b.txt", Action::ENCRYPT));
    pm.submitToQueue(Task("c.txt", Action::ENCRYPT));
    pm.submitToQueue(Task("c.txt", Action::ENCRYPT));
    pm.submitToQueue(Task("c.txt", Action::ENCRYPT));
    pm.submitToQueue(Task("a.txt", Action::DECRYPT));
    pm.submitToQueue(Task("b.txt", Action::DECRYPT));
    pm.submitToQueue(Task("c.txt", Action::DECRYPT));
    pm.submitToQueue(Task("c.txt", Action::ENCRYPT));
    pm.submitToQueue(Task("c.txt", Action::ENCRYPT));


    std::this_thread::sleep_for(std::chrono::seconds(2));

    pm.printMetrics();

    std::cout << "main function leaving...";
    return 0;
}