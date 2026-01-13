#include "cryption.hpp"
#include "../file_handling/io.hpp"
#include "../file_handling/read_env.cpp"
#include "../core/task.hpp"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <thread>
#include <mutex> 

std::mutex logMutex;

int executeCryption(const std::string& filePath, Action action) {

    {
        std::lock_guard<std::mutex> lock(logMutex);
        std::cout << "Thread " << std::this_thread::get_id()
                << " processing " << filePath
                << " (" << (action == Action::ENCRYPT ? "ENCRYPT" : "DECRYPT")
                << ")\n";
    }

    ReadEnv env;
    std::string envKey = env.getEnv();
    int key = std::stoi(envKey);

    IO io(filePath);
    std::fstream file = io.getFileStream();

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return -1;
    }

    char ch;
    if (action == Action::ENCRYPT) {
        while (file.get(ch)) {
            ch = (ch + key) % 256;
            file.seekp(-1, std::ios::cur);
            file.put(ch);
        }
    } else {
        while (file.get(ch)) {
            ch = (ch - key + 256) % 256;
            file.seekp(-1, std::ios::cur);
            file.put(ch);
        }
    }


    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    {
        std::lock_guard<std::mutex> lock(logMutex);
        std::cout << "Finished "
                << (action == Action::ENCRYPT ? "encryption" : "decryption")
                << " at: " << std::put_time(now, "%Y-%m-%d %H:%M:%S")
                << std::endl;
    }

    return 0;
}
