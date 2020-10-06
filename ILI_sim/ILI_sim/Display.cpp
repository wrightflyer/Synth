#include "ILI9341_t3.h"
#include "font_Arial.h"
#include "font_ArialBold.h"

extern "C" {
    extern const unsigned short amelia320[];
}
#define TFT_DC  9
#define TFT_CS 10

#define PANEL_H 140
#define BAR_HEIGHT 100
#define BAR_WIDTH 20
#define BAR_OFFSET 15
#define MIX_PANEL_X 10
#define MIX_PANEL_Y 30
#define MIX_PANEL_W 80
#define ADSR_PANEL_X 120
#define ADSR_PANEL_Y 30
#define ADSR_PANEL_W 100
#define KEYB_X  10
#define KEYB_Y  200
#define KEY_WIDTH 12

ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);

typedef struct {
    uint8_t type;
    uint16_t offset;
} keyb_t;

// keys from 48 up - black (1) or white (0)
const keyb_t keybd[] = {
  { 0, 0 },
  { 1, KEY_WIDTH * 0.5 },
  { 0, KEY_WIDTH },
  { 1, KEY_WIDTH * 1.5 },
  { 0, KEY_WIDTH * 2 },
  { 0, KEY_WIDTH * 3 },
  { 1, KEY_WIDTH * 3.5 },
  { 0, KEY_WIDTH * 4 },
  { 1, KEY_WIDTH * 4.5 },
  { 0, KEY_WIDTH * 5 },
  { 1, KEY_WIDTH * 5.5 },
  { 0, KEY_WIDTH * 6 },
  { 0, KEY_WIDTH * 7 },
  { 1, KEY_WIDTH * 7.5 },
  { 0, KEY_WIDTH * 8 },
  { 1, KEY_WIDTH * 8.5 },
  { 0, KEY_WIDTH * 9 },
  { 0, KEY_WIDTH * 10 },
  { 1, KEY_WIDTH * 10.5 },
  { 0, KEY_WIDTH * 11 },
  { 1, KEY_WIDTH * 11.5 },
  { 0, KEY_WIDTH * 12 },
  { 1, KEY_WIDTH * 12.5 },
  { 0, KEY_WIDTH * 13 },
  { 0, KEY_WIDTH * 14 },
  { 1, KEY_WIDTH * 14.5 },
  { 0, KEY_WIDTH * 15 },
  { 1, KEY_WIDTH * 15.5 },
  { 0, KEY_WIDTH * 16 },
  { 0, KEY_WIDTH * 17 },
  { 1, KEY_WIDTH * 17.5 },
  { 0, KEY_WIDTH * 18 },
  { 1, KEY_WIDTH * 18.5 },
  { 0, KEY_WIDTH * 19 },
  { 1, KEY_WIDTH * 19.5 },
  { 0, KEY_WIDTH * 20 }
};


void whiteKey(int n, bool pressed) {
    if (!pressed) {
        tft.fillRoundRect(KEYB_X + n, KEYB_Y + 20, 10, 20, 2, CL(180, 180, 180));
    }
    else {
        tft.fillRoundRect(KEYB_X + n, KEYB_Y + 20, 10, 20, 2, CL(0, 180, 0));
    }
}

void blackKey(int n, bool pressed) {
    if (!pressed) {
        tft.fillRoundRect(KEYB_X + n, KEYB_Y, 10, 20, 2, ILI9341_BLACK);
    }
    else {
        tft.fillRoundRect(KEYB_X + n, KEYB_Y, 10, 20, 2, CL(0, 180, 0));
    }
}

void drawBar(int x, int y, int value, char * text) {
    // erase existing bar  
    tft.fillRoundRect(x, y + BAR_OFFSET, BAR_WIDTH, BAR_HEIGHT, 5, ILI9341_LIGHTGREY);
    // draw full height outline
    tft.drawRoundRect(x, y + BAR_OFFSET, BAR_WIDTH, BAR_HEIGHT, 5, ILI9341_BLACK);
    // draw filled part rect to show value
    tft.fillRoundRect(x, y + BAR_OFFSET + value, BAR_WIDTH, BAR_HEIGHT - value, 5, ILI9341_BLACK);
    // add a label above
    if (text != NULL) {
        tft.setCursor(x + 8, y + BAR_HEIGHT + 20);
		tft.setTextColor(0);
        tft.print(text);
    }
}

char text[] = "Test";

void setup() {
    tft.begin();
    tft.setRotation(3);
    //tft.fillScreen(CL(240, 240, 240));
    tft.writeRect(0, 0, 320, 240, amelia320);
    tft.setTextColor(CL(0, 160, 0));
    tft.setCursor(40, 0);
    tft.setFont(Arial_18_Bold);
    tft.println("Teensy Synth");
    tft.fillRoundRect(ADSR_PANEL_X, ADSR_PANEL_Y, ADSR_PANEL_W, PANEL_H, 5, ILI9341_DARKGREY);
    tft.setCursor(ADSR_PANEL_X + 30, ADSR_PANEL_Y);
    tft.setFont(Arial_14);
	tft.setTextColor(ILI9341_BLACK);
    tft.print("ADSR");
    tft.fillRoundRect(MIX_PANEL_X, MIX_PANEL_Y, MIX_PANEL_W, PANEL_H, 5, ILI9341_DARKGREY);
    tft.setCursor(MIX_PANEL_X + 20, MIX_PANEL_Y);
    tft.print("Mixer");
    tft.setFont(Arial_11_Bold);
    for (int i = 48; i < 84; i++) {
        if (keybd[i - 48].type == 0) {
            whiteKey(keybd[i - 48].offset, false);
        }
        else {
            blackKey(keybd[i - 48].offset, false);
        }
    }
}

void loop() {
    static int count5ms = 201;
    if (++count5ms > 200) {
        drawBar(MIX_PANEL_X + 5, MIX_PANEL_Y, rand() % 100, (char *)"1");
        drawBar(MIX_PANEL_X + 30, MIX_PANEL_Y, rand() % 100, (char *)"2");
        drawBar(MIX_PANEL_X + 55, MIX_PANEL_Y, rand() % 100, (char *)"N");

        drawBar(ADSR_PANEL_X + 5, ADSR_PANEL_Y, rand() % 100, (char *)"A");
        drawBar(ADSR_PANEL_X + 30, ADSR_PANEL_Y, rand() % 100, (char *)"D");
        drawBar(ADSR_PANEL_X + 55, ADSR_PANEL_Y, rand() % 100, (char *)"S");
        drawBar(ADSR_PANEL_X + 80, ADSR_PANEL_Y, rand() % 100, (char *)"R");
        count5ms = 0;
    }
#if 0
    tft.fillRect(0, 0, 100, 16, 0);
    tft.setTextColor(CL(255, 255, 255));
    tft.setCursor(0, 0);
    tft.print("count = ");
    tft.print(count5ms);
#endif
}