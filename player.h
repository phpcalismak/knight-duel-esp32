#ifndef PLAYER_H
#define PLAYER_H

#include <Arduino.h> 
#include "game_config.h"

struct Player {
  float x, y;
  float velocityX, velocityY;
  bool facingRight;
  bool attacking;
  bool jumping;
  bool crouching;
  int health;
  int animFrame; 
  unsigned long lastAttackTime;
  unsigned long lastAnimUpdateTime;
  bool attackButtonHeld;
  unsigned long attackButtonPressTime;
  int attackPower;
  float prevX, prevY;
  int prevWidth, prevHeight;
  const uint16_t* prevSprite; 
};

extern Player player1;
extern Player player2;

void updatePlayer(Player &p);
void handleInput(Player &p, int joyXPin, int joyYPin, int btnPin);

#endif
