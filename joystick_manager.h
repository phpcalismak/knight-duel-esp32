#ifndef JOYSTICK_MANAGER_H
#define JOYSTICK_MANAGER_H

#include <Arduino.h>
#include "game_config.h" // For joystick pin definitions

// External declarations for joystick center values
extern int joy1XCenter;
extern int joy1YCenter;
extern int joy2XCenter;
extern int joy2YCenter;

// Function prototypes
void calibrateJoysticks();
void printDebugInfo();
bool isAttackPressed(int btnPin); // <-- ADD THIS LINE

#endif // JOYSTICK_MANAGER_H