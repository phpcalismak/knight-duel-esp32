#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>   // Arduino çekirdek fonksiyonları için
#include <TFT_eSPI.h>  // TFT ekran kütüphanesi için

// Oyun mantığı ve yapılandırma dosyaları
#include "game_logic.h"    // Player yapısı ve slashEffect için
#include "game_config.h"   // SCREEN_WIDTH, SCREEN_HEIGHT gibi tanımlamalar için
#define TRANSPARENT_COLOR TFT_BLACK
#include "knight.h" 

// TFT_eSPI nesneleri global olarak tanımlanır ve diğer dosyalardan erişilebilmesi için 'extern' ile bildirilir.
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

/**
 * @brief TFT_eSPI sprite'larını başlatır ve yapılandırır.
 */
void initSprites();

/**
 * @brief Oyuncu karakter sprite'ını ekrana çizer.
 * @param p Çizilecek oyuncunun Player yapısı.
 * @param spriteData Karakterin RGB565 piksel verisi (uint16_t dizisi).
 * @param spriteWidth Çizilecek sprite'ın genişliği.
 * @param spriteHeight Çizilecek sprite'ın yüksekliği.
 */
void drawCharacterSprite(Player& p, const uint16_t* spriteData, int spriteWidth, int spriteHeight);

/**
 * @brief Ninja karakter sprite'ını ilkel şekillerle çizer.
 * @param sprite Çizimin yapılacağı TFT_eSprite objesi.
 * @param p Çizilecek oyuncunun Player yapısı.
 * @param color Ana renk.
 * @param detailColor Detay rengi.
 */


/**
 * @brief Oyunun başlangıç ekranını çizer.
 */
void drawTitleScreen();

/**
 * @brief Oyun sonu ekranını (kazananı belirterek) çizer.
 * @param winner Kazanan oyuncunun numarası (1 veya 2).
 */
void drawGameOverScreen(int winner);

/**
 * @brief Oyuncuların sağlık çubuklarını günceller ve ekrana basar.
 */
void updateHealthBars();

/**
 * @brief Kılıç darbesi efektini çizer.
 * @param x Efektin X koordinatı.
 * @param y Efektin Y koordinatı.
 * @param facingRight Oyuncunun sağa mı baktığı (efekt yönü için).
 * @param power Atak gücü (efektin boyutu ve rengi için).
 */
void drawSlashEffect(int x, int y, bool facingRight, int power);

/**
 * @brief Oyun sahnesini (arka plan, karakterler, efektler) her karede çizer.
 */

void drawGroundSprite();
void drawGame();

#endif // DISPLAY_MANAGER_H