#ifndef PIPE_H
#define PIPE_H

#include <iostream>
#include <fstream>
#include <string>

class Pipe {
private:
    int id;
    std::string name;
    double length;
    int diameter;
    bool underRepair;
    
    static int nextId;

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
    
    void readFromConsole();
    void printToConsole() const;
    void toggleRepair();
    void saveToFile(std::ofstream& out) const;
    void loadFromFile(std::ifstream& in);
    
    static int getNextId() { return nextId; }
    static void setNextId(int newId) { nextId = newId; }
};

#endif