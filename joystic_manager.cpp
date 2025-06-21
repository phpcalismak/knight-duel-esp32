#include "joystick_manager.h"
#include "player.h"


int joy1XCenter = 2048;
int joy1YCenter = 2048;
int joy2XCenter = 2048;
int joy2YCenter = 2048;

void calibrateJoysticks() {
  joy1XCenter = 0;
  joy1YCenter = 0;
  joy2XCenter = 0;
  joy2YCenter = 0;

  for (int i = 0; i < 10; i++) {
    joy1XCenter += analogRead(JOY1_X_PIN);
    joy1YCenter += analogRead(JOY1_Y_PIN);
    joy2XCenter += analogRead(JOY2_X_PIN);
    joy2YCenter += analogRead(JOY2_Y_PIN);
    delay(10);
  }

  joy1XCenter /= 10;
  joy1YCenter /= 10;
  joy2XCenter /= 10;
  joy2YCenter /= 10;

  Serial.println("Joystick calibration complete:");
  Serial.print("Joy1 center: X="); Serial.print(joy1XCenter); Serial.print(", Y="); Serial.println(joy1YCenter);
  Serial.print("Joy2 center: X="); Serial.print(joy2XCenter); Serial.print(", Y="); Serial.println(joy2YCenter);
}

void printDebugInfo() {
  Serial.print("P1: ");
  Serial.print("Pos("); Serial.print(player1.x); Serial.print(","); Serial.print(player1.y);
  Serial.print(") Vel("); Serial.print(player1.velocityX); Serial.print(","); Serial.print(player1.velocityY);
  Serial.print(") Health:"); Serial.println(player1.health);

  Serial.print("P2: ");
  Serial.print("Pos("); Serial.print(player2.x); Serial.print(","); Serial.print(player2.y);
  Serial.print(") Vel("); Serial.print(player2.velocityX); Serial.print(","); Serial.print(player2.velocityY);
  Serial.print(") Health:"); Serial.println(player2.health);

  Serial.print("Joystick1 raw: "); Serial.print(analogRead(JOY1_X_PIN)); Serial.print(","); Serial.print(analogRead(JOY1_Y_PIN));
  Serial.print(" | Joystick2 raw: "); Serial.print(analogRead(JOY2_X_PIN)); Serial.print(","); Serial.println(analogRead(JOY2_Y_PIN));
}

// Function to check if attack button is pressed
bool isAttackPressed(int btnPin) { // <-- ADD THIS FUNCTION
  return digitalRead(btnPin) == LOW;
}
