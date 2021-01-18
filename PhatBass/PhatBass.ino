#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "types.h"

#include "Synth.h"
 
// PhatBass: begin automatically generated code

Synth phatBass;

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
    phatBass.update();
}

// PhatBass: end automatically generated code
