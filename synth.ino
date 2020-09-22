#include "NoteData.h"
#include "SPI.h"
#include "ILI9341_t3.h"
#include "font_Arial.h"
#include "font_ArialBold.h"
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

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

extern const unsigned short amelia320[];

#define LED_PIN 13
#define TFT_DC  9
#define TFT_CS 10
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

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);

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
  Serial.printf("so freq = %f\n", freq);
  if (keybd[note - 48].type == 0) {
    whiteKey(keybd[note - 48].offset, true);
  }
  else {
    blackKey(keybd[note - 48].offset, true);
  }
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
  }
  digitalWrite(LED_PIN, LOW);
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
  tft.setCursor(x + 8, y + BAR_HEIGHT + 20);
  tft.print(text);
}

void updateMix(Mix_change_t change) {
  int w1_val, w2_val, noise_val;
  mixer1.gain(0, wave1Amp);  
  mixer1.gain(1, wave2Amp);
  mixer1.gain(2, noiseAmp);
  if ((change == W1) || (change == AllMix)) {
    w1_val = BAR_HEIGHT - mapf(wave1Amp, 0.0, 1.0, 0, BAR_HEIGHT);
    drawBar(MIX_PANEL_X + 5, MIX_PANEL_Y, w1_val, "1");
  }
  if ((change == W2) || (change == AllMix)) {
    w2_val = BAR_HEIGHT - mapf(wave2Amp, 0.0, 1.0, 0, BAR_HEIGHT);
    drawBar(MIX_PANEL_X + 30, MIX_PANEL_Y, w2_val, "2");
  }
  if ((change == Noise) || (change == AllMix)) {
    noise_val = BAR_HEIGHT - mapf(noiseAmp, 0.0, 1.0, 0, BAR_HEIGHT);
    drawBar(MIX_PANEL_X + 55, MIX_PANEL_Y, noise_val, "N");
  }
  Serial.printf("Mixer: Osc1 = %.02f, Osc2 = %.02f, Noise = %.02f, w1 = %u, w2 = %u, noise =%u\n", 
    wave1Amp, wave2Amp, noiseAmp, w1_val, w2_val, noise_val);
}

void updateADSR(ADSR_change_t change) {
  int a_val, d_val, s_val, r_val;
  envelope1.attack(envAttack);
  envelope1.decay(envDecay);
  envelope1.sustain(envSustain);
  envelope1.release(envRelease);
  if ((change == Attack) || (change == AllADSR)) {
    a_val = BAR_HEIGHT - mapf(envAttack, 0, 2000.0, 0, BAR_HEIGHT); 
    drawBar(ADSR_PANEL_X + 5, ADSR_PANEL_Y, a_val, "A");
  }
  if ((change == Decay) || (change == AllADSR)) {
    d_val = BAR_HEIGHT - mapf(envDecay, 0, 2000.0, 0, BAR_HEIGHT); 
    drawBar(ADSR_PANEL_X + 30, ADSR_PANEL_Y, d_val, "D");
  }
  if ((change == Sustain) || (change == AllADSR)) {
    s_val = BAR_HEIGHT - mapf(envSustain, 0, 1.0, 0, BAR_HEIGHT); 
    drawBar(ADSR_PANEL_X + 55, ADSR_PANEL_Y, s_val, "S");
  }
  if ((change == Release) || (change == AllADSR)) {
    r_val = BAR_HEIGHT - mapf(envRelease, 0, 2000.0, 0, BAR_HEIGHT); 
    drawBar(ADSR_PANEL_X + 80, ADSR_PANEL_Y, r_val, "R");
  }
  Serial.printf("ADSR: Attack = %.02fms, Decay = %.02fms, Sustain = %.02f, Release = %.02fms\n", envAttack, envDecay, envSustain, envRelease);
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

    default:
      // if unrecognised do nothing
      break;
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Alive");
  tft.begin();
  tft.setRotation(3);
  // show the pretty kitty
  tft.writeRect(0, 0, 320, 240, amelia320);
  // and give the world a chance to marvel in her glory
  delay(4000);
  // then clear the decks
  tft.fillScreen(ILI9341_LIGHTGREY);
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
  waveformMod1.begin(WAVEFORM_SINE);
  waveformMod1.amplitude(1.0);
  waveformMod2.begin(WAVEFORM_SAWTOOTH);
  waveformMod2.amplitude(1.0);
  pink1.amplitude(1.0);
  updateMix(AllMix);
  updateADSR(AllADSR);
  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandleControlChange(OnControlChange);
}

void loop() {
  // put your main code here, to run repeatedly:
  usbMIDI.read();
  int16_t x, y;
  tft.getCursor(&x, &y);
  if (y > 240) {
    tft.fillRect(0, 20, 320, 220, ILI9341_BLACK);
    tft.setCursor(0, 20);
  }
  //Serial.printf("x = %u, y = %u\n", x, y);
}
