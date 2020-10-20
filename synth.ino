#include "NoteData.h"
#include <MIDI.h>
#include <ILI9341_t3.h>
#include <font_Arial.h>
#include <font_ArialBold.h>
#include <XPT2046_Touchscreen.h>
#include <Encoder.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       LFO1;           //xy=413,114
AudioSynthWaveform       LFO2;           //xy=415,168
AudioSynthWaveformDc     dc1;            //xy=461,272
AudioSynthWaveformModulated waveformMod1;   //xy=575,115
AudioSynthWaveformModulated waveformMod2;   //xy=576,158
AudioSynthNoisePink      pink1;          //xy=581,199
AudioEffectEnvelope      envelope2;      //xy=608,272
AudioMixer4              mixer1;         //xy=737,160
AudioAmplifier           amp1;           //xy=742,273
AudioFilterStateVariable filter1;        //xy=877,219
AudioMixer4              Filter_Select;  //xy=1032,222
AudioEffectEnvelope      envelope1;      //xy=1190,222
AudioOutputI2S           i2s1;           //xy=1351,223
AudioConnection          patchCord1(LFO1, 0, waveformMod1, 0);
AudioConnection          patchCord2(LFO2, 0, waveformMod2, 0);
AudioConnection          patchCord3(dc1, envelope2);
AudioConnection          patchCord4(waveformMod1, 0, mixer1, 0);
AudioConnection          patchCord5(waveformMod2, 0, mixer1, 1);
AudioConnection          patchCord6(pink1, 0, mixer1, 2);
AudioConnection          patchCord7(envelope2, amp1);
AudioConnection          patchCord8(mixer1, 0, filter1, 0);
AudioConnection          patchCord9(amp1, 0, filter1, 1);
AudioConnection          patchCord10(filter1, 0, Filter_Select, 0);
AudioConnection          patchCord11(filter1, 1, Filter_Select, 1);
AudioConnection          patchCord12(filter1, 2, Filter_Select, 2);
AudioConnection          patchCord13(Filter_Select, envelope1);
AudioConnection          patchCord14(envelope1, 0, i2s1, 0);
AudioConnection          patchCord15(envelope1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=917,78
// GUItool: end automatically generated code

extern const unsigned short amelia320[];

#define LED_PIN 13
#define TFT_DC  9
#define TFT_CS 10
#define TOUCH_CS 8
#define JOY_SW 2
#define JOY_X 8 // pin 22
#define JOY_Y 5 // pin 19

#define PANEL_H 140
#define BAR_HEIGHT 100
#define BAR_WIDTH 20
#define BAR_OFFSET 15
#define MIX_PANEL_X 10
#define MIX_PANEL_Y 20
#define MIX_PANEL_W 80
#define ADSR_PANEL_X 120
#define ADSR_PANEL_Y 20
#define ADSR_PANEL_W 100
#define KEYB_X  10
#define KEYB_Y  200
#define KEY_WIDTH 12

typedef enum {
  AllMix,
  W1,
  W2,
  Noise
} Mix_change_t;

typedef enum {
  AllADSR,
  Attack,
  Decay,
  Sustain,
  Release
} ADSR_change_t;

typedef enum {
  LPF,
  BPF,
  HPF
} filter_band_t;

typedef enum {
  Arp_Off,
  Arp_Up,
  Arp_Down,
  Arp_UpDown,
  Arp_Random
} arp_mode_t;

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);
XPT2046_Touchscreen ts(TOUCH_CS);
Encoder enc(2,3);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

/*===========================================================
 *  TS calibration musings... points at 4 corners:
 * 
        3740, 3833                     227, 3818
        
        
        3687, 374                      235, 346
        
        3740 - 227 = 3513 to represent 320 X
        3687 - 235 = 3452 to represent 320 X
        Avg = 3482
        
        3833 - 374 = 3459 to represent 240 Y
        3818 - 346 = 3472 to represent 240 Y
        Avg = 3465
        
        X = (X reading - 231) then map(3482, 0, 0, 320)
        Y = (Y reading - 360) then map(3465, 0, 0, 240)
        
============================================================*/

typedef struct {
  uint8_t type;
  float offset;
} keyb_t;

// keys from 48 up - black (1) or white (0)
const keyb_t keybd[] = {
  { 0, 0 },
  { 1, KEY_WIDTH * 0.5 },
  { 0, KEY_WIDTH },
  { 1, KEY_WIDTH * 1.5 },
  { 0, KEY_WIDTH * 2 },
  { 0, KEY_WIDTH * 3 },
  { 1, KEY_WIDTH * 3.5 },
  { 0, KEY_WIDTH * 4 },
  { 1, KEY_WIDTH * 4.5 },
  { 0, KEY_WIDTH * 5 },
  { 1, KEY_WIDTH * 5.5 },
  { 0, KEY_WIDTH * 6 },
  { 0, KEY_WIDTH * 7 },
  { 1, KEY_WIDTH * 7.5 },
  { 0, KEY_WIDTH * 8 },
  { 1, KEY_WIDTH * 8.5 },
  { 0, KEY_WIDTH * 9 },
  { 0, KEY_WIDTH * 10 },
  { 1, KEY_WIDTH * 10.5 },
  { 0, KEY_WIDTH * 11 },
  { 1, KEY_WIDTH * 11.5 },
  { 0, KEY_WIDTH * 12 },
  { 1, KEY_WIDTH * 12.5 },
  { 0, KEY_WIDTH * 13 },
  { 0, KEY_WIDTH * 14 },
  { 1, KEY_WIDTH * 14.5 },
  { 0, KEY_WIDTH * 15 },
  { 1, KEY_WIDTH * 15.5 },
  { 0, KEY_WIDTH * 16 },
  { 0, KEY_WIDTH * 17 },
  { 1, KEY_WIDTH * 17.5 },
  { 0, KEY_WIDTH * 18 },
  { 1, KEY_WIDTH * 18.5 },
  { 0, KEY_WIDTH * 19 },
  { 1, KEY_WIDTH * 19.5 },
  { 0, KEY_WIDTH * 20 }
};

char * waves[] = {
  "Sine",
  "Saw",
  "Square",
  "Triangle",
  "Arbitrary",
  "Pulse",
  "Rev Saw",
  "S/H"
};

char * bands[] = {
  "LPF",
  "BPF",
  "HPF"
};

char * arpModes[] = {
  "Off",
  "Up",
  "Down",
  "Up/Down",
  "Random"
};

int globalNote;

// modulator settings
int osc1Waveform = WAVEFORM_SINE;
float osc1Amplitude = 1.0;
int osc1Octave = 0;
float osc1Detune = 1.0; // can range 1.0 to 0.85

int osc2Waveform = WAVEFORM_SAWTOOTH;
float osc2Amplitude = 1.0;
int osc2Octave = 0;
float osc2Detune = 1.0; // can range 1.0 to 0.85

// LFO settings
int lfo1Waveform = WAVEFORM_SINE;
float lfo1Freq = 13.0;
float lfo1Amplitude = 0.02;
float lfo1PWM = 0.0;

int lfo2Waveform = WAVEFORM_SINE;
float lfo2Freq = 6.0;
float lfo2Amplitude = 0.02;
float lfo2PWM = 0.0;

// noise settings
float noiseAmplitude = 1.0;

// Mixer settings
float osc1Amp = 1.0;
float osc2Amp = 0.4;
float noiseAmp = 0.2;

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
byte arpNotes[8]; // Arpeggiate up to 8 notes
int arpStoreIndex;
int arpPlayIndex;
int arpNumDown;
int arpIncrement;
int arpPlayOctave;

int clickCount = 0;
unsigned long lastMillis = 0;
unsigned long last_time = 0;
long encPos = -999;
int encVal = 0;

void dumpPatch() {
  Serial.println("====================================");
  Serial.printf( "OSC1: wave=%s, ampl=%.2f, octave=%d, detune=%.2f\n",
                  waves[osc1Waveform], osc1Amplitude, osc1Octave, osc1Detune);
  Serial.printf( "LFO1: wave=%s, freq=%.2f, ampl=%.2f, PWM=%.2f\n\n",
                  waves[lfo1Waveform], lfo1Freq, lfo1Amplitude, lfo1PWM);
  Serial.printf( "OSC2: wave=%s, ampl=%.2f, octave=%d, detune= %.2f\n",
                  waves[osc2Waveform], osc2Amplitude, osc2Octave, osc2Detune);
  Serial.printf( "LFO2: wave=%s, freq=%.2f, ampl=%.2f, PWM=%.2f\n\n",
                  waves[lfo2Waveform], lfo2Freq, lfo2Amplitude, lfo2PWM);
  Serial.printf( "Filter: band=%s, freq=%.2f, res=%.2f, DC=%.2f, modulated=%u\n\n",
                  bands[filtBand], filterFreq, filterRes, filtDC, filtMod);
  Serial.printf( "Mix ADSR: attack=%.2f, decay=%.2f, sustain=%.2f, release=%.2f\n",
                  envAttack, envDecay, envSustain, envRelease);
  Serial.printf( "Filter ADSR: attack=%.2f, decay=%.2f, sustain=%.2f, release=%.2f\n\n",
                  filtEnvA, filtEnvD, filtEnvS, filtEnvR);
  Serial.printf( "Mixer: osc1=%.2f, osc2=%.2f, noise=%.2f\n\n",
                  osc1Amp, osc2Amp, noiseAmp);
  Serial.printf( " Arpeggiator: arpMode=%s, arpPeriod=%.2f, arpOctave=%u, arpLatch=%u, arpDelay=%.2f, arpTranspose=%d\n",
                  arpModes[arpMode], arpPeriod, arpOctave, arpLatch, arpDelay, arpTranspose);
  Serial.println("====================================");
}

boolean pointInRect(int x, int y, int rectX, int rectY, int rectW, int rectH) {
  boolean ret = false;
  if ((x >= rectX) && (x <= (rectX + rectW)) && (y >= rectY) && (y <= (rectY + rectH))) {
    ret = true;
  }
  Serial.printf("x=%u y=%u, Rx=%u, Ry=%u, Rw=%u Rh=%u, Rx2=%u, Ry2=%u, ret=%u\n",
            x, y, rectX, rectY, rectW, rectH, rectX+rectW, rectY+rectH, ret);
  return ret;
}

int pointInBar(int x, int y, int barX, int barY) {
  int ret = -1;
  if (pointInRect(x, y, barX, barY + BAR_OFFSET, BAR_WIDTH, BAR_HEIGHT)) {
    ret = map((y - (barY + BAR_OFFSET)), BAR_HEIGHT, 0, 0, 127);
  }
  return ret;
}

double mapf(double x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void whiteKey(int n, bool pressed) {
  if (!pressed) {
    tft.fillRoundRect(KEYB_X + n, KEYB_Y + 20, 10, 20, 2, CL(180, 180, 180));
  }
  else {
    tft.fillRoundRect(KEYB_X + n, KEYB_Y + 20, 10, 20, 2, CL(0, 180, 0));
  }
}

void blackKey(int n, bool pressed) {
  if (!pressed) {
    tft.fillRoundRect(KEYB_X + n, KEYB_Y, 10, 20, 2, ILI9341_BLACK);
  }
  else {
    tft.fillRoundRect(KEYB_X + n, KEYB_Y, 10, 20, 2, CL(0, 180, 0));
  }
}

void oscillatorsOn() {
  byte note;
  note = globalNote;  

  note += osc1Octave; // -24, -12, 0, 12 or 24
  if (note < 0) {
    note = 0;
  }
  if (note > 127) {
    note = 127;
  }
  Serial.printf("Osc1 note=%d ", note);
  float freq = tune_frequencies2_PGM[note];
  freq *= osc1Detune; // mult 0.85 .. 1.0
  Serial.printf("so freq1 = %f, ", freq);
  waveformMod1.frequency(freq);

  note = globalNote;
  note += osc2Octave; // -24, -12, 0, 12 or 24
  if (note < 0) {
    note = 0;
  }
  if (note > 127) {
    note = 127;
  }
  Serial.printf("Osc2 note=%d ", note);
  freq = tune_frequencies2_PGM[note];
  freq *= osc2Detune; // mult 0.85 .. 1.0
  Serial.printf("freq2 = %.2f\n", freq);
  waveformMod2.frequency(freq);
  // sound ADSR
  envelope1.noteOn();
  // filter ADSR
  envelope2.noteOn();
}

void oscillatorsOff() {
  envelope1.noteOff();
  envelope2.noteOff();  
}

void sortNotes() {
/* From Wikipedia 
 *   
procedure bubbleSort(A : list of sortable items)
    n := length(A)
    repeat
        newn := 0
        for i := 1 to n - 1 inclusive do
            if A[i - 1] > A[i] then
                swap(A[i - 1], A[i])
                newn := i
            end if
        end for
        n := newn
    until n ≤ 1
end procedure
*/
  int newLen;
  int len = arpStoreIndex + 1;
#ifdef DEBUG_SORT  
  Serial.print("Input = ");
  for (int i = 0; i < arpStoreIndex; i++) {
    Serial.printf("%d ", arpNotes[i]);
  }
  Serial.println();
#endif  
  do {
    newLen = 0;
#ifdef DEBUG_SORT  
    Serial.printf("i = 1 to %d\n", len - 1);
#endif
    
    for (int i = 1; i < (len - 1); i++) {
      if (arpNotes[i - 1] > arpNotes[i]) {
#ifdef DEBUG_SORT  
        Serial.printf("Swap %d and %d\n", arpNotes[i - 1], arpNotes[i]);
#endif
        byte temp = arpNotes[i - 1];
        arpNotes[i - 1] = arpNotes[i];
        arpNotes[i] = temp;
        newLen = i + 1;
      }
    }
    len = newLen;
#ifdef DEBUG_SORT  
    Serial.printf("Set len to %d\n", newLen);
    for (int i = 0; i < arpStoreIndex; i++) {
      Serial.printf("%d ", arpNotes[i]);
    }
    Serial.println();
#endif
  } while(len > 1);
}

void OnNoteOn(byte channel, byte note, byte velocity) {
  Serial.printf("ch: %u, note: %u, vel: %u ", channel, note, velocity);
  digitalWrite(LED_PIN, HIGH);
  if (keybd[note - 48].type == 0) {
    whiteKey((int)keybd[note - 48].offset, true);
  }
  else {
    blackKey((int)keybd[note - 48].offset, true);
  }
  globalNote = note;
  arpNumDown++;
  if (arpMode == Arp_Off) {
    oscillatorsOn();
  }
  else if (arpStoreIndex < 8) {
    Serial.println(); // finish note info logging
    // just consider adding notes to be played to the Arp array

#if 0
    if (arpNumDown == 1) {
      // start of new group
      for (int i = 0; i < 8; i++) {
        arpNotes[i] = 255;
      }
      arpStoreIndex = 0;
      arpPlayIndex = 0;
    }
#endif

    // now see if we already have this note/key in the array
    bool already = false;
    for (int i = 0; i < arpStoreIndex; i++) {
      if (arpNotes[i] == note) {
        already = true;
      }
    }
    // if not already in the array then add it (and sort into ascending order)
    if (!already) {
      arpNotes[arpStoreIndex] = note;
      arpStoreIndex++;
      sortNotes();
    }
  }
}

void OnNoteOff(byte channel, byte note, byte velocity) {
  if (keybd[note - 48].type == 0) {
    whiteKey(keybd[note - 48].offset, false);
  }
  else {
    blackKey(keybd[note - 48].offset, false);
  }  digitalWrite(LED_PIN, LOW);
  arpNumDown--;
  if (!arpLatch) {
    // remove the off note from the array
    for (int i = 0; i < arpStoreIndex; i++) {
      if (arpNotes[i] == note) {
        // shuffle remaining back 1 to this slot
        for (int j = i + 1; j < arpStoreIndex; j++) {
          arpNotes[j - 1] = arpNotes[j];
        }
        // and reduce number in array.
        arpStoreIndex--;
        break;
      }
    }
  }
  oscillatorsOff();
}

void drawBar(int x, int y, int value, const char * text) {
  // erase existing bar  
  tft.fillRoundRect(x, y + BAR_OFFSET, BAR_WIDTH, BAR_HEIGHT, 5, ILI9341_LIGHTGREY);
  // draw full height outline
  tft.drawRoundRect(x, y + BAR_OFFSET, BAR_WIDTH, BAR_HEIGHT, 5, ILI9341_BLACK);
  // draw filled part rect to show value
  tft.fillRoundRect(x, y + BAR_OFFSET + value, BAR_WIDTH, BAR_HEIGHT - value, 5, ILI9341_BLACK);
  // add a label above
  if (text != NULL) {
    tft.setCursor(x + 8, y + BAR_HEIGHT + 20);
    tft.print(text);
  }
}

void updateMix(Mix_change_t change) {
  int w1_val, w2_val, noise_val;
  mixer1.gain(0, osc1Amp);  
  mixer1.gain(1, osc2Amp);
  mixer1.gain(2, noiseAmp);
  if ((change == W1) || (change == AllMix)) {
    w1_val = BAR_HEIGHT - mapf(osc1Amp, 0.0, 1.0, 0, BAR_HEIGHT);
    drawBar(MIX_PANEL_X + 5, MIX_PANEL_Y, w1_val, (change == AllMix) ? "1" : NULL);
  }
  if ((change == W2) || (change == AllMix)) {
    w2_val = BAR_HEIGHT - mapf(osc2Amp, 0.0, 1.0, 0, BAR_HEIGHT);
    drawBar(MIX_PANEL_X + 30, MIX_PANEL_Y, w2_val, (change == AllMix) ? "2" : NULL);
  }
  if ((change == Noise) || (change == AllMix)) {
    noise_val = BAR_HEIGHT - mapf(noiseAmp, 0.0, 1.0, 0, BAR_HEIGHT);
    drawBar(MIX_PANEL_X + 55, MIX_PANEL_Y, noise_val, (change == AllMix) ? "N" : NULL);
  }
  #if 0
  Serial.printf("Mixer: Osc1 = %.02f, Osc2 = %.02f, Noise = %.02f, w1 = %u, w2 = %u, noise =%u\n", 
    osc1Amp, osc2Amp, noiseAmp, w1_val, w2_val, noise_val);
    #endif
}

void updateADSR(ADSR_change_t change) {
  int a_val, d_val, s_val, r_val;
  envelope1.attack(envAttack);
  envelope1.decay(envDecay);
  envelope1.sustain(envSustain);
  envelope1.release(envRelease);
  if ((change == Attack) || (change == AllADSR)) {
    a_val = BAR_HEIGHT - mapf(envAttack, 0, 2000.0, 0, BAR_HEIGHT);
    drawBar(ADSR_PANEL_X + 5, ADSR_PANEL_Y, a_val, (change == AllADSR) ? "A" : NULL);
  }
  if ((change == Decay) || (change == AllADSR)) {
    d_val = BAR_HEIGHT - mapf(envDecay, 0, 2000.0, 0, BAR_HEIGHT); 
    drawBar(ADSR_PANEL_X + 30, ADSR_PANEL_Y, d_val, (change == AllADSR) ? "D" : NULL);
  }
  if ((change == Sustain) || (change == AllADSR)) {
    s_val = BAR_HEIGHT - mapf(envSustain, 0, 1.0, 0, BAR_HEIGHT); 
    drawBar(ADSR_PANEL_X + 55, ADSR_PANEL_Y, s_val, (change == AllADSR) ? "S" : NULL);
  }
  if ((change == Release) || (change == AllADSR)) {
    r_val = BAR_HEIGHT - mapf(envRelease, 0, 2000.0, 0, BAR_HEIGHT); 
    drawBar(ADSR_PANEL_X + 80, ADSR_PANEL_Y, r_val, (change == AllADSR) ? "R" : NULL);
  }
  //Serial.printf("ADSR: Attack = %.02fms, Decay = %.02fms, Sustain = %.02f, Release = %.02fms\n", envAttack, envDecay, envSustain, envRelease);
}

void updateFiltADSR() {
  envelope2.attack(filtEnvA);
  envelope2.decay(filtEnvD);
  envelope2.sustain(filtEnvS);
  envelope2.release(filtEnvR);
  dc1.amplitude(filtDC);
  if (filtMod == 0) {
    amp1.gain(0.0);
  }
  else {
    amp1.gain(1.0);
  }
}

void updateFilterBand() {
  switch(filtBand) {
    case LPF:
      Filter_Select.gain(0, 1.0);
      Filter_Select.gain(1, 0);
      Filter_Select.gain(2, 0);
      break;

    case BPF:
      Filter_Select.gain(0, 0);
      Filter_Select.gain(1, 1.0);
      Filter_Select.gain(2, 0);
      break;

    case HPF:
      Filter_Select.gain(0, 0);
      Filter_Select.gain(1, 0);
      Filter_Select.gain(2, 1.0);
     break;
  }
}

void updateFilter() {
  filter1.frequency(filterFreq);
  filter1.resonance(filterRes);
}

void updateOsc1() {
  waveformMod1.begin(osc1Waveform);
  waveformMod1.amplitude(osc1Amplitude);
  //waveformMod1.pulseWidth(osc1PWM);
}

void updateOsc2() {
  waveformMod2.begin(osc2Waveform);
  waveformMod2.amplitude(osc2Amplitude);
  //waveformMod2.pulseWidth(osc2PWM);
}

void updateLFO1() {
  LFO1.begin(lfo1Waveform);
  LFO1.frequency(lfo1Freq);
  LFO1.amplitude(lfo1Amplitude);
  LFO1.pulseWidth(lfo1PWM);
}

void updateLFO2() {
  LFO2.begin(lfo2Waveform);
  LFO2.frequency(lfo2Freq);
  LFO2.amplitude(lfo2Amplitude);
  LFO2.pulseWidth(lfo2PWM);
}

void updateNoise() {
  pink1.amplitude(noiseAmplitude);
}

void OnControlChange(byte channel, byte control /* CC num*/, byte value /* 0 .. 127 */) {
  Serial.printf("CC: %u = %u\n", control, value);
  switch(control) {
    // 100, 101, 102 are Mixer controls for OSC1, OSC2 and Noise
    case 100:
      osc1Amp =  velocity2amplitude[value];
      updateMix(W1);
      break;

    case 101:
      osc2Amp = velocity2amplitude[value];
      updateMix(W2);
      break;

    case 102:
      noiseAmp = velocity2amplitude[value];
      updateMix(Noise);
      break;

    // 103, 104, 105, 106 are Envelope Attack, Decay, Sustain, Release
    case 103:
      envAttack = mapf(value, 0, 127, 0.0, 3000.0);
      updateADSR(Attack);
      break;

    case 104:
      envDecay = mapf(value, 0, 127, 0.0, 3000.0);
      updateADSR(Decay);
      break;

    case 105:
      envSustain = mapf(value, 0, 127, 0.0, 1.0);
      updateADSR(Sustain);
      break;

    case 106:
      envRelease = mapf(value, 0, 127, 0.0, 3000.0);
      updateADSR(Release);
      break;

    // 107, 108, 109 are for filter (band, freq, res)
    case 107:
      if (value < 43) {
        filtBand= LPF;
      }
      else if ((value >= 43) && (value < 86)) {
        filtBand= BPF;
      }
      else {
        filtBand = HPF;
      }
      Serial.printf("Filt band = %s\n", (filtBand == LPF) ? "Low" : (filtBand == BPF) ? "Band" : "High");
      break;

    case 108:
      filterFreq = mapf(value, 0, 127, 0, 10000);
      Serial.printf("Filter Freq = %f\n", filterFreq);
      updateFilter();
      break;

    case 109:
      filterRes = mapf(value, 0, 127, 0.7, 5.0);
      updateFilter();
      break;

    // All of Osc1, Osc2, LFO1. LFO2 have similar wave shape selects...
    case 110:
    case 111:
    case 116:
    case 117:
      int change;
      switch (value / 16) {
        case 0:
          change = WAVEFORM_SINE;
          break;
        case 1:
          change = WAVEFORM_SAWTOOTH;
          break;
        case 2:
          change = WAVEFORM_SAWTOOTH_REVERSE;
          break;
        case 3:
          change = WAVEFORM_SQUARE;
          break;
        case 4:
          change = WAVEFORM_TRIANGLE;
          break;
        case 5:
          change = WAVEFORM_SAMPLE_HOLD;
          break;
        case 6:
          change = WAVEFORM_ARBITRARY;
          break;
        case 7:
          change = WAVEFORM_PULSE;
          break;
      }
      Serial.printf("Wave: %s\n", waves[change]);
      if (control == 110) {
        lfo1Waveform = change;
        updateLFO1();
      }
      if (control == 111) {
        lfo2Waveform = change;
        updateLFO2();
      }
      if (control == 116) {
        osc1Waveform = change;
        updateOsc1();
      }
      if (control == 117) {
        osc2Waveform = change;
        updateOsc2();
      }
      break;

    // other LF01 settings
    case 112:
      lfo1Freq = mapf(value, 0, 127, 0.0, 20.0);
      Serial.printf("LFO1 freq = %f\n", lfo2Freq);
      updateLFO1();
      break;

    case 114:
      lfo1Amplitude = mapf(value, 0, 127, 0.0, 1.0);
      updateLFO1();
      break;

    case 20:
      lfo1PWM = mapf(value, 0, 127, 0.0, 1.0);
      updateLFO1();
      break;

    // other LFO2 settings
    case 113:
      lfo2Freq = mapf(value, 0, 127, 0.0, 20.0);
      Serial.printf("LFO2 freq = %f\n", lfo2Freq);
      updateLFO2();
      break;

    case 115:
      lfo2Amplitude = mapf(value, 0, 127, 0.0, 1.0);
      updateLFO2();
      break;

    case 21:
      lfo2PWM = mapf(value, 0, 127, 0.0, 1.0);
      updateLFO2();
      break;

    // Osc1 has wave (above), Ampl, Octave, FineTune
    case 118:
      osc1Amplitude = mapf(value, 0, 127, 0.0, 1.0);
      updateOsc1();
      break;

    case 23:
      if (value < 26) {
        osc1Octave = -24;
      }
      else if ((value >= 26) && (value < 52)) {
        osc1Octave = -12;
      }
      else if ((value >= 52) && (value < 78)) {
        osc1Octave = 0;
      }
      else if ((value >= 78) && (value < 104)) {
        osc1Octave = 12;
      }
      else if (value >= 104) {
        osc1Octave = 24;
      }
      Serial.printf("Osc1 Octave: %d\n", osc1Octave);
      break;

    case 25:
      osc1Detune = mapf(value, 0, 127, 1.0, 0.85);
      Serial.printf("Osc1 detune = %.2f\n", osc1Detune);
      break;

    // Osc2 has wave (above), Freq, Ampl, Octave, FineTune
    case 119:
      osc2Amplitude = mapf(value, 0, 127, 0.0, 1.0);
      updateOsc2();
      break;

    case 26:
      if (value < 26) {
        osc2Octave = -24;
      }
      else if ((value >= 26) && (value < 52)) {
        osc2Octave = -12;
      }
      else if ((value >= 52) && (value < 78)) {
        osc2Octave = 0;
      }
      else if ((value >= 78) && (value < 104)) {
        osc2Octave = 12;
      }
      else if (value >= 104) {
        osc2Octave = 24;
      }
      Serial.printf("Osc2 Octave: %d\n", osc2Octave);
      break;

    case 27:
      osc2Detune = mapf(value, 0, 127, 1.0, 0.85);
      Serial.printf("Osc2 detune = %.2f\n", osc2Detune);
      break;

    // ADSR for filter
    case 22:
      filtEnvA = mapf(value, 0, 127, 0.0, 3000.0);
      updateFiltADSR();
      break;

    case 24:
      filtEnvD = mapf(value, 0, 127, 0.0, 3000.0);
      updateFiltADSR();
      break;

    case 28:
      filtEnvS = mapf(value, 0, 127, 0.0, 1.0);
      updateFiltADSR();
      break;

    case 29:
      filtEnvR = mapf(value, 0, 127, 0.0, 3000.0);
      updateFiltADSR();
      break;

    case 30:
      filtDC = mapf(value, 0, 127, -1.0, 1.0);
      Serial.printf("DC = %f\n", filtDC);
      updateFiltADSR();
      break;

    case 31:
      // use amp as a 0/1 switch for filter DC modulation
      filtMod = (value == 0) ? 0 : 1;
      Serial.printf("Modulation = %d\n", filtMod);
      updateFiltADSR();
      if (filtMod == 0) {
        dumpPatch();
      }
      break;

    case 85:
      if (value < 24) {
        arpMode = Arp_Off;
        oscillatorsOff();
        arpStoreIndex = 0;
        arpPlayIndex = 0;
      }
      else if (value < 48) {
        arpMode = Arp_Up;
        arpPlayIndex = 0;
        arpDelayActive = false;
        arpPlayOctave = 1;
      }
      else if (value < 72) {
        arpMode = Arp_Down;
        arpPlayIndex = 0;
        arpDelayActive = false;
        arpPlayOctave = arpOctave;
      }
      else if (value < 96) {
        arpMode = Arp_UpDown;
        arpPlayIndex = 0;
        arpIncrement = 1; // start "up"
        arpDelayActive = false;
        arpPlayOctave = 1;
      }
      else {
        arpMode = Arp_Random;
      }
      break;

    case 86:
      if (value < 24) {
        arpOctave = 0;
      }
      else if (value < 48) {
        arpOctave = 1;;
      }
      else if (value < 72) {
        arpOctave = 2;
      }
      else if (value < 96) {
        arpOctave = 3;
      }
      break;

    case 87:
      arpLatch = value; // 0 /1
      if (value == 0) {
        arpPlayIndex = 0;
        arpStoreIndex = 0;
        arpPlayOctave = 1;
        oscillatorsOff();
      }
      break;

    case 88:
      arpPeriod = mapf(value, 0, 127, 500, 50); // not sure of units yet
      break;

    case 89:
      arpDelay = mapf(value, 0, 127, 0, 2000);
      break;

    case 90:
      arpTranspose = (int)mapf(value, 0, 127, 0, 11);
      break;

    default:
      // if unrecognised do nothing
      break;
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(JOY_SW, INPUT_PULLUP);
  Serial.begin(9600);
  Serial1.begin(31250);
  Serial.println("Alive");
  MIDI.begin(); 
  tft.begin();
  tft.setRotation(3);
  ts.begin();
  ts.setRotation(3);
  // show the pretty kitty
  tft.writeRect(0, 0, 320, 240, amelia320);
  // and give the world a chance to marvel in her glory
  //delay(4000);
  // then clear the decks  tft.fillScreen(ILI9341_LIGHTGREY);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(40, 0);
  tft.setFont(Arial_18_Bold);
  tft.println("Teensy Synth");
  tft.fillRoundRect(ADSR_PANEL_X, ADSR_PANEL_Y, ADSR_PANEL_W, PANEL_H, 5, ILI9341_DARKGREY);
  tft.setCursor(ADSR_PANEL_X + 30, ADSR_PANEL_Y);
  tft.setFont(Arial_14);
  tft.print("ADSR");
  tft.fillRoundRect(MIX_PANEL_X, MIX_PANEL_Y, MIX_PANEL_W, PANEL_H, 5, ILI9341_DARKGREY);
  tft.setCursor(MIX_PANEL_X + 20, MIX_PANEL_Y);
  tft.print("Mixer");
  tft.setFont(Arial_11_Bold);
  for (int i = 48; i < 84; i++) {
    if (keybd[i - 48].type == 0) {
      whiteKey(keybd[i - 48].offset, false);
    }
    else {
      blackKey(keybd[i - 48].offset, false);
    }
  }
  AudioMemory(20);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  updateOsc1();
  updateOsc2();
  updateNoise(); // this probably fixed at 1.0 (mixer varies level)
  updateLFO1();
  updateLFO2();
  updateMix(AllMix);
  updateADSR(AllADSR);
  updateFiltADSR();
  updateFilterBand();
  updateFilter();
  arpMode = Arp_Off;
  arpLatch = false;
  arpOctave = 0;
  arpPeriod = 1000;
  arpDelay = 0;
  arpTranspose = 0;
  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandleControlChange(OnControlChange);
  //MIDI.setHandleNoteOff(OnNoteOff);
  //MIDI.setHandleNoteOn(OnNoteOn);
  //MIDI.setHandleControlChange(OnControlChange);
}

void loop() {
  // put your main code here, to run repeatedly:
  usbMIDI.read();

#if 0  // testing touch input
  int16_t x, y;
  tft.getCursor(&x, &y);
  if (y > 240) {
    tft.fillRect(0, 20, 320, 220, ILI9341_BLACK);
    tft.setCursor(0, 20);
  }
  //Serial.printf("x = %u, y = %u\n", x, y);
#endif

#ifdef USE_TOUCH
  if (ts.touched()) {
    int X, Y;
    TS_Point p = ts.getPoint();
    X = p.x - 231;
    X = map(X, 3482, 0, 0, 320);
    Y = p.y - 360;
    Y = map(Y, 3465, 0, 0, 240);
#if 0
    tft.fillRect(100, 150, 140, 60, ILI9341_BLACK);
    tft.setTextColor(ILI9341_GREEN);
    tft.setFont(Arial_24);
    tft.setCursor(100, 150);
    tft.print("X = ");
    tft.print(X);
    tft.setCursor(100, 180);
    tft.print("Y = ");
    tft.print(Y);
    tft.fillCircle(X, Y, 4, CL(0, 255, 0));
#endif
    tft.setFont(Arial_14);
    tft.fillRect(240, 5, 100, 30, ILI9341_BLACK);
    tft.setTextColor(ILI9341_RED);
    tft.setCursor(240, 5);
    int Aval;
    Aval = pointInBar(X, Y, ADSR_PANEL_X + 5, ADSR_PANEL_Y);
    if (Aval > 0) {
      tft.print(Aval);
      OnControlChange(1, 103, Aval);
    }
    int Dval;
    Dval = pointInBar(X, Y, ADSR_PANEL_X + 30, ADSR_PANEL_Y);
    if (Dval > 0) {
      tft.print(Dval);
      OnControlChange(1, 104, Dval);
    }
    int Sval;
    Sval = pointInBar(X, Y, ADSR_PANEL_X + 55, ADSR_PANEL_Y);
    if (Sval > 0) {
      tft.print(Sval);
      OnControlChange(1, 105, Sval);
    }
    int Rval;
    Rval = pointInBar(X, Y, ADSR_PANEL_X + 80, ADSR_PANEL_Y);
    if (Rval > 0) {
      tft.print(Rval);
      OnControlChange(1, 106, Rval);
    }
    int O1val;
    O1val = pointInBar(X, Y, MIX_PANEL_X + 5, MIX_PANEL_Y);
    if (O1val > 0) {
      tft.print(O1val);
      OnControlChange(1, 100, O1val);
    }
    int O2val;
    O2val = pointInBar(X, Y, MIX_PANEL_X + 30, MIX_PANEL_Y);
    if (O2val > 0) {
      tft.print(O2val);
      OnControlChange(1, 101, O2val);
    }
    int Nval;
    Nval = pointInBar(X, Y, MIX_PANEL_X + 55, MIX_PANEL_Y);
    if (Nval > 0) {
      tft.print(Nval);
      OnControlChange(1, 102, Nval);
    }
  }
  #endif

  #if 0 // testing joystick input
  if (millis() > (lastMillis + 100)) {
    if (digitalRead(JOY_SW) == 0) {
      clickCount++;
      Serial.printf("Click %u\n", clickCount);
    }
    int joyX, joyY;
    joyX = analogRead(JOY_X);
    joyY = analogRead(JOY_Y);
    #if 1
    tft.fillRect(220, 10, 100, 12, ILI9341_BLACK);
    tft.setCursor(220, 10);
    tft.setTextColor(ILI9341_WHITE);
    tft.setFont(Arial_12);
    tft.printf("x=%u y=%u\r", joyX, joyY);
    #endif
    OnControlChange(1, 103, map(joyX, 0, 1023, 0, 127));
    OnControlChange(1, 104, map(joyY, 0, 1023, 0, 127));
    lastMillis = millis();
  }
  #endif

  #if 0 // testing encoder input
  long newPos;
  newPos = enc.read();
  if (newPos != encPos) {
    if ((newPos > encPos) && (encVal < 127)) {
      encVal++;
    }
    if ((newPos < encPos) && (encVal > 0)) {
      encVal--;
    }
    OnControlChange(1, 105, encVal);
    encPos = newPos;
  }
  #endif

  #if 1 // support 5 pin DIN MIDI in (the attach callbacks do't work??)
  if (MIDI.read()) {                    // Is there a MIDI message incoming ?
    int note, velocity, channel, d1, d2;
    byte type = MIDI.getType();
    switch (type) {
      case midi::NoteOn:
        note = MIDI.getData1();
        velocity = MIDI.getData2();
        channel = MIDI.getChannel();
        #if 0
        if (velocity > 0) {
          Serial.println(String("Note On:  ch=") + channel + ", note=" + note + ", velocity=" + velocity);
        } else {
          Serial.println(String("Note Off: ch=") + channel + ", note=" + note);
        }
        #endif
        OnNoteOn(channel, note, velocity);
        break;
      case midi::NoteOff:
        note = MIDI.getData1();
        velocity = MIDI.getData2();
        channel = MIDI.getChannel();
        #if 0
        Serial.println(String("Note Off: ch=") + channel + ", note=" + note + ", velocity=" + velocity);
        #endif
        OnNoteOff(channel, note, velocity);
        break;
      case midi::ControlChange:
        d1 = MIDI.getData1();
        d2 = MIDI.getData2();
        channel = MIDI.getChannel();
        OnControlChange(channel, d1, d2);
        break;
      default:
        d1 = MIDI.getData1();
        d2 = MIDI.getData2();
        Serial.println(String("Message, type=") + type + ", data = " + d1 + " " + d2);
    }
  }
  #endif
  if (arpMode != Arp_Off) {
    // play Arpeggiator notes
    if (arpStoreIndex != 0) { // any notes in the array to play?
      // are we in a post sequence delay?
      if ((arpDelay != 0) && (arpDelayActive == true)) {
        if (millis() > (lastMillis + arpPeriod)) {
          oscillatorsOff();
        }
        if (millis() > (lastMillis + arpDelay)) {
          arpDelayActive = false;
          Serial.println("Delay stop");
          lastMillis = millis();
        }
      }
      else if (millis() > (lastMillis + arpPeriod)) {
        lastMillis = millis();
        Serial.print("[ ");
        for (int i = 0; i < arpStoreIndex; i++) {
          Serial.print(arpNotes[i]);
          if (i == arpPlayIndex) {
            Serial.print('*');
          }
          Serial.print(' ');
        }
        Serial.printf("] Octave=%u\n", arpPlayOctave);
        globalNote = arpNotes[arpPlayIndex] + arpTranspose + (12 * arpPlayOctave);
        oscillatorsOff(); // end previous note
        oscillatorsOn(); // start new note ("globalNote") from ARP array
        switch(arpMode) {
          case Arp_Up:
            arpPlayIndex++;
            if (arpPlayIndex >= arpStoreIndex) {
              arpPlayIndex = 0;
              if (arpPlayOctave < arpOctave) {
                arpPlayOctave++;
              }
              else {
                arpDelayActive = true;
                arpPlayOctave = 0;
                Serial.println("Up: Delay start");
              }
            }
            break;
          case Arp_Down:
            arpPlayIndex--;
            if (arpPlayIndex < 0) {
              arpPlayIndex = arpStoreIndex - 1;
              if (arpPlayOctave > 0) {
                arpPlayOctave--;
              }
              else {
                arpDelayActive = true;
                arpPlayOctave = arpOctave;
                Serial.println("Dn: Delay start");
              }
            }
            break;
          case Arp_UpDown:
            arpPlayIndex += arpIncrement;
            if (arpPlayIndex >= arpStoreIndex) {
              if (arpPlayOctave < arpOctave) {
                arpPlayOctave++;
                arpPlayIndex = 0;
              }
              else {
                arpPlayIndex--;
                arpIncrement = -arpIncrement;
                arpDelayActive = true;
                Serial.println("Up*: Delay start");
              }
            }
            if (arpPlayIndex < 0) {
              if (arpPlayOctave > 0) {
                arpPlayOctave--;
                arpPlayIndex = arpStoreIndex - 1;
              }
              else {
                arpPlayIndex++;
                arpIncrement = -arpIncrement;
                arpDelayActive = true;
                Serial.println("Dn*: Delay start");
              }
            }
            break;
         case Arp_Random:
            arpPlayIndex = rand() % arpStoreIndex;
            if (arpOctave > 0) {
              arpPlayOctave = rand() % (arpOctave + 1);
            }
            else {
              arpPlayOctave = 0;
            }
            break;
        }
      }
    }
  }
  if(0) {
    if(millis() - last_time >= 5000) {
      Serial.print("Proc = ");
      Serial.print(AudioProcessorUsage());
      Serial.print(" (");    
      Serial.print(AudioProcessorUsageMax());
      Serial.print("),  Mem = ");
      Serial.print(AudioMemoryUsage());
      Serial.print(" (");    
      Serial.print(AudioMemoryUsageMax());
      Serial.println(")");
      last_time = millis();
    }
  }
}
