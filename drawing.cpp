#include "drawing.h"
#include "globals.h"

double mapf(double x, double in_min, double in_max, double out_min, double out_max);

void tftInit() {
  tft.begin();
  tft.setRotation(3);
  // show the pretty kitty
  //tft.writeRect(0, 0, 320, 240, amelia320);
  // and give the world a chance to marvel in her glory
  //delay(4000);
  // then clear the decks  tft.fillScreen(ILI9341_LIGHTGREY);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(40, 0);
  tft.setFont(Arial_18_Bold);
  tft.println("Teensy Synth");
  tft.fillRoundRect(ADSR_PANEL_X, ADSR_PANEL_Y, ADSR_PANEL_W, PANEL_H, 5, ILI9341_DARKGREY);
  tft.setCursor(ADSR_PANEL_X + 30, ADSR_PANEL_Y);
  tft.setFont(Arial_14);
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

boolean pointInRect(int x, int y, int rectX, int rectY, int rectW, int rectH) {
  boolean ret = false;
  if ((x >= rectX) && (x <= (rectX + rectW)) && (y >= rectY) && (y <= (rectY + rectH))) {
    ret = true;
  }
  Serial.printf("x=%u y=%u, Rx=%u, Ry=%u, Rw=%u Rh=%u, Rx2=%u, Ry2=%u, ret=%u\n",
            x, y, rectX, rectY, rectW, rectH, rectX+rectW, rectY+rectH, ret);
  return ret;
}

int pointInBar(int x, int y, int barX, int barY) {
  int ret = -1;
  if (pointInRect(x, y, barX, barY + BAR_OFFSET, BAR_WIDTH, BAR_HEIGHT)) {
    ret = map((y - (barY + BAR_OFFSET)), BAR_HEIGHT, 0, 0, 127);
  }
  return ret;
}

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

void highlightKey(int note, bool onoff) {
    if (keybd[note - 48].type == 0) {
      whiteKey((int)keybd[note - 48].offset, onoff);
    }
    else {
      blackKey((int)keybd[note - 48].offset, onoff);
    }  
}

void drawBar(int x, int y, int value, const char * text) {
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

void updateMixerBars(Mix_change_t change) {
  int w1_val, w2_val, noise_val;
  if ((change == OSC1) || (change == AllMix)) {
    w1_val = BAR_HEIGHT - mapf(osc1Amp, 0.0, 1.0, 0, BAR_HEIGHT);
    drawBar(MIX_PANEL_X + 5, MIX_PANEL_Y, w1_val, (change == AllMix) ? "1" : NULL);
  }
  if ((change == OSC2) || (change == AllMix)) {
    w2_val = BAR_HEIGHT - mapf(osc2Amp, 0.0, 1.0, 0, BAR_HEIGHT);
    drawBar(MIX_PANEL_X + 30, MIX_PANEL_Y, w2_val, (change == AllMix) ? "2" : NULL);
  }
  if ((change == Noise) || (change == AllMix)) {
    noise_val = BAR_HEIGHT - mapf(noiseAmp, 0.0, 1.0, 0, BAR_HEIGHT);
    drawBar(MIX_PANEL_X + 55, MIX_PANEL_Y, noise_val, (change == AllMix) ? "N" : NULL);
  }  
}

void updateADSRBars(ADSR_change_t change) {
  int a_val, d_val, s_val, r_val;
  if ((change == Attack) || (change == AllADSR)) {
    a_val = BAR_HEIGHT - mapf(envAttack, 0, 2000.0, 0, BAR_HEIGHT);
    drawBar(ADSR_PANEL_X + 5, ADSR_PANEL_Y, a_val, (change == AllADSR) ? "A" : NULL);
  }
  if ((change == Decay) || (change == AllADSR)) {
    d_val = BAR_HEIGHT - mapf(envDecay, 0, 2000.0, 0, BAR_HEIGHT); 
    drawBar(ADSR_PANEL_X + 30, ADSR_PANEL_Y, d_val, (change == AllADSR) ? "D" : NULL);
  }
  if ((change == Sustain) || (change == AllADSR)) {
    s_val = BAR_HEIGHT - mapf(envSustain, 0, 1.0, 0, BAR_HEIGHT); 
    drawBar(ADSR_PANEL_X + 55, ADSR_PANEL_Y, s_val, (change == AllADSR) ? "S" : NULL);
  }
  if ((change == Release) || (change == AllADSR)) {
    r_val = BAR_HEIGHT - mapf(envRelease, 0, 2000.0, 0, BAR_HEIGHT); 
    drawBar(ADSR_PANEL_X + 80, ADSR_PANEL_Y, r_val, (change == AllADSR) ? "R" : NULL);
  }  
}
