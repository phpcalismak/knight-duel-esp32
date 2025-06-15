#include "player.h"
#include "audio_manager.h" // For playSound()
#include "joystick_manager.h" // For joystick calibration values

// Define players (global, as they are accessed by main.ino and other modules)
Player player1 = {100, GROUND_LEVEL, 0, 0, true, false, false, false, 100, 0, 0, 0, false, 0, 1};
Player player2 = {220, GROUND_LEVEL, 0, 0, false, false, false, false, 100, 0, 0, 0, false, 0, 1};

// External declarations for joystick center values
extern int joy1XCenter;
extern int joy1YCenter;
extern int joy2XCenter;
extern int joy2YCenter;

void updatePlayer(Player &p) {
  // Apply physics
  p.x += p.velocityX;

  if (p.y < GROUND_LEVEL) {
    p.velocityY += GRAVITY;
  }
  p.y += p.velocityY;

  if (p.y > GROUND_LEVEL) {
    p.y = GROUND_LEVEL;
    p.velocityY = 0;
    p.jumping = false;
  }

  // Keep player within screen bounds
  p.x = constrain(p.x, 20, SCREEN_WIDTH - 20); // Adjust player X bounds

  // Handle animation updates based on movement
  if (abs(p.velocityX) > 0 && millis() - p.lastAnimUpdateTime > ANIM_SPEED) {
    p.animFrame = (p.animFrame + 1) % 2;  // Toggle between 0 and 1 for simple walk
    p.lastAnimUpdateTime = millis();
  } else if (p.velocityX == 0 && !p.jumping && !p.crouching) {
    p.animFrame = 0;  // Standing still animation frame
  }

  // Apply friction
  if (p.velocityX > 0) {
    p.velocityX -= FRICTION;
    if (p.velocityX < STOP_THRESHOLD) p.velocityX = 0;
  } else if (p.velocityX < 0) {
    p.velocityX += FRICTION;
    if (p.velocityX > -STOP_THRESHOLD) p.velocityX = 0;
  }
}

void handleInput(Player &p, int joyXPin, int joyYPin, int btnPin) {
  bool isPlayer1 = (joyXPin == JOY1_X_PIN);
  int centerX = isPlayer1 ? joy1XCenter : joy2XCenter;
  int centerY = isPlayer1 ? joy1YCenter : joy2YCenter;

  int rawX = analogRead(joyXPin);
  int rawY = analogRead(joyYPin);

  int moveX = 0;
  if (abs(rawX - centerX) > JOYSTICK_DEADZONE) {
    moveX = (rawX > centerX) ? 1 : -1;
  }

  if (moveX != 0) {
    p.velocityX += moveX * ACCELERATION;
    p.facingRight = moveX > 0;
  } else {
    if (p.velocityX > 0) {
      p.velocityX -= DECELERATION;
      if (p.velocityX < STOP_THRESHOLD) p.velocityX = 0;
    } else if (p.velocityX < 0) {
      p.velocityX += DECELERATION;
      if (p.velocityX > -STOP_THRESHOLD) p.velocityX = 0;
    }
  }

  p.velocityX = constrain(p.velocityX, -MAX_SPEED, MAX_SPEED);

  int moveY = 0;
  if (abs(rawY - centerY) > JOYSTICK_DEADZONE) {
    moveY = (rawY > centerY) ? 1 : -1;
  }

  if (moveY == -1 && !p.jumping && p.y >= GROUND_LEVEL) {
    p.jumping = true;
    p.velocityY = JUMP_FORCE;
    playSound(1); // Jump sound
  }

  p.crouching = (moveY == 1 && !p.jumping);

  bool attackPressed = (digitalRead(btnPin) == LOW);

  if (attackPressed && !p.attackButtonHeld) {
    p.attackButtonHeld = true;
    p.attackButtonPressTime = millis();
  }

  if (!attackPressed && p.attackButtonHeld) {
    p.attackButtonHeld = false;

    if (millis() - p.lastAttackTime > ATTACK_COOLDOWN) {
      unsigned long heldDuration = millis() - p.attackButtonPressTime;

      if (heldDuration > SUPER_ATTACK_THRESHOLD) {
        p.attackPower = 3;
      } else if (heldDuration > STRONG_ATTACK_THRESHOLD) {
        p.attackPower = 2;
      } else {
        p.attackPower = 1;
      }

      p.attacking = true;
      p.lastAttackTime = millis();
    }
  }

  if (p.attacking && millis() - p.lastAttackTime > 150) {
    p.attacking = false;
  }
}