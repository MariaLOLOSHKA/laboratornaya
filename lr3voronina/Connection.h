#ifndef CONNECTION_H
#define CONNECTION_H

struct Connection {
    int pipeId;
    int startStationId;
    int endStationId;
    int diameter;
    
    Connection(int pipeId, int startId, int endId, int diam) 
        : pipeId(pipeId), startStationId(startId), endStationId(endId), diameter(diam) {}
};

#endif