#pragma once

#include "resource.h"

typedef enum {
    NONE,
    DISPLAY_1,
    DISPLAY_2,
    DISPLAY_3
} active_t;

extern active_t activeDisplay;
extern int mouseX, mouseY;
extern bool mouseDown;
