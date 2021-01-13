#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "LFOSources.h"
#include "Voice.h"
#include "Arpeggiator.h"
#include "ProgChange.h"
#include "ContinuousController.h"
#include "PitchBend.h"
#include "NoteHandling.h"
#include "FilterEffects.h"
#include includeDef
 
// PhatBass: begin automatically generated code

class Synth
{
public:
    const static int NumStoredNotes = 32;
    int clickCount = 0;
    elapsedMillis lastMillis = 0;
    elapsedMillis last_time = 0;
    long encPos = -999;
    int encVal = 0;
    
    // Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
    ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);
    XPT2046_Touchscreen ts(TOUCH_CS);
    Encoder enc(2,3);
    MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);
    
    byte currentNote;
    
    //Arpeggiator arp;
    //ContinuousController CChandler;
    //PitchBend PBhandler;
    //NoteHandling noteHandler;
    //ProgChange progChange;
    const static int NUMVOICES = 8;
    
    LFOSources                       LFOSources;
    Voice                            Voice[8];
    AudioMixer<8>                    MixVoices;
    FilterEffects                    FilterEffects;
    AudioOutputI2S                   i2s1;
    AudioOutputUSB                   usb1;
    Arpeggiator Arp;
    ProgChange ProgHndlr;
    ContinuousController CCHndlr;
    PitchBend PBHndlr;
    AudioControlSGTL5000 sgtl5000_1;
    NoteHandling NoteHndlr;
    AudioConnection                  *patchCord[30]; // total patchCordCount:30 including array typed ones.

    Synth() { // constructor (this is called when class-object is created)
        int pci = 0; // used only for adding new patchcords


        patchCord[pci++] = new AudioConnection(MixVoices, 0, FilterEffects.FilterSelect, 0);
        patchCord[pci++] = new AudioConnection(MixVoices, 0, FilterEffects.filter1, 0);
        patchCord[pci++] = new AudioConnection(FilterEffects.chorus1, 0, i2s1, 0);
        patchCord[pci++] = new AudioConnection(FilterEffects.chorus1, 0, i2s1, 1);
        patchCord[pci++] = new AudioConnection(FilterEffects.chorus1, 0, usb1, 0);
        patchCord[pci++] = new AudioConnection(FilterEffects.chorus1, 0, usb1, 1);
        for (int i = 0; i < 8; i++) {
            patchCord[pci++] = new AudioConnection(LFOSources.LFO1, 0, Voice[i].VCO1, 0);
            patchCord[pci++] = new AudioConnection(LFOSources.LFO2, 0, Voice[i].VCO2, 0);
            patchCord[pci++] = new AudioConnection(Voice[i].ADSR, 0, MixVoices, i);
        }
        
    }

    void dumpPatch() {
      Serial.println("====================================");
      Serial.printf( "OSC1: wave=%s, ampl=%.2f, octave=%d, semis=%u, detune=%.2f\n",
                      waves[osc1Waveform], osc1Amplitude, osc1Octave, osc1Semis, osc1Detune);
      Serial.printf( "LFO1: wave=%s, freq=%.2fHz, depth=%.2f, PWM=%.2f\n\n",
                      waves[lfo1Waveform], lfo1Freq, lfo1Depth, lfo1PWM);
      Serial.printf( "OSC2: wave=%s, ampl=%.2f, octave=%d, semis=%u, detune= %.2f\n",
                      waves[osc2Waveform], osc2Amplitude, osc2Octave, osc2Semis, osc2Detune);
      Serial.printf( "LFO2: wave=%s, freq=%.2fHz, depth=%.2f, PWM=%.2f\n\n",
                      waves[lfo2Waveform], lfo2Freq, lfo2Depth, lfo2PWM);
      Serial.printf( "Filter: band=%s, freq=%.2fHz, res=%.2f, DC=%.2f, modulated=%u\n\n",
                      bands[filtBand], filterFreq, filterRes, filtDC, filtMod);
      Serial.printf( "Mix ADSR: attack=%.2f, decay=%.2f, sustain=%.2f, release=%.2f\n",
                      envAttack, envDecay, envSustain, envRelease);
      Serial.printf( "Filter ADSR: attack=%.2f, decay=%.2f, sustain=%.2f, release=%.2f\n\n",
                      filtEnvA, filtEnvD, filtEnvS, filtEnvR);
      Serial.printf( "Mixer: osc1=%.2f, osc2=%.2f, noise=%.2f, wavetab=%.2f\n\n",
                      osc1Amp, osc2Amp, noiseAmp, waveAmp);
      Serial.printf( " Arpeggiator: arpMode=%s, arpPeriod=%.2f, arpOctave=%u, arpLatch=%u, arpDelay=%.2f, arpTranspose=%d\n",
                      arpModes[arpMode], arpPeriod, arpOctave, arpLatch, arpDelay, arpTranspose);
      Serial.println("====================================");
    }
    
    double mapf(double x, double in_min, double in_max, double out_min, double out_max)
    {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
    
    void update() {
      // put your main code here, to run repeatedly:
      usbMIDI.read();
      MIDI.read()
    
    #ifdef USE_TOUCH
      if (ts.touched()) {
        int X, Y;
        TS_Point p = ts.getPoint();
        X = p.x - 231;
        X = map(X, 3482, 0, 0, 320);
        Y = p.y - 360;
        Y = map(Y, 3465, 0, 0, 240);
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
    
      arp.update();
      if(0) {
        if(last_time >= 5000) {
          Serial.print("Proc = ");
          Serial.print(AudioProcessorUsage());
          Serial.print(" (");    
          Serial.print(AudioProcessorUsageMax());
          Serial.print("),  Mem = ");
          Serial.print(AudioMemoryUsage());
          Serial.print(" (");    
          Serial.print(AudioMemoryUsageMax());
          Serial.println(")");
          last_time = 0;
        }
      }
    }
    
    
};
// PhatBass: end automatically generated code
