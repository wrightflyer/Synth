#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "NoteData.h"

// GUItool: begin automatically generated code
AudioSynthWaveform       waveform1;      //xy=291,219
AudioOutputI2S           i2s1;           //xy=599,232
AudioConnection          patchCord1(waveform1, 0, i2s1, 0);
AudioConnection          patchCord2(waveform1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=489,53
// GUItool: end automatically generated code


void OnNoteOn(byte channel, byte note, byte velocity) {
  Serial.printf("ch: %u, note: %u, vel: %u\n", channel, note, velocity);
  waveform1.frequency(tune_frequencies2_PGM[note]);
  waveform1.amplitude(1.0);
  waveform1.pulseWidth(0.15);
}

void OnNoteOff(byte channel, byte note, byte velocity) {
  waveform1.amplitude(0);
  Serial.println("and off");
}

void setup() {
  Serial.begin(9600);
  AudioMemory(20);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  waveform1.begin(WAVEFORM_SINE);
  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleNoteOn(OnNoteOn);
}

void loop() {
  // put your main code here, to run repeatedly:
  usbMIDI.read();
}
