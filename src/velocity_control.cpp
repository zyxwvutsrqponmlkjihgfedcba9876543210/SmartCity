/* All bare metal code
Timer3 setup (memory mapped I/O)
Encoder reading
PID math 
Writing to PWM registers*/
#include "velocity_control.h"
#include "globals.h"
#include <avr/io.h>
#include <avr/interrupt.h>

// PID constants (tune these)
const float KP = 1.0f;
const float KI = 0.0f;
const float KD = 0.0f;

// For now, treat everything as PWM units (0-400) since we can't measure real cm/s
// Later: measure actual speed at PWM 400, then calculate scale factor
const float PWM_PER_COUNT = 1.0f;  // Placeholder - counts directly map to PWM
const float CM_PER_COUNT = 0.055f; // From battery.cpp but measure it, and then replace PWM_PER_COUNT in calculations
// Left motor PID state
static float integralLeft = 0.0f;
static float lastErrorLeft = 0.0f;
static int lastCountLeft = 0;

// Right motor PID state
static float integralRight = 0.0f;
static float lastErrorRight = 0.0f;
static int lastCountRight = 0;

void setupVelocityControl() {
    // Setup direction pins as outputs (bare metal)
    // Pin 16 (Left direction) = PB2
    // Pin 15 (Right direction) = PB1
    DDRB |= (1 << DDB2) | (1 << DDB1);  // Set PB1 and PB2 as outputs
    PORTB &= ~((1 << PB2) | (1 << PB1)); // Both forward initially
    
    // Timer3 CTC mode configuration
    TCCR3A = 0;                          // Clear control register A
    TCCR3B = (1 << WGM32);               // CTC mode (WGM32 = 1)
    TCCR3B |= (1 << CS31) | (1 << CS30); // Prescaler 64 (CS31=1, CS30=1)
    OCR3A = 250;                         // 16MHz/64/250 = 1000Hz = 1kHz
    TIMSK3 |= (1 << OCIE3A);             // Enable compare match A interrupt
    sei();                               // Enable global interrupts
}

void velocityControlISR() {
    // ===== LEFT MOTOR PID =====
    int currentCountLeft = encoders.getCountsLeft();
    int deltaLeft = currentCountLeft - lastCountLeft;
    lastCountLeft = currentCountLeft;
    
    // Store actual velocity in global for debugging
    // For now: treat as PWM units per second
    actualVelocityLeft = deltaLeft * PWM_PER_COUNT * 1000.0f;
    
    float errorLeft = targetVelocityLeft - actualVelocityLeft;
    
    float P_left = KP * errorLeft;
    integralLeft += errorLeft * 0.001f;
    float I_left = KI * integralLeft;
    float D_left = (errorLeft - lastErrorLeft) * 1000.0f * KD;
    lastErrorLeft = errorLeft;
    
    float outputLeft = P_left + I_left + D_left;
    
    // Handle direction BEFORE constraining
    if (outputLeft < 0) {
        PORTB |= (1 << PB2);      // Reverse
        outputLeft = -outputLeft; // Make positive
    } else {
        PORTB &= ~(1 << PB2);     // Forward
    }
    
    // Constrain to valid PWM range
    if (outputLeft > 400.0f) outputLeft = 400.0f;
    if (outputLeft < 0.0f) outputLeft = 0.0f;
    
    // ===== RIGHT MOTOR PID =====
    int currentCountRight = encoders.getCountsRight();
    int deltaRight = currentCountRight - lastCountRight;
    lastCountRight = currentCountRight;
    
    // Store actual velocity in global
    actualVelocityRight = deltaRight * PWM_PER_COUNT * 1000.0f;
    
    float errorRight = targetVelocityRight - actualVelocityRight;
    
    float P_right = KP * errorRight;
    integralRight += errorRight * 0.001f;
    float I_right = KI * integralRight;
    float D_right = (errorRight - lastErrorRight) * 1000.0f * KD;
    lastErrorRight = errorRight;
    
    float outputRight = P_right + I_right + D_right;
    
    // Handle direction BEFORE constraining
    if (outputRight < 0) {
        PORTB |= (1 << PB1);       // Reverse
        outputRight = -outputRight; // Make positive
    } else {
        PORTB &= ~(1 << PB1);      // Forward
    }
    
    // Constrain to valid PWM range
    if (outputRight > 400.0f) outputRight = 400.0f;
    if (outputRight < 0.0f) outputRight = 0.0f;
    
    // ===== BARE METAL: Write to PWM registers =====
    OCR1B = (uint16_t)outputLeft;   // Left motor PWM (Pin 10)
    OCR1A = (uint16_t)outputRight;  // Right motor PWM (Pin 9)
}

ISR(TIMER3_COMPA_vect) {
    velocityControlISR();
}

void setTargetVelocity(float left, float right) {
    targetVelocityLeft = left;
    targetVelocityRight = right;
}
