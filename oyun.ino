#include <TFT_eSPI.h>
#include <SPI.h>

#include "game_config.h"
#include "player.h"
#include "display_manager.h"
#include "game_logic.h"
#include "audio_manager.h"
#include "joystick_manager.h"

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
  randomSeed(analogRead(0)); 
 
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  initSprites(); 

  pinMode(JOY1_BTN_PIN, INPUT_PULLUP);
  pinMode(JOY2_BTN_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  calibrateJoysticks(); 

  drawTitleScreen(); 
  stateChangeTime = millis();
}

void loop() {
  unsigned long currentTime = millis();

  switch(currentState) {
    case TITLE_SCREEN:
      if (currentTime - stateChangeTime >= TITLE_SCREEN_DELAY ||
        isAttackPressed(JOY1_BTN_PIN) || isAttackPressed(JOY2_BTN_PIN)) {
        tft.fillScreen(TFT_BLACK);
        currentState = PLAYING;
        resetGame(); 
        
      }
      break;

    case PLAYING:
      if (currentTime - lastFrameTime >= FRAME_INTERVAL) {
        lastFrameTime = currentTime;
        if (random(0, 1000) < HEALTH_PICKUP_SPAWN_CHANCE) { 
            spawnHealthPickUp(); 
        }
        // Update game logic
        handleInput(player1, JOY1_X_PIN, JOY1_Y_PIN, JOY1_BTN_PIN);
        handleInput(player2, JOY2_X_PIN, JOY2_Y_PIN, JOY2_BTN_PIN); 

        updatePlayer(player1);
        updatePlayer(player2); 

        handleAttack(player1, player2); 
        handleAttack(player2, player1); 
  
        updateHealthPickUp(); 
        updateVisualEffects(); 

        if (player1.health <= 0 || player2.health <= 0) {
          currentState = GAME_OVER;
          winnerPlayer = (player1.health <= 0) ? 2 : 1;
          stateChangeTime = currentTime;
          playSound(3); // Victory sound - From audio_manager.cpp
        }

        drawGame();
      }
      break;

    case GAME_OVER:
      if (currentTime - stateChangeTime < GAME_OVER_DELAY) {
        drawGameOverScreen(winnerPlayer); 
      } else {
        tft.fillScreen(TFT_BLACK);
        currentState = TITLE_SCREEN;
        stateChangeTime = currentTime;
        drawTitleScreen(); 
      }
      break;
  }

  if (currentTime - debugUpdateTime >= DEBUG_INTERVAL) {
    debugUpdateTime = currentTime;
    printDebugInfo(); 
  }
}
