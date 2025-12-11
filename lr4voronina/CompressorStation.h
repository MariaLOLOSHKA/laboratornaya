#ifndef COMPRESSOR_STATION_H
#define COMPRESSOR_STATION_H

#include <iostream>
#include <fstream>
#include <string>


class CompressorStation {
private:
    int id;
    std::string name;
    int totalWorkshops;
    int workingWorkshops;
    int stationClass;
    
    static int nextId;

public:
    CompressorStation();
    CompressorStation(int id, const std::string& name, int total, int working, int stationClass);
    
    int getId() const { return id; }
    std::string getName() const { return name; }
    int getTotalWorkshops() const { return totalWorkshops; }
    int getWorkingWorkshops() const { return workingWorkshops; }
    int getStationClass() const { return stationClass; }
    double getUnusedPercentage() const { 
        return totalWorkshops > 0 ? (1.0 - (double)workingWorkshops / totalWorkshops) * 100 : 0; 
    }
    
    void setName(const std::string& newName) { name = newName; }
    void setTotalWorkshops(int total) { totalWorkshops = total; }
    void setWorkingWorkshops(int working) { workingWorkshops = working; }
    void setStationClass(int stationClass) { this->stationClass = stationClass; }
    
    void readFromConsole();
    void printToConsole() const;
    void startWorkshop();
    void stopWorkshop();
    void saveToFile(std::ofstream& out) const;
    void loadFromFile(std::ifstream& in);
    
    static int getNextId() { return nextId; }
    static void setNextId(int newId) { nextId = newId; }
};

#endif