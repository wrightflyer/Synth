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

class FilterEffects
{
public:
    private:
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
    
    // chorus
    int chorusVoices;
    // Number of samples in each delay line
    // Allocate the delay lines for left and right channels
    short delayline[CHORUS_DELAY_LENGTH];
    
    
    AudioFilterStateVariable         filter1;
    AudioSynthWaveformDc             dc1;
    AudioEffectEnvelope              FilterADSR;
    AudioMixer4                      FilterSelect;
    AudioAmplifier                   ModSwitch;
    AudioEffectChorus                chorus;
    AudioConnection                  *patchCord[7]; // total patchCordCount:7 including array typed ones.

    FilterEffects() { // constructor (this is called when class-object is created)
        int pci = 0; // used only for adding new patchcords


        patchCord[pci++] = new AudioConnection(filter1, 0, FilterSelect, 1);
        patchCord[pci++] = new AudioConnection(filter1, 1, FilterSelect, 2);
        patchCord[pci++] = new AudioConnection(filter1, 2, FilterSelect, 3);
        patchCord[pci++] = new AudioConnection(dc1, 0, FilterADSR, 0);
        patchCord[pci++] = new AudioConnection(FilterADSR, 0, ModSwitch, 0);
        patchCord[pci++] = new AudioConnection(FilterSelect, 0, chorus, 0);
        patchCord[pci++] = new AudioConnection(ModSwitch, 0, filter1, 1);
        updateFiltADSR();
        updateFilterBand();
        updateFilter();
        initChorus();
        updateChorus();
        
    }

    private:
    void updateFiltADSR() {
      FilterADSR.attack(filtEnvA);
      FilterADSR.decay(filtEnvD);
      FilterADSR.sustain(filtEnvS);
      FilterADSR.release(filtEnvR);
      dc1.amplitude(filtDC);
      if (filtMod == 0) {
        ModSwitch.gain(0.0);
      }
      else {
        ModSwitch.gain(1.0);
      }
    }
    
    void updateFilterBand() {
      switch(filtBand) {
        case FILT_OFF:
          FilterSelect.gain(0, 1.00);
          FilterSelect.gain(1, 0);
          FilterSelect.gain(2, 0);
          FilterSelect.gain(3, 0);
          break;
    
        case LPF:
          FilterSelect.gain(0, 0);
          FilterSelect.gain(1, 1.0);
          FilterSelect.gain(2, 0);
          FilterSelect.gain(3, 0);
          break;
    
        case BPF:
          FilterSelect.gain(0, 0);
          FilterSelect.gain(1, 0);
          FilterSelect.gain(2, 1.0);
          FilterSelect.gain(3, 0);
          break;
    
        case HPF:
          FilterSelect.gain(0, 0);
          FilterSelect.gain(1, 0);
          FilterSelect.gain(2, 0);
          FilterSelect.gain(3, 1.0);
         break;
      }
    }
    
    void updateFilter() {
      filter1.frequency(filterFreq);
      filter1.resonance(filterRes);
    }
    
    void initChorus() {
      chorusVoices = 0; // off by default
    
      // Initialize the effect
      // address of delayline
      // total number of samples in the delay line
      // number of voices in the chorus INCLUDING the original voice
      if(!chorus.begin(delayline, CHORUS_DELAY_LENGTH, 6)) {
        Serial.println("AudioEffectChorus - left channel begin failed");
        while(1);
      }
    }
    
    void updateChorus() {
      chorus.voices(chorusVoices);
    }
    
    public:
    void setFiltAttack(float attack) {
      filtEnvA = attack;
      updateFiltADSR();
    }
    
    void setFiltDecay(float decay) {
      filtEnvD = decay;
      updateFiltADSR();
    }
    
    void setFiltSustain(float sus) {
      filtEnvS = sus;
      updateFiltADSR();
    }
    
    void setFiltRelease(float release) {
      filtEnvR = release;
      updateFiltADSR();
    }
    
    void setFiltDC(float dc) {
      filtDC = dc;
      updateFiltADSR();
    }
    
    void setFiltModulation(int mod) {
      filtMod = mod;
      updateFiltADSR();
    }
    
    void setFiltBand(filt_band_t band) {
      filtBand = band;
      updateFilterBand();
    }
    
    void setFiltFreq(float freq) {
      filterFreq = freq;
      updateFilter();
    }
    
    void setFiltRes(float res) {
      filterRes = res;
      updateFilter();
    }
    
    void setChorus(int voices) {
        chorusVoices = voices;
        updateChorus();
    }
    
};
// PhatBass: end automatically generated code
