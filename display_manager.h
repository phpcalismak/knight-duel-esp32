#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>   
#include <TFT_eSPI.h>  

#include "game_logic.h"    
#include "game_config.h"   
#define TRANSPARENT_COLOR TFT_BLACK
#include "knight.h" 

extern TFT_eSPI tft;
extern TFT_eSprite player1Sprite;
extern TFT_eSprite player2Sprite;
extern TFT_eSprite bgSprite;
extern TFT_eSprite uiSprite;
extern TFT_eSprite groundSprite; 

#define KNIGHT_PANDA_WIDTH   18  
#define KNIGHT_PANDA_HEIGHT  29 

#define KNIGHT_ATTACK_WIDTH  60  
#define KNIGHT_ATTACK_HEIGHT 38 
const int GROUND_SPRITE_WIDTH = 320; 
const int GROUND_SPRITE_HEIGHT = 30; 


void initSprites();
void drawCharacterSprite(Player& p, const uint16_t* spriteData, int spriteWidth, int spriteHeight);
void drawTitleScreen();
void drawGameOverScreen(int winner);
void updateHealthBars();
void drawSlashEffect(int x, int y, bool facingRight, int power);
void drawGroundSprite();
void drawGame();

#endif 
