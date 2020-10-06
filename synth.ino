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
AudioSynthWaveform       LFO1;      //xy=67.77777099609375,203.3333225250244
AudioSynthWaveform       LFO2;      //xy=69.99999321831592,257.77776696946887
AudioSynthWaveformModulated waveformMod1;   //xy=229.77773666381836,204.11112785339355
AudioSynthWaveformModulated waveformMod2;   //xy=230.33329391479492,247.1111297607422
AudioSynthNoisePink      pink1;          //xy=235.77773666381836,288.33335304260254
AudioMixer4              mixer1;         //xy=391.3332939147949,249.1111297607422
AudioFilterStateVariable filter1;        //xy=527.7777442932129,252.3333396911621
AudioMixer4              Filter_Select;         //xy=682.3333015441895,255.88888931274414
AudioEffectEnvelope      envelope1;      //xy=840.1110305786133,255.44447135925293
AudioOutputI2S           i2s1;           //xy=1001.1110305786133,256.44447135925293
AudioConnection          patchCord1(LFO1, 0, waveformMod1, 0);
AudioConnection          patchCord2(LFO2, 0, waveformMod2, 0);
AudioConnection          patchCord3(waveformMod1, 0, mixer1, 0);
AudioConnection          patchCord4(waveformMod2, 0, mixer1, 1);
AudioConnection          patchCord5(pink1, 0, mixer1, 2);
AudioConnection          patchCord6(mixer1, 0, filter1, 0);
AudioConnection          patchCord7(filter1, 0, Filter_Select, 0);
AudioConnection          patchCord8(filter1, 1, Filter_Select, 1);
AudioConnection          patchCord9(filter1, 2, Filter_Select, 2);
AudioConnection          patchCord10(Filter_Select, envelope1);
AudioConnection          patchCord11(envelope1, 0, i2s1, 0);
AudioConnection          patchCord12(envelope1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=571.444393157959,167.11111640930176
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
  uint16_t offset;
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

// modulator settings
int osc1Waveform = WAVEFORM_SINE;
float osc1Amplitude = 1.0;
int osc2Waveform = WAVEFORM_SAWTOOTH;
float osc2Amplitude = 1.0;

// LFO settings
int lfo1Waveform = WAVEFORM_SINE;
float lfo1Freq = 13.0;
float lfo1Amplitude = 0.02;
int lfo2Waveform = WAVEFORM_SINE;
float lfo2Freq = 6.0;
float lfo2Amplitude = 0.02;

// noise settings
float noiseAmplitude = 1.0;

// Mixer settings
float wave1Amp = 1.0;
float wave2Amp = 0.4;
float noiseAmp = 0.2;

// ADSR envelope settings
float envAttack = 10.5;
float envDecay = 35;
float envSustain = 0.5;
float envRelease = 300;

filter_band_t filtBand = LPF;
float filterFreq = 5000;
float filterRes = 2.5;

int clickCount = 0;
unsigned long lastMillis = 0;
long encPos = -999;
int encVal = 0;

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

void OnNoteOn(byte channel, byte note, byte velocity) {
  Serial.printf("ch: %u, note: %u, vel: %u ", channel, note, velocity);
  digitalWrite(LED_PIN, HIGH);
  float freq = tune_frequencies2_PGM[note];
  if (keybd[note - 48].type == 0) {
    whiteKey(keybd[note - 48].offset, true);
  }
  else {
    blackKey(keybd[note - 48].offset, true);
  }  Serial.printf("so freq = %f\n", freq);
  waveformMod1.frequency(freq);
  if (note > 12) {
    note -= 10;
    freq = tune_frequencies2_PGM[note];
  }
  waveformMod2.frequency(freq);
  envelope1.noteOn();
}

void OnNoteOff(byte channel, byte note, byte velocity) {
  if (keybd[note - 48].type == 0) {
    whiteKey(keybd[note - 48].offset, false);
  }
  else {
    blackKey(keybd[note - 48].offset, false);
  }  digitalWrite(LED_PIN, LOW);
  envelope1.noteOff();
}

void drawBar(int x, int y, int value, char * text) {
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
  mixer1.gain(0, wave1Amp);  
  mixer1.gain(1, wave2Amp);
  mixer1.gain(2, noiseAmp);
  if ((change == W1) || (change == AllMix)) {
    w1_val = BAR_HEIGHT - mapf(wave1Amp, 0.0, 1.0, 0, BAR_HEIGHT);
    drawBar(MIX_PANEL_X + 5, MIX_PANEL_Y, w1_val, (change == AllMix) ? "1" : NULL);
  }
  if ((change == W2) || (change == AllMix)) {
    w2_val = BAR_HEIGHT - mapf(wave2Amp, 0.0, 1.0, 0, BAR_HEIGHT);
    drawBar(MIX_PANEL_X + 30, MIX_PANEL_Y, w2_val, (change == AllMix) ? "2" : NULL);
  }
  if ((change == Noise) || (change == AllMix)) {
    noise_val = BAR_HEIGHT - mapf(noiseAmp, 0.0, 1.0, 0, BAR_HEIGHT);
    drawBar(MIX_PANEL_X + 55, MIX_PANEL_Y, noise_val, (change == AllMix) ? "N" : NULL);
  }
  #if 0
  Serial.printf("Mixer: Osc1 = %.02f, Osc2 = %.02f, Noise = %.02f, w1 = %u, w2 = %u, noise =%u\n", 
    wave1Amp, wave2Amp, noiseAmp, w1_val, w2_val, noise_val);
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
}

void updateOsc2() {
  waveformMod2.begin(osc2Waveform);
  waveformMod2.amplitude(osc2Amplitude);
}

void updateLFO1() {
  LFO1.begin(lfo1Waveform);
  LFO1.frequency(lfo1Freq);
  LFO1.amplitude(lfo1Amplitude);
}

void updateLFO2() {
  LFO1.begin(lfo2Waveform);
  LFO1.frequency(lfo2Freq);
  LFO1.amplitude(lfo2Amplitude);
}

void updateNoise() {
  pink1.amplitude(noiseAmplitude);
}

void OnControlChange(byte channel, byte control /* CC num*/, byte value /* 0 .. 127 */) {
  switch(control) {
    // 100, 101, 102 are OSC1, OSC2 and Noise mixers
    case 100:
      wave1Amp =  velocity2amplitude[value];
      updateMix(W1);
      break;

    case 101:
      wave2Amp = velocity2amplitude[value];
      updateMix(W2);
      break;

    case 102:
      noiseAmp = velocity2amplitude[value];
      updateMix(Noise);
      break;

    // 103, 104, 105, 106 are Envelope Attack, Decay, Sustain, Release
    case 103:
      envAttack = mapf(value, 0, 127, 0.0, 2000.0);
      updateADSR(Attack);
      break;

    case 104:
      envDecay = mapf(value, 0, 127, 0.0, 2000.0);
      updateADSR(Decay);
      break;

    case 105:
      envSustain = mapf(value, 0, 127, 0.0, 1.0);
      updateADSR(Sustain);
      break;

    case 106:
      envRelease = mapf(value, 0, 127, 0.0, 2000.0);
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
      updateFilterBand();
      break;

    case 108:
      filterFreq = mapf(value, 0, 127, 0, 10000);
      updateFilter();
      break;

    case 109:
      filterRes = mapf(value, 0, 127, 0.7, 5.0);
      updateFilter();
      break;

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
          change = WAVEFORM_PULSE;
          break;
        case 6:
          change = WAVEFORM_SAMPLE_HOLD;
          break;
        case 7:
          change = WAVEFORM_ARBITRARY;
          break;
      }
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

    case 112:
      lfo1Freq = mapf(value, 0, 127, 0.0, 20.0);
      updateLFO1();
      break;

    case 113:
      lfo2Freq = mapf(value, 0, 127, 0.0, 20.0);
      updateLFO2();
      break;

    case 114:
      lfo1Amplitude = mapf(value, 0, 127, 0.0, 1.0);
      updateLFO1();
      break;

    case 115:
      lfo2Amplitude = mapf(value, 0, 127, 0.0, 1.0);
      updateLFO2();
      break;

    case 118:
      osc1Amplitude = mapf(value, 0, 127, 0.0, 1.0);
      updateOsc1();
      break;

    case 119:
      osc2Amplitude = mapf(value, 0, 127, 0.0, 1.0);
      updateOsc2();
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
  delay(4000);
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
  updateFilterBand();
  updateFilter();
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
#if 0  
  int16_t x, y;
  tft.getCursor(&x, &y);
  if (y > 240) {
    tft.fillRect(0, 20, 320, 220, ILI9341_BLACK);
    tft.setCursor(0, 20);
  }
  //Serial.printf("x = %u, y = %u\n", x, y);
#endif
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
  #if 0
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
  #if 0
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
  #if 1
  if (MIDI.read()) {                    // Is there a MIDI message incoming ?
    int note, velocity, channel, d1, d2;
    byte type = MIDI.getType();
    switch (type) {
      case midi::NoteOn:
        note = MIDI.getData1();
        velocity = MIDI.getData2();
        channel = MIDI.getChannel();
        if (velocity > 0) {
          Serial.println(String("Note On:  ch=") + channel + ", note=" + note + ", velocity=" + velocity);
        } else {
          Serial.println(String("Note Off: ch=") + channel + ", note=" + note);
        }
        break;
      case midi::NoteOff:
        note = MIDI.getData1();
        velocity = MIDI.getData2();
        channel = MIDI.getChannel();
        Serial.println(String("Note Off: ch=") + channel + ", note=" + note + ", velocity=" + velocity);
        break;
      default:
        d1 = MIDI.getData1();
        d2 = MIDI.getData2();
        Serial.println(String("Message, type=") + type + ", data = " + d1 + " " + d2);
    }
  }
  #endif
}
