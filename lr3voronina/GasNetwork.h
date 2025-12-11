#ifndef GASNETWORK_H
#define GASNETWORK_H

#include <vector>
#include <map>
#include <set>
#include <queue>
#include "Connection.h"

class GasNetwork {
private:
    std::vector<Connection> connections;
    std::map<int, std::vector<int>> adjacencyList; // stationId -> list of connection indices
    
public:
    GasNetwork();
    
    void addConnection(int pipeId, int startStationId, int endStationId, int diameter);
    bool hasConnection(int startStationId, int endStationId) const;
    std::vector<int> getConnectionsByDiameter(int diameter) const;
    std::vector<Connection> getAllConnections() const;
    void removeConnection(int pipeId);
    
    // Topological sort
    std::vector<int> topologicalSort() const;
    bool hasCycle() const;
    
    void clear();
    bool isEmpty() const;
    
private:
    bool hasCycleDFS(int station, std::set<int>& visited, std::set<int>& recursionStack) const;
};

#endif