#include "line_follow.h"
#include "battery.h"
#include "globals.h"
#include "obstacle.h"
#include "velocity_control.h"

// State machine variables
static bool turning = false;
static unsigned long crossStartTime = 0;
static bool crossDetected = false;
static int state = 0;

// Intersection handling constants
static const unsigned long INTERSECTION_CROSS_TIME_MS = 300;
static const unsigned long YIELD_TIMEOUT_MS = 3000;
static const unsigned long INTERSECTION_READ_DELAY_MS = 50;

void linefollow() {
    int pos = lineSensors.readLine(sensorValues);
    int error = pos - 2000;

    // P-controller for line position - outputs target velocity
    float Kp = 0.1f;
    float correction = error * Kp;

    float leftTarget  = baseSpeed + correction;
    float rightTarget = baseSpeed - correction;
    setTargetVelocity(leftTarget, rightTarget);
}

bool handleIntersectionWithPriority(ProximitySensor &prox) {
    setTargetVelocity(0.0f, 0.0f);
    
    prox.read();
    if (prox.hasRightOfWay()) {
        setTargetVelocity(baseSpeed, baseSpeed);
        delay(INTERSECTION_CROSS_TIME_MS);
        return true;
    }
    
    unsigned long waitStart = millis();
    while (!prox.hasRightOfWay()) {
        prox.read();
        
        if (millis() - waitStart > YIELD_TIMEOUT_MS) {
            break;
        }
        
        delay(INTERSECTION_READ_DELAY_MS);
    }
    
    setTargetVelocity(baseSpeed, baseSpeed);
    delay(INTERSECTION_CROSS_TIME_MS);
    return true;
}

void crossroads(ProximitySensor &prox) {
    lineSensors.read(sensorValues);

    if (turning) {
        linefollow();
        return;
    }

    // Decode sensor readings
    bool leftOuter  = sensorValues[0] > threshold;
    bool rightOuter = sensorValues[4] > threshold;
    bool center     = sensorValues[2] > threshold;
    bool leftCross  = (leftOuter && center);
    bool rightCross = (rightOuter && center);
    bool isCross    = (leftCross && rightCross);

    // State 0: line following and intersection handling
    if (state == 0) {
        if (battery_cap < 25.0f) {
            state = 1;
            return;
        }

        if (isCross) {
            handleIntersectionWithPriority(prox);
            return;
        }

        if (leftCross || rightCross) {
            setTargetVelocity(baseSpeed, baseSpeed);
            delay(200);
            return;
        }
        // Normal line following
        linefollow();
        return;
    }

    // State 1: Navigate to charging station (turn at first intersection)
    if (state == 1) {
        if (leftCross) {
            setTargetVelocity(0.0f, 0.0f);
            delay(100);
            setTargetVelocity(-50.0f, 200.0f);  // Turn left
            delay(500);
            state = 2;
            return;
        }

        if (rightCross) {
            setTargetVelocity(0.0f, 0.0f);
            delay(100);
            setTargetVelocity(200.0f, -50.0f);  // Turn right
            delay(500);
            state = 2;
            return;
        }
        
        linefollow();
        return;
    }

    // State 2: At charging station area
    if (state == 2) {
        if (leftCross) {
            setTargetVelocity(0.0f, 0.0f);
            delay(100);
            setTargetVelocity(-50.0f, 200.0f);  // Turn left
            delay(500);
            setTargetVelocity(100.0f, 100.0f);  // Forward slightly
            delay(200);
            stopCarIfEmpty();
            state = 3;
            return;
        }
        
        if (rightCross) {
            setTargetVelocity(0.0f, 0.0f);
            delay(100);
            setTargetVelocity(200.0f, -50.0f);  // Turn right
            delay(500);
            setTargetVelocity(100.0f, 100.0f);  // Forward slightly
            delay(200);
            stopCarIfEmpty();
            state = 3;
            return;
        }

        linefollow();
        return;
    }

    // State 3: Returning from charging
    if (state == 3) {
        if (isCross) {
            setTargetVelocity(0.0f, 0.0f);
            delay(100);
            setTargetVelocity(200.0f, -50.0f); 
            delay(700);
            state = 0;
            return;
        }
        linefollow();
        return;
    }
}

void turnRight() {
    setTargetVelocity(0.0f, 0.0f);
    delay(100);
    setTargetVelocity(200.0f, -50.0f);
    delay(500);
    turning = false;
}

void turnLeft() {
    setTargetVelocity(0.0f, 0.0f);
    delay(100);
    setTargetVelocity(-50.0f, 200.0f);
    delay(500);
    turning = false;
}
