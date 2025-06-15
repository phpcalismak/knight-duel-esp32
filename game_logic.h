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

// Utility function for calculating distance (Euclidean distance)
float dist(float x1, float y1, float x2, float y2); // <<< ADD THIS LINE

// Can veren nokta için struct
struct HealthPickUp {
    bool active; // Noktanın aktif olup olmadığını belirtir
    float x, y;  // Noktanın konumu
    int size;    // Noktanın boyutu (piksel)
    uint16_t color; // Noktanın rengi (kırmızı olacak)
    
    // Temizleme için önceki konum bilgisi
    float prevX, prevY;
    int prevSize;
};

// External declarations for game logic variables
extern VisualEffect slashEffect;

// Global olarak bir can veren nokta nesnesi tanımlayalım
extern HealthPickUp healthPickUp;

// --- ENSURE THESE ARE DECLARED AS EXTERN HERE ---
extern GameState currentState;
extern int winnerPlayer;
extern unsigned long stateChangeTime;
extern unsigned long lastFrameTime; // ADDED IF NOT ALREADY THERE
extern unsigned long debugUpdateTime; // ADDED IF NOT ALREADY THERE
// -------------------------------------------------

extern int standingHitbox[];
extern int crouchingHitbox[];

void resetGame();
void updateVisualEffects();
void handleAttack(Player &attacker, Player &defender);
// Yeni fonksiyon bildirimleri
void spawnHealthPickUp();
void updateHealthPickUp();
void drawHealthPickUp();
bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

#endif // GAME_LOGIC_H