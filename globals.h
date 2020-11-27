#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "types.h"
extern ILI9341_t3 tft;

extern short delayline[];

extern int globalNote;

// modulator settings
extern int osc1Waveform;
extern float osc1Amplitude;
extern int osc1Octave;
extern int osc1Semis;
extern float osc1Detune; // can range 1.0 to 0.85
extern float osc1PB;
extern osc_mod_t osc1Mod;

extern int osc2Waveform;
extern float osc2Amplitude;
extern int osc2Octave;
extern int osc2Semis;
extern float osc2Detune; // can range 1.0 to 0.85
extern float osc2PB;
extern osc_mod_t osc2Mod;

// noise settings
extern float noiseAmplitude;

// wavetable
extern int waveInstrument; // piano
extern float waveAmplitude;

// LFO settings
extern int lfo1Waveform;
extern float lfo1Freq;
extern float lfo1Depth;
extern float lfo1PWM;

extern int lfo2Waveform;
extern float lfo2Freq;
extern float lfo2Depth;
extern float lfo2PWM;

// Mixer settings
extern float osc1Amp;
extern float osc2Amp;
extern float noiseAmp;
extern float waveAmp;

// ADSR envelope settings
extern float envAttack;
extern float envDecay;
extern float envSustain;
extern float envRelease;

// filter ADSR
extern float filtEnvA;
extern float filtEnvD;
extern float filtEnvS;
extern float filtEnvR;

// filter main variables
extern filter_band_t filtBand;
extern float filterFreq;
extern float filterRes;
extern float filtDC;
extern int filtMod;

// Arpeggiator
extern arp_mode_t arpMode;
extern int arpOctave;
extern float arpPeriod;
extern bool arpLatch;
extern float arpDelay;
extern bool arpDelayActive;
extern int arpTranspose;
extern byte arpNotes[20]; // Arpeggiate up to 20 notes
extern int arpStoreIndex;
extern int arpPlayIndex;
extern int arpNumDown;
extern int arpIncrement;
extern int arpPlayOctave;
extern int arpScaleMode;

extern int chorusVoices;

extern int clickCount;
extern unsigned long lastMillis;
extern unsigned long last_time;
extern long encPos;
extern int encVal;
#endif
