#include <cstdlib>
#include <string>

class ReadEnv {
public:
    std::string getEnv() {
        const char* val = std::getenv("KEY");
        if (!val) {
            return "";
        }
        return std::string(val);
    }
};