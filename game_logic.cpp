#include "game_logic.h"
#include "audio_manager.h"
#include "display_manager.h" // For drawBackground()
#include "game_config.h" // Needed for constant values like GROUND_LEVEL, etc.

// Define game logic variables
VisualEffect slashEffect = {false, 0, 0, true, 1, 0, 200};

GameState currentState = TITLE_SCREEN; // Initialize
int winnerPlayer = 0; // Initialize
unsigned long stateChangeTime = 0; // Initialize
unsigned long lastFrameTime = 0; // Initialize
unsigned long debugUpdateTime = 0; // Initialize
// ----------------------------------------------------------
HealthPickUp healthPickUp = {false, 0, 0, 0, TFT_GREEN, 0, 0, 0}; // Başlangıçta pasif

// Hitbox [x, y, width, height] - relative to player's (x,y)
int standingHitbox[] = {-10, 2, 20, 30};
int crouchingHitbox[] = {-10, 12, 20, 20};

// Implementation of the distance function
float dist(float x1, float y1, float x2, float y2) {
  return sqrt(sq(x2 - x1) + sq(y2 - y1));
}

// Rastgele bir konumda can veren nokta oluşturur
void spawnHealthPickUp() {
    if (!healthPickUp.active) { // Sadece aktif bir nokta yoksa yenisini oluştur
        healthPickUp.active = true;
        healthPickUp.x = random(10, SCREEN_WIDTH - 10); // Ekran genişliği içinde rastgele X
        healthPickUp.y = 0; // Ekranın en üstünden başla
        healthPickUp.size = 5; // Noktanın boyutu (5x5 piksel kare)
        healthPickUp.color = TFT_RED;
        
        // Önceki konum bilgilerini sıfırla
        healthPickUp.prevX = 0;
        healthPickUp.prevY = 0;
        healthPickUp.prevSize = 0;
    }
}

// Can veren noktanın konumunu günceller ve çarpışmayı kontrol eder
void updateHealthPickUp() {
    if (healthPickUp.active) {
        // Noktayı aşağı doğru hareket ettir
        healthPickUp.y += HEALTH_PICKUP_FALL_SPEED; // game_config.h'de tanımlanacak

        // Eğer zemin seviyesine ulaşırsa veya ekranın dışına çıkarsa pasif hale getir
        if (healthPickUp.y >= GROUND_LEVEL + healthPickUp.size) { // Yere ulaştığında veya altından geçtiğinde
            healthPickUp.active = false;
        }

        // Oyuncularla çarpışma kontrolü
        // Player 1 ile çarpışma
        float player1CenterX = player1.x;
        float player1CenterY = player1.y - (KNIGHT_PANDA_HEIGHT / 2.0); // Ortası değil, zeminden yukarısı

        if (dist(healthPickUp.x, healthPickUp.y, player1CenterX, player1CenterY) < (healthPickUp.size / 2.0) + (KNIGHT_PANDA_HEIGHT / 2.0)) { // Basit mesafe kontrolü
            if (healthPickUp.active) {
                player1.health = min(100, player1.health + HEALTH_PICKUP_AMOUNT); // Canı artır, 100'ü geçmesin
                healthPickUp.active = false; // Noktayı pasif hale getir
                playSound(1); // Örneğin bir "toplama" sesi
            }
        }
        
        // Player 2 ile çarpışma (aynı mantık)
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
  // Use GROUND_LEVEL from game_config.h
  player1 = {100, GROUND_LEVEL, 0, 0, true, false, false, false, 100, 0, 0, 0, false, 0, 1};
  player2 = {220, GROUND_LEVEL, 0, 0, false, false, false, false, 100, 0, 0, 0, false, 0, 1};
  slashEffect.active = false;
   // HealthPickUp'ı da sıfırla
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