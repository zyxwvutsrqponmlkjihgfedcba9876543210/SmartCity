#ifndef LINE_FOLLOW_H
#define LINE_FOLLOW_H
#include <Zumo32U4.h>
#include "globals.h"

class ProximitySensor;
void linefollow();
void crossroads(ProximitySensor &prox);
bool handleIntersectionWithPriority(ProximitySensor &prox);
void turnRight();
void turnLeft();

#endif
