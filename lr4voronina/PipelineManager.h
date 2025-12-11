#ifndef PIPELINE_MANAGER_H
#define PIPELINE_MANAGER_H

#include <vector>
#include <map>
#include <functional>
#include "Pipe.h"
#include "CompressorStation.h"
#include "Logger.h"
#include "GasNetwork.h"

class PipelineManager {
private:
    std::map<int, Pipe> pipes;
    std::map<int, CompressorStation> stations;
    GasNetwork network;
    Logger logger;

    std::vector<int> findPipesByName(const std::string& name) const;
    std::vector<int> findPipesByRepairStatus(bool underRepair) const;
    std::vector<int> findStationsByName(const std::string& name) const;
    std::vector<int> findStationsByUnusedPercentage(double minPercent, double maxPercent = 100) const;
    std::vector<int> findFreePipesByDiameter(int diameter) const;

    void batchEditPipes(const std::vector<int>& pipeIds);
    void selectiveEditPipes(const std::vector<int>& pipeIds);

    void deletePipe(int id);
    void deleteStation(int id);
    
    void printAllPipes() const;
    void printAllStations() const;
    void printNetwork() const;
    void printMenu() const;
    void clearScreen() const;
    void pause() const;
    void resetCounters();
    int createPipeWithDiameter(int diameter);
    int createPipeWithAllowedDiameter();

public:
    void addPipe();
    void addStation();
    void viewAll() const;
    void editPipe();
    void editStation();
    void deleteObject();
    void findObjects();
    void batchEdit();
    void connectStations();
    void disconnectStations();
    void viewNetwork() const;
    void topologicalSort();
    void saveData();
    void loadData();
    void calculateMaxFlow();
    void findShortestPath();
    
    void run();
};

#endif