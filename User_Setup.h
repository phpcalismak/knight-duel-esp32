// User_Setup.h - ESP32 + ILI9341 + Hardware SPI

#define ILI9341_DRIVER       // Ekran sürücüsü

// Ekran çözünürlüğü (ili9341 için sabit)
#define TFT_WIDTH  240
#define TFT_HEIGHT 320

// SPI pini tanımları
#define TFT_MOSI 23   // SPI MOSI
#define TFT_SCLK 18   // SPI CLOCK
#define TFT_CS   5    // Chip select
#define TFT_DC   2    // Data/command
#define TFT_RST  4    // Reset

#define LOAD_GLCD     // Default font
#define LOAD_FONT2    // Font 2. Small 16 pixel high font
#define LOAD_FONT4    // Font 4. Medium 26 pixel font
#define LOAD_FONT6    // Font 6. Large 48 pixel font
#define LOAD_FONT7    // Font 7. 7-segment 48 pixel font
#define LOAD_FONT8    // Font 8. Large 75 pixel font

#define LOAD_GFXFF    // FreeFonts destekle
#define SMOOTH_FONT

// Renk sırası
#define TFT_RGB_ORDER TFT_RGB

// SPI frekansları
#define SPI_FREQUENCY  40000000  // 40 MHz
#define SPI_READ_FREQUENCY 20000000
#define SPI_TOUCH_FREQUENCY 2500000
