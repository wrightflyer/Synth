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

float wave1Amp = 1.0;
float wave2Amp = 1.0;
float noiseAmp = 1.0;

double mapf(double x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void OnNoteOn(byte channel, byte note, byte velocity) {
  Serial.printf("ch: %u, note: %u, vel: %u\n", channel, note, velocity);
  float freq = tune_frequencies2_PGM[note];
  waveformMod1.frequency(freq);
  waveformMod1.amplitude(wave1Amp);
  if (note > 12) {
    note -= 10;
    freq = tune_frequencies2_PGM[note];
  }
  waveformMod2.frequency(freq);
  waveformMod2.amplitude(wave2Amp);
  pink1.amplitude(noiseAmp);
}

void OnNoteOff(byte channel, byte note, byte velocity) {
  waveformMod1.amplitude(0);
  waveformMod2.amplitude(0);
  pink1.amplitude(0);
  Serial.println("and off");
}

void updateMix() {
      mixer1.gain(0, wave1Amp);
      mixer1.gain(1, wave2Amp);
      mixer1.gain(2, noiseAmp);
}

void OnControlChange(byte channel, byte control /* CC num*/, byte value /* 0 .. 127 */) {
  switch(control) {
    case 100:
      wave1Amp =  velocity2amplitude[value];
      updateMix();
      break;

    case 101:
      wave2Amp = velocity2amplitude[value];
      updateMix();
      break;

    case 102:
      noiseAmp = velocity2amplitude[value];
      updateMix();
      break;

    default:
      // if unrecognised do nothing
      break;
  }
}

void setup() {
  Serial.begin(9600);
  AudioMemory(20);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  waveformMod1.begin(WAVEFORM_SINE);
  waveformMod2.begin(WAVEFORM_SAWTOOTH);
  mixer1.gain(0, 1.0);
  mixer1.gain(1, 1.0);
  mixer1.gain(2, 1.0);
  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandleControlChange(OnControlChange);
}

void loop() {
  // put your main code here, to run repeatedly:
  usbMIDI.read();
}
