#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "NoteData.h"

// GUItool: begin automatically generated code
AudioSynthWaveformModulated waveformMod1;   //xy=242,198
AudioSynthWaveformModulated waveformMod2;   //xy=247,271
AudioSynthNoisePink      pink1;          //xy=258,350
AudioMixer4              mixer1;         //xy=485,232
AudioOutputI2S           i2s1;           //xy=730,235
AudioConnection          patchCord1(waveformMod1, 0, mixer1, 0);
AudioConnection          patchCord2(waveformMod2, 0, mixer1, 1);
AudioConnection          patchCord3(pink1, 0, mixer1, 2);
AudioConnection          patchCord4(mixer1, 0, i2s1, 0);
AudioConnection          patchCord5(mixer1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=489,53
// GUItool: end automatically generated code


void OnNoteOn(byte channel, byte note, byte velocity) {
  Serial.printf("ch: %u, note: %u, vel: %u\n", channel, note, velocity);
  waveformMod1.frequency(tune_frequencies2_PGM[note]);
  waveformMod1.amplitude(1.0);
}

void OnNoteOff(byte channel, byte note, byte velocity) {
  waveformMod1.amplitude(0);
  Serial.println("and off");
}

void setup() {
  Serial.begin(9600);
  AudioMemory(20);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  waveformMod1.begin(WAVEFORM_SINE);
  mixer1.gain(0, 0.3);
  mixer1.gain(1, 0.3);
  mixer1.gain(2, 0.3);
  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleNoteOn(OnNoteOn);
}

void loop() {
  // put your main code here, to run repeatedly:
  usbMIDI.read();
}
