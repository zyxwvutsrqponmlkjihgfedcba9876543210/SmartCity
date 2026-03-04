#include "obstacle.h"
#include <Arduino.h>

void ProximitySensor::init(){
    prox.initThreeSensors();
}

void ProximitySensor::read(){
    prox.read();
    left = prox.countsLeftWithLeftLeds();
    frontLeft = prox.countsFrontWithLeftLeds();
    frontRight = prox.countsFrontWithRightLeds();
}

bool ProximitySensor::isObstacleAhead(){
    return (frontLeft > OBSTACLE_THRESHOLD || frontRight > OBSTACLE_THRESHOLD);
}

bool ProximitySensor::isZumoOnLeft(){
    return (left > INTERSECTION_THRESHOLD);
}

bool ProximitySensor::hasRightOfWay(){
    return !isZumoOnLeft();
}

void ProximitySensor::printDebug(){
    Serial.print("L: ");
    Serial.print(left);
    Serial.print("FL: ");
    Serial.print(frontLeft);
    Serial.print("FR: ");
    Serial.println(frontRight);
}