#pragma once

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "types.h"

#include "SynthEngine.h"
#include "Arpeggiator.h"
#include "ContinuousController.h"
#include "ProgChange.h"
#include "PitchBend.h"
#include "NoteHandling.h"
#include <ILI9341_t3.h>
#include <MIDI.h>
#include <Encoder.h>
#include <XPT2046_Touchscreen.h>
 
// PhatBass: begin automatically generated code
// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);
XPT2046_Touchscreen ts(TOUCH_CS);
Encoder enc(2,3);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

SynthEngine unused_Synth;
Arpeggiator unused_Arp;
ContinuousController CCHndlr;
ProgChange ProgHndlr;
PitchBend PBHndlr;
NoteHandling NoteHndlr;

void setup() {
    pinMode(LED_PIN, OUTPUT);
    pinMode(JOY_SW, INPUT_PULLUP);
    Serial.begin(9600);
    Serial1.begin(31250);
    Serial.println("The bastard is Alive!");
    MIDI.begin();
    ts.begin();
    ts.setRotation(3);
    tftInit(); 
    AudioMemory(20);
    sgtl5000_1.enable();
    sgtl5000_1.volume(0.75);
}

void loop() {
    MIDI.read()

    SynthEngine::update();
    Arpeggiator::update();
}

// PhatBass: end automatically generated code
