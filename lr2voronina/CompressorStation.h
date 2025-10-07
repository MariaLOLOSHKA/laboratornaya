#ifndef COMPRESSOR_STATION_H
#define COMPRESSOR_STATION_H

#include <iostream>
#include <fstream>
#include <string>

class CompressorStation {
private:
    std::string name;
    int totalWorkshops;
    int workingWorkshops;
    int stationClass;

public:
    // Конструктор
    CompressorStation();
    
    // Ввод данных с консоли
    void readFromConsole();
    
    // Вывод данных на консоль
    void printToConsole() const;
    
    // Запуск цеха
    void startWorkshop();
    
    // Остановка цеха
    void stopWorkshop();
    
    // Сохранение в файл
    void saveToFile(std::ofstream& out) const;
    
    // Загрузка из файла
    void loadFromFile(std::ifstream& in);
    
    // Геттеры
    std::string getName() const;
    int getWorkingWorkshops() const;
    int getTotalWorkshops() const;
};

#endif