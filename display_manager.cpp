#include "display_manager.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite player1Sprite = TFT_eSprite(&tft); 
TFT_eSprite player2Sprite = TFT_eSprite(&tft);
TFT_eSprite bgSprite = TFT_eSprite(&tft);    
TFT_eSprite uiSprite = TFT_eSprite(&tft); 
TFT_eSprite groundSprite = TFT_eSprite(&tft);

void drawHealthPickUp() {
    if (healthPickUp.active) {
        if (healthPickUp.prevX != 0 || healthPickUp.prevY != 0) {
            tft.fillRect(healthPickUp.prevX - healthPickUp.prevSize / 2, 
                         healthPickUp.prevY - healthPickUp.prevSize / 2, 
                         healthPickUp.prevSize, healthPickUp.prevSize, TFT_BLACK);
        }
    }
        
        tft.fillCircle(healthPickUp.x, healthPickUp.y, healthPickUp.size / 2, healthPickUp.color); 
        

        healthPickUp.prevX = healthPickUp.x;
        healthPickUp.prevY = healthPickUp.y;
        healthPickUp.prevSize = healthPickUp.size;
    } else {
        if (healthPickUp.prevX != 0 || healthPickUp.prevY != 0 || healthPickUp.prevSize != 0) {
            tft.fillRect(healthPickUp.prevX - healthPickUp.prevSize / 2, 
                         healthPickUp.prevY - healthPickUp.prevSize / 2, 
                         healthPickUp.prevSize, healthPickUp.prevSize, TFT_BLACK);
            healthPickUp.prevX = 0; 
            healthPickUp.prevY = 0;
            healthPickUp.prevSize = 0;
        }
    }
}
void drawCharacterSprite(Player& p) {
    TFT_eSprite& currentSprite = (&p == &player1) ? player1Sprite : player2Sprite;

    const uint16_t* currentSpriteData;
    int currentSpriteWidth;
    int currentSpriteHeight;

    if (p.attacking) {
        currentSpriteData = p.facingRight ? knight_attack : knight_attack_left;
        currentSpriteWidth = KNIGHT_ATTACK_WIDTH;
        currentSpriteHeight = KNIGHT_ATTACK_HEIGHT;
    } else {
        currentSpriteData = p.facingRight ? knight_panda : knight_panda_left;
        currentSpriteWidth = KNIGHT_PANDA_WIDTH;
        currentSpriteHeight = KNIGHT_PANDA_HEIGHT;
    }

    int drawX = round(p.x - (currentSpriteWidth / 2.0));
    int drawY = round(p.y - currentSpriteHeight);

    if (p.prevX != 0 || p.prevY != 0 || p.prevWidth != 0 || p.prevHeight != 0) { // İlk çizim değilse
        if (drawX != p.prevX || drawY != p.prevY ||
            currentSpriteWidth != p.prevWidth || currentSpriteHeight != p.prevHeight ||
            currentSpriteData != p.prevSprite) { 
         
            tft.fillRect(p.prevX, p.prevY, p.prevWidth, p.prevHeight, TFT_BLACK); 
        }
    }

    currentSprite.fillSprite(TRANSPARENT_COLOR);
    
    currentSprite.pushImage(0, 0, currentSpriteWidth, currentSpriteHeight, (uint16_t*)currentSpriteData);

    currentSprite.pushSprite(drawX, drawY, TRANSPARENT_COLOR);

    p.prevX = drawX;
    p.prevY = drawY;
    p.prevWidth = currentSpriteWidth;
    p.prevHeight = currentSpriteHeight;
    p.prevSprite = currentSpriteData;
}


void initSprites() {
    int maxSpriteWidth = max(KNIGHT_PANDA_WIDTH, KNIGHT_ATTACK_WIDTH);
    int maxSpriteHeight = max(KNIGHT_PANDA_HEIGHT, KNIGHT_ATTACK_HEIGHT);

    player1Sprite.setColorDepth(16); 
    player1Sprite.createSprite(maxSpriteWidth, maxSpriteHeight);
    player1Sprite.setSwapBytes(true); 

    player2Sprite.setColorDepth(16); 
    player2Sprite.createSprite(maxSpriteWidth, maxSpriteHeight);
    player2Sprite.setSwapBytes(true); 

    bgSprite.setColorDepth(16); 
    bgSprite.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT); 
    bgSprite.setSwapBytes(true);

    uiSprite.setColorDepth(8); 
    uiSprite.createSprite(SCREEN_WIDTH, 20); 
    uiSprite.setSwapBytes(true);

    groundSprite.setColorDepth(16);
    groundSprite.createSprite(GROUND_SPRITE_WIDTH, GROUND_SPRITE_HEIGHT);
    groundSprite.setSwapBytes(true);

    groundSprite.pushImage(0, 0, GROUND_SPRITE_WIDTH, GROUND_SPRITE_HEIGHT, (uint16_t*)ground_sprite_data);
}

void drawTitleScreen() {
    tft.fillScreen(TFT_BLACK); 
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(3);
    tft.setCursor(30, 40);
    tft.println("KNIGHT DUEL");
    tft.setTextSize(2);
    tft.setCursor(70, 100);
    tft.println("Player 1: BLUE");
    tft.setCursor(70, 130);
    tft.println("Player 2: RED");
    tft.setTextSize(1);
    tft.setCursor(20, 180);
    tft.println("Move: Joystick | Attack: Button");
    tft.setCursor(20, 200);
    tft.println("Hold attack for stronger moves!");
    tft.setCursor(80, 220);
    tft.println("Press any button to start");
}

void drawGameOverScreen(int winner) {
    tft.fillScreen(TFT_BLACK); 
    tft.setTextSize(3);
    tft.setCursor(50, 100);
    if (winner == 1) {
        tft.setTextColor(TFT_BLUE);
        tft.println("PLAYER 1 WINS!");
    } else {
        tft.setTextColor(TFT_RED);
        tft.println("PLAYER 2 WINS!");
    }
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(100, 160);
    tft.println("Game restarting...");
}

void updateHealthBars() {
    uiSprite.fillSprite(TFT_BLACK); 
    uiSprite.setTextColor(TFT_BLUE);
    uiSprite.setCursor(10, 13);
    uiSprite.print("P1");
    uiSprite.setTextColor(TFT_RED);
    uiSprite.setCursor(300, 13);
    uiSprite.print("P2");
    
    int p1BarWidth = map(player1.health, 0, 100, 0, 100);
    int p2BarWidth = map(player2.health, 0, 100, 0, 100);
    
    uiSprite.drawRect(30, 4, 100, 6, TFT_WHITE); 
    uiSprite.fillRect(30, 4, p1BarWidth, 6, TFT_BLUE);
    
    uiSprite.drawRect(190, 4, 100, 6, TFT_WHITE); 
    uiSprite.fillRect(290 - p2BarWidth, 4, p2BarWidth, 6, TFT_RED); 
    
    uiSprite.pushSprite(0, 0); 

void drawSlashEffect(int x, int y, bool facingRight, int power) {
    if (!slashEffect.active) return;
    uint16_t color;
    switch (power) {
        case 1: color = TFT_WHITE; break;
        case 2: color = TFT_YELLOW; break;
        case 3: color = TFT_ORANGE; break;
        default: color = TFT_WHITE;
    }
    int size = 10 + (power * 5);
 
    if (facingRight) {
        tft.drawLine(x, y, x + size, y - size, color);
        tft.drawLine(x, y, x + size, y + size, color);
        tft.drawLine(x + 5, y, x + size + 5, y - size, color);
        tft.drawLine(x + 5, y, x + size + 5, y + size, color);
    } else {
        tft.drawLine(x, y, x - size, y - size, color);
        tft.drawLine(x, y, x - size, y + size, color);
        tft.drawLine(x - 5, y, x - size - 5, y - size, color);
        tft.drawLine(x - 5, y, x - size - 5, y + size, color);
    }
}

void drawGame() {
  

      groundSprite.pushSprite(0, 180);

    drawCharacterSprite(player1);
    drawCharacterSprite(player2);

    if (slashEffect.active) {
        drawSlashEffect(slashEffect.x, slashEffect.y, slashEffect.facingRight, slashEffect.power);
    }
    drawHealthPickUp();
    updateHealthBars();
}
