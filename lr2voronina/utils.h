#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <limits>
#include <string>

// Функция для получения положительного целого числа
inline int getPositiveInt(const std::string& prompt, int maxValue = INT_MAX) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value > 0 && value <= maxValue) {
            return value;
        } else {
            std::cout << "Ошибка! Введите положительное целое число";
            if (maxValue < INT_MAX) {
                std::cout << " не больше " << maxValue;
            }
            std::cout << ".\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

// Функция для получения положительного дробного числа
inline double getPositiveDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value > 0) {
            return value;
        } else {
            std::cout << "Ошибка! Введите положительное число.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

// Функция для очистки экрана (кроссплатформенная)
inline void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Функция для паузы
inline void pause() {
    std::cout << "Нажмите Enter для продолжения...";
    std::cin.ignore();
    std::cin.get();
}

#endif