#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <algorithm>

class InputUtils {
public:
    static int getPositiveInt(const std::string& prompt, int maxValue = 1000000) {
        int value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value && value > 0 && value <= maxValue) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            } else {
                std::cout << "Ошибка! Введите положительное целое число";
                if (maxValue < 1000000) {
                    std::cout << " не больше " << maxValue;
                }
                std::cout << ".\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }

    static double getPositiveDouble(const std::string& prompt) {
        double value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value && value > 0) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            } else {
                std::cout << "Ошибка! Введите положительное число.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }

    static std::string getString(const std::string& prompt) {
        std::string value;
        std::cout << prompt;
        std::getline(std::cin, value);
        return value;
    }

    static void clearInputBuffer() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    static bool getYesNo(const std::string& prompt) {
        std::string answer;
        while (true) {
            std::cout << prompt << " (y/n): ";
            std::getline(std::cin, answer);
            if (answer == "y" || answer == "Y") return true;
            if (answer == "n" || answer == "N") return false;
            std::cout << "Ошибка! Введите 'y' или 'n'.\n";
        }
    }

    static void printVector(const std::vector<int>& vec, const std::string& title = "") {
        if (!title.empty()) {
            std::cout << title;
        }
        for (size_t i = 0; i < vec.size(); ++i) {
            std::cout << vec[i];
            if (i < vec.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
};

#endif