#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <Arduino.h>
#include "player.h"
#include "game_config.h"

struct VisualEffect {
  bool active;
  int x, y;
  bool facingRight;
  int power;
  unsigned long startTime;
  unsigned long duration;
};

float dist(float x1, float y1, float x2, float y2); // <<< ADD THIS LINE


struct HealthPickUp {
    bool active; 
    float x, y;  
    int size;    
    uint16_t color; 
    
    
    float prevX, prevY;
    int prevSize;
};


extern VisualEffect slashEffect;

extern HealthPickUp healthPickUp;

extern GameState currentState;
extern int winnerPlayer;
extern unsigned long stateChangeTime;
extern unsigned long lastFrameTime; 
extern unsigned long debugUpdateTime; 
// -------------------------------------------------

extern int standingHitbox[];
extern int crouchingHitbox[];

void resetGame();
void updateVisualEffects();
void handleAttack(Player &attacker, Player &defender);
void spawnHealthPickUp();
void updateHealthPickUp();
void drawHealthPickUp();
bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

#endif // GAME_LOGIC_H
