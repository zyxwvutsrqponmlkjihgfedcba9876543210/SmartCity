#include "globals.h"
#include "battery.h"
#include "calibrate.h"
#include "display.h"
#include "line_follow.h"
#include "obstacle.h"
#include "velocity_control.h"

ProximitySensor proximity;

void setup() {
    Serial.begin(9600);
    displayStartup();
  
    // Status LED for proximity reading indicator
    pinMode(13, OUTPUT);
    proximity.init();
    lineSensors.initFiveSensors();
    calibrateLineSensors(lineSensors, motors, 5000);
    setupVelocityControl();
    Serial.println("Calibration complete.");
}

void loop() {
    static unsigned long lastBatteryUpdate = 0;
    unsigned long now = millis();
    
    displayStatus();
    proximity.read();
  
    if (proximity.isObstacleAhead()) {
        // Stop using velocity controller instead of direct motor control
        setTargetVelocity(0.0f, 0.0f);
        Serial.println("[Obstacle] Zumo ahead - stopping");
        
        // Blink LED while blocked
        digitalWrite(13, (now / 250) % 2);
        chargeBattery();
        return;
    }
    
    if (now - lastBatteryUpdate >= 500) {
        float deltaTime = (now - lastBatteryUpdate) / 1000.0f;
        lastBatteryUpdate = now;
        
        if (!isCharging) {
            battery_calculator(deltaTime);
        }
        checkBatteryState();
    }
    
    if (!isCharging) {
        // Line following and intersection handling
        crossroads(proximity);
        
        // Debug output
        int offset = lineSensors.readLine(sensorValues);
        Serial.print("Offset: ");
        Serial.print(offset);
        Serial.print(" | V_L: ");
        Serial.print(actualVelocityLeft);
        Serial.print(" | V_R: ");
        Serial.println(actualVelocityRight);
    }
    
    displayStatus();
}
