#pragma once

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "types.h"

#include "LFOSources.h"
#include "Voice.h"
#include "FilterEffects.h"
#include "themixer.h"
 
// PhatBass: begin automatically generated code

/**
 * Core engine of synth that ties LFOs, Oscs and Filter
 */
class SynthEngine
{
public:
    private:
    int clickCount = 0;
    elapsedMillis lastMillis = 0;
    elapsedMillis last_time = 0;
    long encPos = -999;
    int encVal = 0;
    
    byte currentNote;
    
    public:
    
    
    LFOSources                       LFOs;
    Voice                            voice[8];
    AudioMixer<8>                    MixVoices;
    FilterEffects                    Filter;
    AudioOutputI2S                   i2s1;
    AudioOutputUSB                   usb1;
    AudioControlSGTL5000 sgtl5000_1;
    AudioConnection                  *patchCord[30]; // total patchCordCount:30 including array typed ones.

    SynthEngine() { // constructor (this is called when class-object is created)
        int pci = 0; // used only for adding new patchcords


        patchCord[pci++] = new AudioConnection(MixVoices, 0, Filter.FilterSelect, 0);
        patchCord[pci++] = new AudioConnection(MixVoices, 0, Filter.filter1, 0);
        patchCord[pci++] = new AudioConnection(Filter.chorus, 0, i2s1, 0);
        patchCord[pci++] = new AudioConnection(Filter.chorus, 0, i2s1, 1);
        patchCord[pci++] = new AudioConnection(Filter.chorus, 0, usb1, 0);
        patchCord[pci++] = new AudioConnection(Filter.chorus, 0, usb1, 1);
        for (int i = 0; i < 8; i++) {
            patchCord[pci++] = new AudioConnection(LFOs.LFO1switch, 0, voice[i].VCO1, 0);
            patchCord[pci++] = new AudioConnection(LFOs.LFO2switch, 0, voice[i].VCO2, 0);
            patchCord[pci++] = new AudioConnection(voice[i].ADSR, 0, MixVoices, i);
        }
        
    }

    static SynthEngine& getInst() {
        static SynthEngine instance;
        return instance;
    }
    static void update() { getInst().Iupdate(); }
    
    void Iupdate() {
      // put your main code here, to run repeatedly:
      usbMIDI.read();
    
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
    
    void oscillatorsOn() {
      oscillatorsOn(currentNote);
    }
    
    void oscillatorsOn(byte playNote) {
      currentNote = playNote; // in case we need to retrigger after recalculating pitch bend
      
      byte note;
      note = playNote;  
    
      note += osc1Octave; // -24, -12, 0, 12 or 24
      note += osc1Semis;
      if (note < 0) {
        note = 0;
      }
      if (note > 127) {
        note = 127;
      }
      Serial.printf("Osc1 note=%d (%s%u) ", note, noteNames[note %12], (note / 12) - 1);
      float freq = tune_frequencies2_PGM[note];
      freq *= osc1Detune; // mult 0.85 .. 1.0
      freq *= osc1PB;
      Serial.printf("so freq1 = %.2fHz, ", freq);
      waveformMod1.frequency(freq);
    
      wavetable1.playFrequency(freq, 127);
    
      note = playNote;
      note += osc2Octave; // -24, -12, 0, 12 or 24
      note += osc2Semis;
      if (note < 0) {
        note = 0;
      }
      if (note > 127) {
        note = 127;
      }
      Serial.printf("Osc2 note=%d (%s%u) ", note, noteNames[note %12], (note / 12) - 1);
      freq = tune_frequencies2_PGM[note];
      freq *= osc2Detune; // mult 0.85 .. 1.0
      freq *= osc2PB;
      Serial.printf("freq2 = %.2fHz\n", freq);
      waveformMod2.frequency(freq);
      // sound ADSR
      envelope1.noteOn();
      // filter ADSR
      envelope2.noteOn();
    //  Serial.println("Both Env triggered");
    }
    
    void oscillatorsOff() {
      envelope1.noteOff();
      envelope2.noteOff();
      wavetable1.stop();
    }
    
    
};
// PhatBass: end automatically generated code
