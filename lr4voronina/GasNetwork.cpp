#include "GasNetwork.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <queue>
#include <functional>
#include <map>
#include <limits>
#include <cmath>

GasNetwork::GasNetwork() {}

void GasNetwork::setPipePointers(const std::map<int, Pipe>& pipes) {
    pipePointers.clear();
    for (const auto& [id, pipe] : pipes) {
        pipePointers[id] = const_cast<Pipe*>(&pipe);
    }
}

void GasNetwork::addConnection(int pipeId, int startStationId, int endStationId, int diameter) {
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

// ДОБАВЛЯЕМ РЕАЛИЗАЦИЮ getAllConnections
std::vector<Connection> GasNetwork::getAllConnections() const {
    return connections;
}

void GasNetwork::removeConnection(int pipeId) {
    for (auto it = connections.begin(); it != connections.end(); ++it) {
        if (it->pipeId == pipeId) {
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
    
    std::map<int, int> inDegree;
    for (const auto& [stationId, connIndices] : adjacencyList) {
        inDegree[stationId];
        for (int connIndex : connIndices) {
            int endStation = connections[connIndex].endStationId;
            inDegree[endStation]++;
        }
    }
    
    std::queue<int> zeroInDegree;
    for (const auto& [stationId, degree] : inDegree) {
        if (degree == 0) {
            zeroInDegree.push(stationId);
        }
    }
    
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

double GasNetwork::calculateMaxFlow(int source, int sink) {
    if (pipePointers.empty()) {
        throw std::runtime_error("Не установлены ссылки на трубы");
    }
    
    std::map<int, std::map<int, double>> capacity;
    std::map<int, std::vector<int>> graph;
    
    for (const auto& conn : connections) {
        double pipeCapacity = 0.0;
        if (pipePointers.find(conn.pipeId) != pipePointers.end()) {
            pipeCapacity = pipePointers[conn.pipeId]->getCapacity();
        }
        
        capacity[conn.startStationId][conn.endStationId] += pipeCapacity;
        graph[conn.startStationId].push_back(conn.endStationId);
        graph[conn.endStationId].push_back(conn.startStationId);
    }
    
    double maxFlow = 0.0;
    
    while (true) {
        std::map<int, int> parent;
        std::queue<int> q;
        std::set<int> visited;
        
        q.push(source);
        visited.insert(source);
        parent[source] = -1;
        
        bool foundPath = false;
        
        while (!q.empty() && !foundPath) {
            int u = q.front();
            q.pop();
            
            if (graph.find(u) != graph.end()) {
                for (int v : graph[u]) {
                    if (visited.find(v) == visited.end() && capacity[u][v] > 0) {
                        parent[v] = u;
                        visited.insert(v);
                        q.push(v);
                        
                        if (v == sink) {
                            foundPath = true;
                            break;
                        }
                    }
                }
            }
        }
        
        if (!foundPath) {
            break;
        }
        
        double pathFlow = std::numeric_limits<double>::max();
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            pathFlow = std::min(pathFlow, capacity[u][v]);
        }
        
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            capacity[u][v] -= pathFlow;
            capacity[v][u] += pathFlow;
        }
        
        maxFlow += pathFlow;
    }
    
    return maxFlow;
}

std::vector<int> GasNetwork::findShortestPath(int start, int end) {
    std::vector<double> distances = dijkstra(start);
    std::vector<int> path;
    
    if (distances[end] == std::numeric_limits<double>::infinity()) {
        return path;
    }
    
    int current = end;
    while (current != start) {
        path.push_back(current);
        
        int prev = -1;
        for (const auto& conn : connections) {
            if (conn.endStationId == current) {
                double edgeWeight = std::numeric_limits<double>::infinity();
                if (pipePointers.find(conn.pipeId) != pipePointers.end()) {
                    edgeWeight = pipePointers[conn.pipeId]->getWeight();
                }
                
                if (distances[conn.startStationId] + edgeWeight == distances[current]) {
                    prev = conn.startStationId;
                    break;
                }
            }
        }
        
        if (prev == -1) break;
        current = prev;
    }
    
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<double> GasNetwork::dijkstra(int start) {
    std::vector<double> dist(1000, std::numeric_limits<double>::infinity());
    dist[start] = 0.0;
    
    using Pair = std::pair<double, int>;
    std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> pq;
    pq.push({0.0, start});
    
    while (!pq.empty()) {
        double currentDist = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        
        if (currentDist > dist[u]) continue;
        
        if (adjacencyList.find(u) != adjacencyList.end()) {
            for (int connIndex : adjacencyList[u]) {
                const Connection& conn = connections[connIndex];
                int v = conn.endStationId;
                
                double weight = std::numeric_limits<double>::infinity();
                if (pipePointers.find(conn.pipeId) != pipePointers.end()) {
                    weight = pipePointers[conn.pipeId]->getWeight();
                }
                
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }
    }
    
    return dist;
}

void GasNetwork::clear() {
    connections.clear();
    adjacencyList.clear();
    pipePointers.clear();
}

bool GasNetwork::isEmpty() const {
    return connections.empty();
}