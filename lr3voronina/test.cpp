#include <iostream>
#include <fstream>
#include <string>

class SimpleLogger {
public:
    void log(const std::string& message) {
        std::cout << message << std::endl;
        std::ofstream file("test_log.txt", std::ios::app);
        if (file.is_open()) {
            file << message << std::endl;
        }
    }
};

int main() {
    SimpleLogger logger;
    logger.log("Тест: программа запущена");
    std::cout << "Программа работает!" << std::endl;
    logger.log("Тест: программа завершена");
    return 0;
}