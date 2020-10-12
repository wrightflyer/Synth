#include "widget.h"

double WidgetUtil::mapf(double x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

bool WidgetUtil::pointInRect(int x, int y, int rectX, int rectY, int rectW, int rectH){
    bool ret = false;
    if ((x >= rectX) && (x <= (rectX + rectW)) && (y >= rectY) && (y <= (rectY + rectH))) {
        ret = true;
    }
    return ret;
}

void VSlider::draw() {
    // erase existing bar  
    mTFT.fillRoundRect(mX, mY, mW, mH, 5, ILI9341_LIGHTGREY);
    // draw full height outline
    mTFT.drawRoundRect(mX, mY, mW, mH, 5, ILI9341_BLACK);
    // draw filled part rect to show value
    mTFT.fillRoundRect(mX, mY + mPos, mW, mH - mPos, 5, ILI9341_BLACK);
    // add a label above
    if (mLabel != NULL) {
        mTFT.setCursor(mX + 8, mY + mH + 20);
        mTFT.print(mLabel);
    }
}

void VSlider::setPos(int val, float min, float max) {
    mPos = mapf(val, min, max, 0, mH);
}

void VSlider::setLabel(char * text) {
    strncpy(mLabel, text, 100);
}
