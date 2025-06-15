// display_manager.cpp
#include "display_manager.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite player1Sprite = TFT_eSprite(&tft); // Karakterleri çizmek için kullanılacak
TFT_eSprite player2Sprite = TFT_eSprite(&tft); // Karakterleri çizmek için kullanılacak
TFT_eSprite bgSprite = TFT_eSprite(&tft);     // Arka planı temizlemek ve çizmek için
TFT_eSprite uiSprite = TFT_eSprite(&tft);     // UI elementleri için
TFT_eSprite groundSprite = TFT_eSprite(&tft);

void drawHealthPickUp() {
    if (healthPickUp.active) {
        // Önceki pozisyonu temizle
        if (healthPickUp.prevX != 0 || healthPickUp.prevY != 0) { // İlk çizim değilse
            tft.fillRect(healthPickUp.prevX - healthPickUp.prevSize / 2, // X pozisyonu merkezden, o yüzden yarısını çıkar
                         healthPickUp.prevY - healthPickUp.prevSize / 2, // Y pozisyonu merkezden
                         healthPickUp.prevSize, healthPickUp.prevSize, TFT_BLACK);
        }
        
        // Noktayı çiz 
        tft.fillCircle(healthPickUp.x, healthPickUp.y, healthPickUp.size / 2, healthPickUp.color); // Daire çizimi için
        

        // Mevcut pozisyonu kaydet
        healthPickUp.prevX = healthPickUp.x;
        healthPickUp.prevY = healthPickUp.y;
        healthPickUp.prevSize = healthPickUp.size;
    } else {
        // Eğer healthPickUp pasif hale geldiyse ve hala ekranda çizilmiş bir önceki konumu varsa, temizle
        if (healthPickUp.prevX != 0 || healthPickUp.prevY != 0 || healthPickUp.prevSize != 0) {
            tft.fillRect(healthPickUp.prevX - healthPickUp.prevSize / 2, 
                         healthPickUp.prevY - healthPickUp.prevSize / 2, 
                         healthPickUp.prevSize, healthPickUp.prevSize, TFT_BLACK);
            healthPickUp.prevX = 0; // Temizlendiğini işaretle
            healthPickUp.prevY = 0;
            healthPickUp.prevSize = 0;
        }
    }
}
// Karakter çizimini ve eski pozisyon temizliğini yöneten fonksiyon
void drawCharacterSprite(Player& p) {
    // Hangi sprite'ın kullanılacağını belirle
    TFT_eSprite& currentSprite = (&p == &player1) ? player1Sprite : player2Sprite;

    // Hangi sprite datasının ve boyutunun kullanılacağını belirle
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

    // Çizim pozisyonlarını hesapla (sprite'ın alt-orta noktası p.x, p.y'ye hizalanır)
    int drawX = round(p.x - (currentSpriteWidth / 2.0));
    int drawY = round(p.y - currentSpriteHeight);

    // Eski alanı temizle (sadece pozisyon/boyut/sprite değiştiyse)
    if (p.prevX != 0 || p.prevY != 0 || p.prevWidth != 0 || p.prevHeight != 0) { // İlk çizim değilse
        if (drawX != p.prevX || drawY != p.prevY ||
            currentSpriteWidth != p.prevWidth || currentSpriteHeight != p.prevHeight ||
            currentSpriteData != p.prevSprite) { // Sprite değiştiyse de eski alanı temizle
            
            // Eski karakterin kapladığı alanı siyaha boyayarak temizle
            // Zemin çizgisinin üzerine basmamaya dikkat etmeliyiz.
            // Bu kısım biraz daha gelişmiş bir temizleme mantığı gerektirebilir
            // Ancak şimdilik bu şekilde tutalım ve sonraki adımda zemini de içeren temizleme düşünelim.
            tft.fillRect(p.prevX, p.prevY, p.prevWidth, p.prevHeight, TFT_BLACK); 
        }
    }

    // Sprite'ın içindeki tamponu şeffaf renkle doldur (genellikle siyah)
    currentSprite.fillSprite(TRANSPARENT_COLOR);
    
    // RGB565 sprite verilerini sprite tamponuna kopyala.
    currentSprite.pushImage(0, 0, currentSpriteWidth, currentSpriteHeight, (uint16_t*)currentSpriteData);

    // Oluşturulan sprite'ı ana ekrana basarken ŞEFFAF RENK PARAMETRESİNİ VER.
    currentSprite.pushSprite(drawX, drawY, TRANSPARENT_COLOR);

    // Mevcut pozisyon, boyut ve kullanılan sprite'ı kaydet
    p.prevX = drawX;
    p.prevY = drawY;
    p.prevWidth = currentSpriteWidth;
    p.prevHeight = currentSpriteHeight;
    p.prevSprite = currentSpriteData;
}


void initSprites() {
    // Karakter sprite'larını 16-bit renk derinliğinde oluştur
    int maxSpriteWidth = max(KNIGHT_PANDA_WIDTH, KNIGHT_ATTACK_WIDTH);
    int maxSpriteHeight = max(KNIGHT_PANDA_HEIGHT, KNIGHT_ATTACK_HEIGHT);

    player1Sprite.setColorDepth(16); // RGB565 için 16-bit
    player1Sprite.createSprite(maxSpriteWidth, maxSpriteHeight);
    player1Sprite.setSwapBytes(true); 

    player2Sprite.setColorDepth(16); // RGB565 için 16-bit
    player2Sprite.createSprite(maxSpriteWidth, maxSpriteHeight);
    player2Sprite.setSwapBytes(true); 

    // Arka plan sprite'ını ekranın TAM boyutunda oluştur
    bgSprite.setColorDepth(16); 
    bgSprite.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT); 
    bgSprite.setSwapBytes(true);

    uiSprite.setColorDepth(8); 
    uiSprite.createSprite(SCREEN_WIDTH, 20); 
    uiSprite.setSwapBytes(true);

  // Zemin sprite'ını oluştur
    groundSprite.setColorDepth(16);
    groundSprite.createSprite(GROUND_SPRITE_WIDTH, GROUND_SPRITE_HEIGHT);
    groundSprite.setSwapBytes(true);

    // Zemin sprite verilerini yükle (sadece bir kez setup'ta)
    groundSprite.pushImage(0, 0, GROUND_SPRITE_WIDTH, GROUND_SPRITE_HEIGHT, (uint16_t*)ground_sprite_data);
}

void drawTitleScreen() {
    tft.fillScreen(TFT_BLACK); // Başlık ekranına geçerken tüm ekranı temizle
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
    tft.fillScreen(TFT_BLACK); // Game Over ekranına geçerken tüm ekranı temizle
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
    uiSprite.fillSprite(TFT_BLACK); // UI sprite'ını her seferinde temizle
    uiSprite.setTextColor(TFT_BLUE);
    uiSprite.setCursor(10, 13);
    uiSprite.print("P1");
    uiSprite.setTextColor(TFT_RED);
    uiSprite.setCursor(300, 13);
    uiSprite.print("P2");
    
    // Sağlık çubuklarını çiz
    int p1BarWidth = map(player1.health, 0, 100, 0, 100);
    int p2BarWidth = map(player2.health, 0, 100, 0, 100);
    
    uiSprite.drawRect(30, 4, 100, 6, TFT_WHITE); // P1 sağlık barı çerçevesi
    uiSprite.fillRect(30, 4, p1BarWidth, 6, TFT_BLUE); // P1 sağlık barı içi
    
    uiSprite.drawRect(190, 4, 100, 6, TFT_WHITE); // P2 sağlık barı çerçevesi
    uiSprite.fillRect(290 - p2BarWidth, 4, p2BarWidth, 6, TFT_RED); // P2 sağlık barı içi (sağdan sola dolacak)
    
    uiSprite.pushSprite(0, 0); // UI sprite'ını ekrana bas
}

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
  
    // Zemin çizgisini çiz

      groundSprite.pushSprite(0, 180);

    // Karakterleri çiz
    drawCharacterSprite(player1);
    drawCharacterSprite(player2);

    // Slash efektini çiz (slashEffect'in aktifliği game_logic.cpp'de yönetiliyor)
    if (slashEffect.active) {
        drawSlashEffect(slashEffect.x, slashEffect.y, slashEffect.facingRight, slashEffect.power);
    }
     // Can veren noktayı çiz
    drawHealthPickUp();

    // Sağlık çubuklarını çiz
    
    // Sağlık çubuklarını çiz
    updateHealthBars();
}