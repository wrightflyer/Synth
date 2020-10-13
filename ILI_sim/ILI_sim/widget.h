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
    VSlider(ILI9341_t3 & tft, int x, int y, int w, int h, uint16_t bkCol = CL(192, 192, 192), char * label = "") : mTFT(tft), mX(x), mY(y), mW(w), mH(h), mBkgnd(bkCol) {
        strncpy_s(mLabel, label, 100);
        mPos = h / 2;
    };
    void draw();
    void setSlider(int val, float min = 0, float max = 127);
    void setLabel(char * text);
    void setPos(int x, int y);
    void setSize(int w, int h);
    bool inSlider(int x, int y);
    void setBGcolor(uint16_t col);
private:
    char mLabel[100];
    int mX;
    int mY;
    int mW;
    int mH;
    int mPos;
    ILI9341_t3 & mTFT;
    uint16_t mBkgnd;
};

class RadioGroup : WidgetUtil {
public:
    RadioGroup(ILI9341_t3 & tft, int x, int y, int radius, uint16_t bkcol = CL(192, 192, 192), int selected = 0) 
        : mTFT(tft), mX(x), mY(y), mRadius(radius), mSelected(selected), mBkgnd(bkcol), mFontSize(1) {
        mH = radius * 3;
    };
    void draw();
    void setElements(vector<string> & labels);
    void setFontSize(int n) { mFontSize = n; };
    void setPos(int x, int y);
    void setSelected(int n);
    int getSelected();
    size_t numElements();
    void mouseClick(int x, int y);
private:
    int mX;
    int mY;
    int mH;
    int mW;
    int mRadius;
    int mSelected;
    ILI9341_t3 & mTFT;
    vector<string> mLabels;
    int mFontSize;
    uint16_t mBkgnd;
};

#endif