// globals.cpp
// Single definitions for globals declared as extern in headers.

#include "line_follow.h"
#include "display.h"

// Definitions
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;
unsigned int sensorValues[5];
Zumo32U4OLED oled;
float battery_cap=100;
const float power=0.05; // kan justeres for hvor fort batteriet skal tømmes
uint16_t threshold = 500;
bool isCharging = false;
unsigned long chargeStartTime = 0;
int baseSpeed = 200; // PWM value, but update for cm/s later (about half of max speed 40cm/s)
volatile float targetVelocityLeft = 0;
volatile float targetVelocityRight = 0;
volatile float actualVelocityLeft = 0;
volatile float actualVelocityRight = 0;
Zumo32U4Encoders encoders;