#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "NoteData.h"

#define LED_PIN 13

// GUItool: begin automatically generated code
AudioSynthWaveformModulated waveformMod1;   //xy=152,165
AudioSynthWaveformModulated waveformMod2;   //xy=157,238
AudioSynthNoisePink      pink1;          //xy=168,317
AudioMixer4              mixer1;         //xy=335,242
AudioEffectEnvelope      envelope1;      //xy=519,242
AudioOutputI2S           i2s1;           //xy=738,240
AudioConnection          patchCord1(waveformMod1, 0, mixer1, 0);
AudioConnection          patchCord2(waveformMod2, 0, mixer1, 1);
AudioConnection          patchCord3(pink1, 0, mixer1, 2);
AudioConnection          patchCord4(mixer1, envelope1);
AudioConnection          patchCord5(envelope1, 0, i2s1, 0);
AudioConnection          patchCord6(envelope1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=399,20
// GUItool: end automatically generated code

// Mixer settings
float wave1Amp = 1.0;
float wave2Amp = 0.4;
float noiseAmp = 0.2;

// ADSR envelope settings
float envAttack = 10.5;
float envDecay = 35;
float envSustain = 0.5;
float envRelease = 300;

double mapf(double x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void OnNoteOn(byte channel, byte note, byte velocity) {
  Serial.printf("ch: %u, note: %u, vel: %u\n", channel, note, velocity);
  digitalWrite(LED_PIN, HIGH);
  float freq = tune_frequencies2_PGM[note];
  waveformMod1.frequency(freq);
  if (note > 12) {
    note -= 10;
    freq = tune_frequencies2_PGM[note];
  }
  waveformMod2.frequency(freq);
  envelope1.noteOn();
}

void OnNoteOff(byte channel, byte note, byte velocity) {
  Serial.println("and off");
  digitalWrite(LED_PIN, LOW);
  envelope1.noteOff();
}

void updateMix() {
  mixer1.gain(0, wave1Amp);  
  mixer1.gain(1, wave2Amp);
  mixer1.gain(2, noiseAmp);
  Serial.printf("Mixer: Osc1 = %f, Osc2 = %f, Noise = %f\n", wave1Amp, wave2Amp, noiseAmp);
}

void updateADSR() {
  envelope1.attack(envAttack);
  envelope1.decay(envDecay);
  envelope1.sustain(envSustain);
  envelope1.release(envRelease);
  Serial.printf("ADSR: Attack = %fms, Decay = %fms, Sustain = %f, Release = %fms\n", envAttack, envDecay, envSustain, envRelease);
}

void OnControlChange(byte channel, byte control /* CC num*/, byte value /* 0 .. 127 */) {
  Serial.printf("ch: %u, cc: %u, val: %u\n", channel, control, value);
  switch(control) {
    // 100, 101, 102 are OSC1, OSC2 and Noise mixers
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

    // 103, 104, 105, 106 are Envelope Attack, Decay, Sustain, Release
    case 103:
      envAttack = mapf(value, 0, 127, 0.0, 2000.0);
      updateADSR();
      break;

    case 104:
      envDecay = mapf(value, 0, 127, 0.0, 2000.0);
      updateADSR();
      break;

    case 105:
      envSustain = mapf(value, 0, 127, 0.0, 1.0);
      updateADSR();
      break;

    case 106:
      envRelease = mapf(value, 0, 127, 0.0, 2000.0);
      updateADSR();
      break;

    default:
      // if unrecognised do nothing
      break;
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Alive");
  AudioMemory(20);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  waveformMod1.begin(WAVEFORM_SINE);
  waveformMod1.amplitude(1.0);
  waveformMod2.begin(WAVEFORM_SAWTOOTH);
  waveformMod2.amplitude(1.0);
  pink1.amplitude(1.0);
  updateMix();
  updateADSR();
  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandleControlChange(OnControlChange);
}

void loop() {
  // put your main code here, to run repeatedly:
  usbMIDI.read();
}
