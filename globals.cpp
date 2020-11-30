#include "types.h"

// Number of samples in each delay line
// Allocate the delay lines for left and right channels
short delayline[CHORUS_DELAY_LENGTH];

// modulator settings
int osc1Waveform = WAVEFORM_SINE;
float osc1Amplitude = 1.0;
int osc1Octave = 0;
int osc1Semis = 0;
float osc1Detune = 1.0; // can range 1.0 to 0.85
float osc1PB = 1.0;
osc_mod_t osc1Mod = Mod_FM;

int osc2Waveform = WAVEFORM_SAWTOOTH;
float osc2Amplitude = 1.0;
int osc2Octave = 0;
int osc2Semis = 0;
float osc2Detune = 1.0; // can range 1.0 to 0.85
float osc2PB = 1.0;
osc_mod_t osc2Mod = Mod_FM;

// noise settings
float noiseAmplitude = 1.0;

// wavetable
int waveInstrument = 0; // piano
float waveAmplitude = 1.0;

// LFO settings
int lfo1Waveform = WAVEFORM_SINE;
float lfo1Freq = 0.0;
float lfo1Depth = 0.0;
float lfo1PWM = 0.0;

int lfo2Waveform = WAVEFORM_SINE;
float lfo2Freq = 0.0;
float lfo2Depth = 0.0;
float lfo2PWM = 0.0;

// Mixer settings
float osc1Amp = 1.0;
float osc2Amp = 0.0;
float noiseAmp = 0.0;
float waveAmp = 0.0;

// ADSR envelope settings
float envAttack = 10.5;
float envDecay = 35;
float envSustain = 0.5;
float envRelease = 300;

// filter ADSR
float filtEnvA = 10.5;
float filtEnvD = 35;
float filtEnvS = 0.5;
float filtEnvR = 300;

// filter main variables
filter_band_t filtBand = LPF;
float filterFreq = 5000;
float filterRes = 2.5;
float filtDC = 1.0;
int filtMod = 0;

// Arpeggiator
arp_mode_t arpMode;
int arpOctave;
float arpPeriod;
bool arpLatch;
float arpDelay;
bool arpDelayActive;
int arpTranspose;
byte arpNotes[20]; // Arpeggiate up to 20 notes
int arpStoreIndex;
int arpPlayIndex;
int arpNumDown;
int arpIncrement;
int arpPlayOctave;
int arpScaleMode;

int chorusVoices;

int clickCount = 0;
elapsedMillis lastMillis = 0;
elapsedMillis last_time = 0;
long encPos = -999;
int encVal = 0;
