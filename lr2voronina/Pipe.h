#ifndef PIPE_H
#define PIPE_H

#include <iostream>
#include <fstream>
#include <string>

class Pipe {
private:
    std::string name;
    double length;
    int diameter;
    bool underRepair;

public:
    // Конструктор
    Pipe();
    
    // Ввод данных с консоли
    void readFromConsole();
    
    // Вывод данных на консоль
    void printToConsole() const;
    
    // Редактирование признака "в ремонте"
    void toggleRepair();
    
    // Сохранение в файл
    void saveToFile(std::ofstream& out) const;
    
    // Загрузка из файла
    void loadFromFile(std::ifstream& in);
    
    // Геттеры
    std::string getName() const;
    bool isUnderRepair() const;
};

#endif