#ifndef OBSTACLE_H
#define OBSTACLE_H
#include <Zumo32U4.h>

class ProximitySensor {
public:
    void init();
    void read();
    bool isObstacleAhead(); // front-left or front-right
    bool isZumoOnLeft(); // right hand rule
    bool hasRightOfWay();
    void printDebug();
private:
    Zumo32U4ProximitySensors prox;
    uint8_t frontLeft, frontRight, left;
    static const uint8_t OBSTACLE_THRESHOLD = 4;
    static const uint8_t INTERSECTION_THRESHOLD = 2;
};


#endif
