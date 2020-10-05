#include "ILI9341_t3.h"

#define TFT_DC  9
#define TFT_CS 10

#define PANEL_H 140
#define BAR_HEIGHT 100
#define BAR_WIDTH 20
#define BAR_OFFSET 15
#define MIX_PANEL_X 10
#define MIX_PANEL_Y 20
#define MIX_PANEL_W 80
#define ADSR_PANEL_X 120
#define ADSR_PANEL_Y 20
#define ADSR_PANEL_W 100
#define KEYB_X  10
#define KEYB_Y  200
#define KEY_WIDTH 12

ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);

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
		tft.print(text);
	}
}

void setup() {
	tft.begin();
	tft.fillScreenVGradient(CL(255, 255, 255), CL(0, 0, 255));
	tft.setTextColor(CL(128, 20, 0));
	drawBar(100, 20, 37, "Test");
}

void loop() {

}