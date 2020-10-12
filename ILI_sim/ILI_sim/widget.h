#ifndef _WIDGET_H_
#define _WIDGET_H_

#include "string.h"
#include "ILI9341_t3.h"

class WidgetUtil {
public:
    double mapf(double x, double in_min, double in_max, double out_min, double out_max);
    bool pointInRect(int x, int y, int rectX, int rectY, int rectW, int rectH);
};

class VSlider : WidgetUtil {
public:
    VSlider(ILI9341_t3 & tft, int x, int y, int w, int h, char * label = "") : mX(x), mY(y), mW(w), mH(h) {
        strncpy(mLabel, label, 100);
    };
    void draw();
    void setPos(int val, float min = 0, float max = 127);
    void setLabel(char * text);
private:
    char mLabel[100];
    int mX;
    int mY;
    int mW;
    int mH;
    int mPos;
    ILI9341_t3 & mTFT;
};

#endif