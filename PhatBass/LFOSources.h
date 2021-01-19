#pragma once

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "types.h"

// forward declare
class Synth;


 
// PhatBass: begin automatically generated code

/**
 * All voices share the same two LFOs
 */
class LFOSources
{
public:
    // LFO settings
    private:
    int lfo1Waveform = WAVEFORM_SINE;
    float lfo1Freq = 0.0;
    float lfo1Depth = 0.0;
    float lfo1PWM = 0.0;
    
    int lfo2Waveform = WAVEFORM_SINE;
    float lfo2Freq = 0.0;
    float lfo2Depth = 0.0;
    float lfo2PWM = 0.0;
    
    AudioSynthWaveform               LFO1;
    AudioSynthWaveform               LFO2;
    AudioAmplifier                   LFO1switch;
    AudioAmplifier                   LFO2switch;
    AudioConnection                  *patchCord[2]; // total patchCordCount:2 including array typed ones.

    LFOSources() { // constructor (this is called when class-object is created)
        int pci = 0; // used only for adding new patchcords


        patchCord[pci++] = new AudioConnection(LFO1, 0, LFO1switch, 0);
        patchCord[pci++] = new AudioConnection(LFO2, 0, LFO2switch, 0);
        updateLFO1();
        updateLFO2();
        
    }

    private:
    void updateLFO1() {
      LFO1.begin(lfo1Waveform);
      LFO1.frequency(lfo1Freq);
      LFO1.amplitude(lfo1Depth);
      LFO1.pulseWidth(lfo1PWM);
    }
    
    void updateLFO2() {
      LFO2.begin(lfo2Waveform);
      LFO2.frequency(lfo2Freq);
      LFO2.amplitude(lfo2Depth);
      LFO2.pulseWidth(lfo2PWM);
    }
    
    public:
    void setLFO1Waveform(int wave) {
        lfo1Waveform = wave;
        if (wave == 0) {
            LFO1switch.gain(0.0); 
        }
        else {
            LFO1switch.gain(1.0);
            updateLFO1();
        }
    }
    
    void setLFO2Waveform(int wave) {
        lfo2Waveform = wave;
        if (wave == 0) {
            LFO2switch.gain(0.0);
        }
        else {
            LFO2switch.gain(1.0);
            updateLFO2();
        }
    }
    
    void setLFO1Freq(float freq) {
        lfo1Freq = freq;
        updateLFO1();
    }
    
    void setLFO2Freq(float freq) {
        lfo2Freq = freq;
        updateLFO2();
    }
    
    void setLFO1Depth(float depth) {
        lfo1Depth = depth;
        updateLFO1();
    }
    
    void setLFO2Depth(float depth) {
        lfo2Depth = depth;
        updateLFO2();
    }
    
    void setLFO1PWM(float pwm) {
        lfo1PWM = pwm;
        updateLFO1();
    }
    
    void setLFO2PWM(float pwm) {
        lfo2PWM = pwm;
        updateLFO2();
    }
    
    
};
// PhatBass: end automatically generated code
