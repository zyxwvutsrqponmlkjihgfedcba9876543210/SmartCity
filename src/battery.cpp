#include "battery.h"
#include "globals.h"
#include "velocity_control.h"
const float CM_PER_COUNT = 0.055;
float remaining_distance() {
    const float MAX_RANGE = 50.0; // meters when fully charged
    return (battery_cap / 100.0) * MAX_RANGE;
}

float battery_calculator(float deltaTime) {
    static int lastLeftCount = 0;
    static int lastRightCount = 0;
    int leftCount = encoders.getCountsLeft();
    int rightCount = encoders.getCountsRight();

    int deltaLeft = leftCount - lastLeftCount;
    int deltaRight = rightCount - lastRightCount;

    Serial.print("L: "); Serial.print(leftCount);
    Serial.print(" R: "); Serial.println(rightCount);

    lastLeftCount = leftCount;
    lastRightCount = rightCount;

    float leftDist = abs(deltaLeft * CM_PER_COUNT);
    float rightDist = abs(deltaRight * CM_PER_COUNT);
    float avgDist = (leftDist + rightDist) / 2.0;

    battery_cap -= (avgDist * power/3 /*/4*/); 
    if (battery_cap < 0) battery_cap = 0;
    if (battery_cap > 100) battery_cap = 100;

    return battery_cap; 
}

void checkBatteryState() {
    if (battery_cap <= 0) {
        stopCarIfEmpty(); 
    } 
    else if (battery_cap < 15) {
        Serial.println("Warning: Low battery!");
        Serial.print("Battery level: ");
        Serial.print(battery_cap);
        Serial.println("%");
    } 
    else {
        Serial.print("Battery level: ");
        Serial.print(battery_cap);
        Serial.println("%");
    }
}

void stopCarIfEmpty() {
    setTargetVelocity(0.0f, 0.0f);
    Serial.println("Battery empty! Car stopped.");
    // Sensor to detect charging station, digitalRead IR sensor? bool atChargingStation = digitalRead(pin);
    // Zumo line sensors for our line-following path is black tape, so we could use a special area of white tape to act as a charging station marker.
    bool atChargingStation = true;
    if (atChargingStation) {
        isCharging = true; 
        chargeStartTime = millis();
        chargeBattery();
    } else {
        Serial.println("Drive to the nearest charging station.");
    }
}

void chargeBattery() {
    if (!isCharging) return;
    if (millis() - chargeStartTime >= 100) {
        chargeStartTime = millis();
        if (battery_cap < 100) {
            battery_cap++;
            Serial.print("Ch:"); Serial.print(battery_cap); Serial.println("%");
        } else isCharging = false;
    }
 }