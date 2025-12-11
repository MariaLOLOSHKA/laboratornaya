#ifndef GASNETWORK_H
#define GASNETWORK_H

#include <vector>
#include <map>
#include <set>
#include <queue>
#include <limits>
#include <algorithm>
#include <functional>
#include "Connection.h"
#include "Pipe.h"

class GasNetwork {
private:
    std::vector<Connection> connections;
    std::map<int, std::vector<int>> adjacencyList;
    std::map<int, Pipe*> pipePointers;

public:
    GasNetwork();
    
    void addConnection(int pipeId, int startStationId, int endStationId, int diameter);
    void setPipePointers(const std::map<int, Pipe>& pipes);
    bool hasConnection(int startStationId, int endStationId) const;
    std::vector<int> getConnectionsByDiameter(int diameter) const;
    std::vector<Connection> getAllConnections() const; // УБИРАЕМ РЕАЛИЗАЦИЮ ЗДЕСЬ
    void removeConnection(int pipeId);
    
    // Топологическая сортировка
    std::vector<int> topologicalSort() const;
    bool hasCycle() const;
    
    // Максимальный поток
    double calculateMaxFlow(int source, int sink);
    
    // Кратчайший путь
    std::vector<int> findShortestPath(int start, int end);
    
    void clear();
    bool isEmpty() const;
    
private:
    bool hasCycleDFS(int station, std::set<int>& visited, std::set<int>& recursionStack) const;
    std::vector<double> dijkstra(int start);
};

#endif