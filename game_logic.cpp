#include "game_logic.h"
#include "audio_manager.h"
#include "display_manager.h" 
#include "game_config.h" 

VisualEffect slashEffect = {false, 0, 0, true, 1, 0, 200};

GameState currentState = TITLE_SCREEN; 
int winnerPlayer = 0; 
unsigned long stateChangeTime = 0; 
unsigned long lastFrameTime = 0; 
unsigned long debugUpdateTime = 0; 
// ----------------------------------------------------------
HealthPickUp healthPickUp = {false, 0, 0, 0, TFT_GREEN, 0, 0, 0}; 

// Hitbox [x, y, width, height] - relative to player's (x,y)
int standingHitbox[] = {-10, 2, 20, 30};
int crouchingHitbox[] = {-10, 12, 20, 20};

float dist(float x1, float y1, float x2, float y2) {
  return sqrt(sq(x2 - x1) + sq(y2 - y1));
}

void spawnHealthPickUp() {
    if (!healthPickUp.active) { 
        healthPickUp.active = true;
        healthPickUp.x = random(10, SCREEN_WIDTH - 10); 
        healthPickUp.y = 0; 
        healthPickUp.size = 5; 
        healthPickUp.color = TFT_RED;
        
        // Önceki konum bilgilerini sıfırla
        healthPickUp.prevX = 0;
        healthPickUp.prevY = 0;
        healthPickUp.prevSize = 0;
    }
}

void updateHealthPickUp() {
    if (healthPickUp.active) {
        healthPickUp.y += HEALTH_PICKUP_FALL_SPEED; 

        if (healthPickUp.y >= GROUND_LEVEL + healthPickUp.size) { 
            healthPickUp.active = false;
        }

       
        float player1CenterX = player1.x;
        float player1CenterY = player1.y - (KNIGHT_PANDA_HEIGHT / 2.0); 

        if (dist(healthPickUp.x, healthPickUp.y, player1CenterX, player1CenterY) < (healthPickUp.size / 2.0) + (KNIGHT_PANDA_HEIGHT / 2.0)) { 
            if (healthPickUp.active) {
                player1.health = min(100, player1.health + HEALTH_PICKUP_AMOUNT); 
                healthPickUp.active = false; 
                playSound(1); 
            }
        }
        
        float player2CenterX = player2.x;
        float player2CenterY = player2.y - (KNIGHT_PANDA_HEIGHT / 2.0);

        if (dist(healthPickUp.x, healthPickUp.y, player2CenterX, player2CenterY) < (healthPickUp.size / 2.0) + (KNIGHT_PANDA_HEIGHT / 2.0)) {
            if (healthPickUp.active) {
                player2.health = min(100, player2.health + HEALTH_PICKUP_AMOUNT);
                healthPickUp.active = false;
                playSound(1);
            }
        }
    }
}


void resetGame() {
  player1 = {100, GROUND_LEVEL, 0, 0, true, false, false, false, 100, 0, 0, 0, false, 0, 1};
  player2 = {220, GROUND_LEVEL, 0, 0, false, false, false, false, 100, 0, 0, 0, false, 0, 1};
  slashEffect.active = false;
    healthPickUp.active = false;
    healthPickUp.x = 0;
    healthPickUp.y = 0;
    healthPickUp.prevX = 0;
    healthPickUp.prevY = 0;
    healthPickUp.prevSize = 0;
}

void updateVisualEffects() {
  if (slashEffect.active) {
    if (millis() - slashEffect.startTime > slashEffect.duration) {
      slashEffect.active = false;
    }
  }
}

void handleAttack(Player &attacker, Player &defender) {
  if (!attacker.attacking) return;

  int attackX = attacker.facingRight ? attacker.x + 15 : attacker.x - 15;
  int attackY = attacker.y + 10;

  int* defenderHitbox = defender.crouching ? crouchingHitbox : standingHitbox;

  if (checkCollision(attackX - 10, attackY - 5, 20, 10,
                     defender.x + defenderHitbox[0],
                     defender.y + defenderHitbox[1],
                     defenderHitbox[2],
                     defenderHitbox[3])) {

    int damage = 5 * attacker.attackPower;

    defender.health -= damage;
    defender.health = constrain(defender.health, 0, 100);

    float knockbackForce = 3.0 * attacker.attackPower;
    defender.velocityX += attacker.facingRight ? knockbackForce : -knockbackForce;

    if (defender.y >= GROUND_LEVEL) {
      defender.velocityY = -2.0;
    }

    slashEffect.active = true;
    slashEffect.x = defender.x;
    slashEffect.y = defender.y;
    slashEffect.facingRight = attacker.facingRight;
    slashEffect.power = attacker.attackPower;
    slashEffect.startTime = millis();

    playSound(2);
  }
}

bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
  return (x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2);
}
