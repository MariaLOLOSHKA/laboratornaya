#ifndef PIPE_H
#define PIPE_H

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <map> // Добавляем include

class Pipe {
private:
    int id;
    std::string name;
    double length;
    int diameter;
    bool underRepair;
    
    static int nextId;
    
    // Статические константы для производительности
    static const std::map<int, double> PIPE_CAPACITIES;
    static const double CAPACITY_COEFFICIENT;

public:
    Pipe();
    Pipe(int id, const std::string& name, double length, int diameter, bool repair);
    
    int getId() const { return id; }
    std::string getName() const { return name; }
    double getLength() const { return length; }
    int getDiameter() const { return diameter; }
    bool isUnderRepair() const { return underRepair; }

    void setName(const std::string& newName) { name = newName; }
    void setLength(double newLength) { length = newLength; }
    void setDiameter(int newDiameter) { diameter = newDiameter; }
    void setRepair(bool repair) { underRepair = repair; }
    
    // Новые методы для расчета производительности и веса
    double getCapacity() const;
    double getWeight() const;
    
    void readFromConsole();
    void printToConsole() const;
    void toggleRepair();
    void saveToFile(std::ofstream& out) const;
    void loadFromFile(std::ifstream& in);
    
    static int getNextId() { return nextId; }
    static void setNextId(int newId) { nextId = newId; }
};

#endif