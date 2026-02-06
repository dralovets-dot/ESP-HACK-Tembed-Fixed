// Setup for LilyGo T-Embed CC1101
// Based on official LilyGO examples

#define USER_SETUP_INFO "LilyGo T-Embed CC1101"

// ST7789 driver
#define ST7789_DRIVER

// Display dimensions
#define TFT_WIDTH  170
#define TFT_HEIGHT 320

// Pin definitions (official from datasheet)
#define TFT_MOSI 9
#define TFT_SCLK 11
#define TFT_CS   41
#define TFT_DC   16
#define TFT_RST  40

// Backlight
#define TFT_BL   21

// SPI frequency
#define SPI_FREQUENCY  40000000

// Font support
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define SMOOTH_FONT
