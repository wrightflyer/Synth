#include "ILI9341_t3.h"

#define TFT_DC  9
#define TFT_CS 10

ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);

void setup() {
	tft.begin();
	tft.fillScreenVGradient(CL(255, 255, 255), CL(0, 0, 255));
}

void loop() {

}