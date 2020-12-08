#ifndef _DRAWING_H_
#define _DRAWING_H_
#include "src/data/types.h"
#include <font_Arial.h>
#include <font_ArialBold.h>
#include "src/data/constdata.h"

void tftInit();
boolean pointInRect(int x, int y, int rectX, int rectY, int rectW, int rectH);
int pointInBar(int x, int y, int barX, int barY);
void whiteKey(int n, bool pressed);
void blackKey(int n, bool pressed);
void highlightKey(int note, bool onoff);
void drawBar(int x, int y, int value, const char * text);
void updateMixerBars(Mix_change_t change);
void updateADSRBars(ADSR_change_t change);

#endif