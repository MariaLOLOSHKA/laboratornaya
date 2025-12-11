#include "GasNetwork.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>

GasNetwork::GasNetwork() {}

void GasNetwork::addConnection(int pipeId, int startStationId, int endStationId, int diameter) {
    // Проверяем, нет ли уже такого соединения
    for (const auto& conn : connections) {
        if (conn.startStationId == startStationId && conn.endStationId == endStationId) {
            throw std::runtime_error("Соединение между этими КС уже существует");
        }
    }
    
    connections.emplace_back(pipeId, startStationId, endStationId, diameter);
    int connectionIndex = connections.size() - 1;
    adjacencyList[startStationId].push_back(connectionIndex);
}

bool GasNetwork::hasConnection(int startStationId, int endStationId) const {
    for (const auto& conn : connections) {
        if (conn.startStationId == startStationId && conn.endStationId == endStationId) {
            return true;
        }
    }
    return false;
}

std::vector<int> GasNetwork::getConnectionsByDiameter(int diameter) const {
    std::vector<int> result;
    for (const auto& conn : connections) {
        if (conn.diameter == diameter) {
            result.push_back(conn.pipeId);
        }
    }
    return result;
}

std::vector<Connection> GasNetwork::getAllConnections() const {
    return connections;
}

void GasNetwork::removeConnection(int pipeId) {
    for (auto it = connections.begin(); it != connections.end(); ++it) {
        if (it->pipeId == pipeId) {
            // Удаляем из adjacencyList
            int startId = it->startStationId;
            auto& adjList = adjacencyList[startId];
            adjList.erase(std::remove_if(adjList.begin(), adjList.end(),
                [&](int idx) { return connections[idx].pipeId == pipeId; }), adjList.end());
            
            connections.erase(it);
            return;
        }
    }
}

std::vector<int> GasNetwork::topologicalSort() const {
    std::vector<int> result;
    
    if (adjacencyList.empty()) {
        return result;
    }
    
    // Вычисляем входящие степени
    std::map<int, int> inDegree;
    for (const auto& [stationId, connIndices] : adjacencyList) {
        inDegree[stationId]; // Инициализируем
        for (int connIndex : connIndices) {
            int endStation = connections[connIndex].endStationId;
            inDegree[endStation]++;
        }
    }
    
    // Находим вершины с нулевой входящей степенью
    std::queue<int> zeroInDegree;
    for (const auto& [stationId, degree] : inDegree) {
        if (degree == 0) {
            zeroInDegree.push(stationId);
        }
    }
    
    // Алгоритм Kahn
    while (!zeroInDegree.empty()) {
        int station = zeroInDegree.front();
        zeroInDegree.pop();
        result.push_back(station);
        
        if (adjacencyList.find(station) != adjacencyList.end()) {
            for (int connIndex : adjacencyList.at(station)) {
                int endStation = connections[connIndex].endStationId;
                inDegree[endStation]--;
                if (inDegree[endStation] == 0) {
                    zeroInDegree.push(endStation);
                }
            }
        }
    }
    
    // Проверяем, есть ли цикл
    if (result.size() != inDegree.size()) {
        throw std::runtime_error("Граф содержит циклы, топологическая сортировка невозможна");
    }
    
    return result;
}

bool GasNetwork::hasCycle() const {
    std::set<int> visited;
    std::set<int> recursionStack;
    
    for (const auto& [stationId, _] : adjacencyList) {
        if (visited.find(stationId) == visited.end()) {
            if (hasCycleDFS(stationId, visited, recursionStack)) {
                return true;
            }
        }
    }
    return false;
}

bool GasNetwork::hasCycleDFS(int station, std::set<int>& visited, std::set<int>& recursionStack) const {
    visited.insert(station);
    recursionStack.insert(station);
    
    if (adjacencyList.find(station) != adjacencyList.end()) {
        for (int connIndex : adjacencyList.at(station)) {
            int neighbor = connections[connIndex].endStationId;
            
            if (visited.find(neighbor) == visited.end()) {
                if (hasCycleDFS(neighbor, visited, recursionStack)) {
                    return true;
                }
            } else if (recursionStack.find(neighbor) != recursionStack.end()) {
                return true;
            }
        }
    }
    
    recursionStack.erase(station);
    return false;
}

void GasNetwork::clear() {
    connections.clear();
    adjacencyList.clear();
}

bool GasNetwork::isEmpty() const {
    return connections.empty();
}