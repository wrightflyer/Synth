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
    mTFT.fillRoundRect(mX, mY, mW, mH, 5, mBkgnd);
    // draw full height outline
    mTFT.drawRoundRect(mX, mY, mW, mH, 5, ILI9341_BLACK);
    // draw filled part rect to show value
    mTFT.fillRoundRect(mX, mY + mPos, mW, mH - mPos, 5, ILI9341_BLACK);
    // add a label above
    if (mLabel != NULL) {
        mTFT.setCursor(mX + 8, mY + mH + 8);
        mTFT.print(mLabel);
    }
}

void VSlider::setSlider(int val, float min, float max) {
    mPos = mapf(val, min, max, 0.0, mH);
    draw();
}

void VSlider::setLabel(char * text) {
    strncpy_s(mLabel, text, 100);
}

void VSlider::setPos(int x, int y){
    mX = x;
    mY = y;
}
void VSlider::setSize(int w, int h){
    mW = w;
    mH = h;
}

bool VSlider::inSlider(int x, int y) {
    return pointInRect(x, y, mX, mY, mW, mH);
}

void VSlider::setBGcolor(uint16_t col) {
    mBkgnd = col;
}

void RadioGroup::setElements(vector<string> & labels) {
    int width;
    mLabels = labels;
    mW = 0;
    mTFT.setFontAdafruit();
    mTFT.setTextSize(mFontSize);
    // work out widest bit..
    for (int i = 0; i < mLabels.size(); i++) {
        width = mTFT.measureTextWidth(mLabels[i].c_str(), 0) + (mRadius * 2) + 4;
        if (width > mW) {
            mW = width; // remember widest
        }
    }
}

void RadioGroup::draw() {
    mTFT.setFontAdafruit();
    mTFT.setTextSize(mFontSize);
    for (int i = 0; i < mLabels.size(); i++) {
        if (i == mSelected) {
            mTFT.fillCircle(mX, mY + (i * mH), mRadius, CL(0, 0, 0));
        }
        else {
            mTFT.fillCircle(mX, mY + (i * mH), mRadius, mBkgnd);
            mTFT.drawCircle(mX, mY + (i * mH), mRadius, CL(0, 0, 0));
        }
        mTFT.setTextColor(CL(0, 0, 0));
        mTFT.setCursor(mX + mRadius + 4, mY + (i * mH) - (mRadius / 2));
        mTFT.print(mLabels[i].c_str());
    }
}

void RadioGroup::setPos(int x, int y) {
    mX = x;
    mY = y;
}

void RadioGroup::setSelected(int n) {
    mSelected = n;
}

int RadioGroup::getSelected() {
    return mSelected;
}

size_t RadioGroup::numElements() {
    return mLabels.size();
}

void RadioGroup::mouseClick(int x, int y) {
    for (int i = 0; i < mLabels.size(); i++) {
        if (pointInRect(x, y, mX - mRadius, mY + (i * mH) - mRadius - 4, mW, mH)) {
            setSelected(i);
            break;
        }
    }
}