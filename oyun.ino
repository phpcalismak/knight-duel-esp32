#include <TFT_eSPI.h>
#include <SPI.h>

// Include all custom header files
#include "game_config.h"
#include "player.h"
#include "display_manager.h"
#include "game_logic.h"
#include "audio_manager.h"
#include "joystick_manager.h"

// External declarations for global variables (defined in game_config.h, game_logic.h, etc.)
extern TFT_eSPI tft;
extern TFT_eSprite player1Sprite;
extern TFT_eSprite player2Sprite;
extern TFT_eSprite bgSprite;
extern TFT_eSprite uiSprite;

extern Player player1;
extern Player player2;

extern VisualEffect slashEffect;

extern unsigned long lastFrameTime;
extern GameState currentState;
extern int winnerPlayer;
extern unsigned long stateChangeTime;
extern unsigned long debugUpdateTime;

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0)); // Analog bir pinden okunan gürültü ile randomSeed başlat
 
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  initSprites(); // From display_manager.cpp

  // Setup input and output pins
  pinMode(JOY1_BTN_PIN, INPUT_PULLUP);
  pinMode(JOY2_BTN_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  calibrateJoysticks(); // From joystick_manager.cpp

  // Draw title screen and set initial game state
  drawTitleScreen(); // From display_manager.cpp
  stateChangeTime = millis();
}

void loop() {
  unsigned long currentTime = millis();

  // Handle different game states
  switch(currentState) {
    case TITLE_SCREEN:
      if (currentTime - stateChangeTime >= TITLE_SCREEN_DELAY ||
        isAttackPressed(JOY1_BTN_PIN) || isAttackPressed(JOY2_BTN_PIN)) {
        tft.fillScreen(TFT_BLACK);
        currentState = PLAYING;
        resetGame(); // From game_logic.cpp
        
      }
      break;

    case PLAYING:
      if (currentTime - lastFrameTime >= FRAME_INTERVAL) {
        lastFrameTime = currentTime;
 // Rastgele can noktası oluşturma (her X milisaniyede bir şans)
        // Ya da her frame'de bir şans:
        if (random(0, 1000) < HEALTH_PICKUP_SPAWN_CHANCE) { // Örneğin 1000'de HEALTH_PICKUP_SPAWN_CHANCE şans
            spawnHealthPickUp(); // game_logic.cpp'den
        }
        // Update game logic
        handleInput(player1, JOY1_X_PIN, JOY1_Y_PIN, JOY1_BTN_PIN); // From player.cpp
        handleInput(player2, JOY2_X_PIN, JOY2_Y_PIN, JOY2_BTN_PIN); // From player.cpp

        updatePlayer(player1); // From player.cpp
        updatePlayer(player2); // From player.cpp

        handleAttack(player1, player2); // From game_logic.cpp
        handleAttack(player2, player1); // From game_logic.cpp
  
        updateHealthPickUp(); // Yeni: Can veren noktayı güncelle
        updateVisualEffects(); // From game_logic.cpp

        // Check for game over condition
        if (player1.health <= 0 || player2.health <= 0) {
          currentState = GAME_OVER;
          winnerPlayer = (player1.health <= 0) ? 2 : 1;
          stateChangeTime = currentTime;
          playSound(3); // Victory sound - From audio_manager.cpp
        }

        drawGame(); // From display_manager.cpp
      }
      break;

    case GAME_OVER:
      if (currentTime - stateChangeTime < GAME_OVER_DELAY) {
        drawGameOverScreen(winnerPlayer); // From display_manager.cpp
      } else {
        tft.fillScreen(TFT_BLACK);
        currentState = TITLE_SCREEN;
        stateChangeTime = currentTime;
        drawTitleScreen(); // From display_manager.cpp
      }
      break;
  }

  // Print debug info at a controlled rate
  if (currentTime - debugUpdateTime >= DEBUG_INTERVAL) {
    debugUpdateTime = currentTime;
    printDebugInfo(); // From joystick_manager.cpp (or a general debug file)
  }
}