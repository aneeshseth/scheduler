#ifndef TASK_HPP
#define TASK_HPP

#include <string>

enum class Action {
    ENCRYPT,
    DECRYPT
};

struct Task {
    std::string file_path;
    Action action;

    Task(const std::string& path, Action act)
        : file_path(path), action(act) {}
};

#endif
