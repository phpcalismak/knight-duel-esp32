#ifndef PLAYER_H
#define PLAYER_H

#include <Arduino.h> // For types like bool, unsigned long
#include "game_config.h" // For GameState, constants, etc.

// Player struct
struct Player {
  float x, y;
  float velocityX, velocityY;
  bool facingRight;
  bool attacking;
  bool jumping;
  bool crouching;
  int health;
  int animFrame; // Current animation frame index
  unsigned long lastAttackTime;
  unsigned long lastAnimUpdateTime;
  bool attackButtonHeld;
  unsigned long attackButtonPressTime;
  int attackPower;
   // Eski çizim pozisyonlarını saklamak için eklenenler
  float prevX, prevY;
  int prevWidth, prevHeight;
  const uint16_t* prevSprite; // Önceki sprite'ı hatırlamak için (opsiyonel)
};

// External declarations for player objects
extern Player player1;
extern Player player2;

// Function prototypes
void updatePlayer(Player &p);
void handleInput(Player &p, int joyXPin, int joyYPin, int btnPin);

#endif // PLAYER_H