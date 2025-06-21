#ifndef JOYSTICK_MANAGER_H
#define JOYSTICK_MANAGER_H

#include <Arduino.h>
#include "game_config.h" 


extern int joy1XCenter;
extern int joy1YCenter;
extern int joy2XCenter;
extern int joy2YCenter;


void calibrateJoysticks();
void printDebugInfo();
bool isAttackPressed(int btnPin); 

#endif // JOYSTICK_MANAGER_H
