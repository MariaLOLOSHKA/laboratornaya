#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

class Logger {
private:
    std::string logFilename;

public:
    Logger(const std::string& filename = "pipeline_log.txt") : logFilename(filename) {}
    
    void log(const std::string& action, const std::string& details = "") {
        std::string timestamp = getCurrentTime();
        std::string message = "[" + timestamp + "] " + action;
        if (!details.empty()) {
            message += " | " + details;
        }
        
        // Вывод в консоль
        std::cout << message << std::endl;
        
        // Запись в файл
        std::ofstream file(logFilename, std::ios::app);
        if (file.is_open()) {
            file << message << std::endl;
            file.close();
        }
    }

private:
    std::string getCurrentTime() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
};

#endif