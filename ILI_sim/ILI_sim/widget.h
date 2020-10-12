#ifndef _WIDGET_H_
#define _WIDGET_H_

#include "string.h"
#include <vector>
#include <string>
#include "ILI9341_t3.h"

using namespace std;

class WidgetUtil {
public:
    double mapf(double x, double in_min, double in_max, double out_min, double out_max);
    bool pointInRect(int x, int y, int rectX, int rectY, int rectW, int rectH);
};

class VSlider : WidgetUtil {
public:
    VSlider(ILI9341_t3 & tft, int x, int y, int w, int h, char * label = "") : mTFT(tft), mX(x), mY(y), mW(w), mH(h) {
        strncpy_s(mLabel, label, 100);
        mPos = h / 2;
    };
    void draw();
    void setSlider(int val, float min = 0, float max = 127);
    void setLabel(char * text);
    void setPos(int x, int y);
    void setSize(int w, int h);
    bool inSlider(int x, int y);
private:
    char mLabel[100];
    int mX;
    int mY;
    int mW;
    int mH;
    int mPos;
    ILI9341_t3 & mTFT;
};

class RadioGroup : WidgetUtil {
public:
    RadioGroup(ILI9341_t3 & tft, int x, int y, int radius, int selected = 0) 
        : mTFT(tft), mX(x), mY(y), mRadius(radius), mSelected(selected), mFontSize(1) {
    };
    void draw();
    void setElements(vector<string> & labels);
    void setFontSize(int n) { mFontSize = n; };
private:
    int mX;
    int mY;
    int mRadius;
    int mSelected;
    ILI9341_t3 & mTFT;
    vector<string> mLabels;
    int mFontSize;
};

#endif