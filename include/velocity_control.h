#ifndef VELOCITY_CONTROL_H
#define VELOCITY_CONTROL_H
void setupVelocityControl();
void velocityControlISR();
void setTargetVelocity(float left, float right);
#endif
