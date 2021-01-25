#pragma once

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "types.h"



 
// PhatBass: begin automatically generated code

/**
 * Receives MIDI CC messages and directs to the right places
 */
class ContinuousController
{
public:

    ContinuousController() { // constructor (this is called when class-object is created)

        usbMIDI.setHandleControlChange(OnControlChange);
        MIDI.setHandleControlChange(OnControlChange);
        
    }

    public:
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
    
    static void OnControlChange(byte channel, byte control /* CC num*/, byte value /* 0 .. 127 */) {
      int i, n;
      int change;
      float fTemp;
      int iTemp;
      Serial.printf("CC: %u = %u ==> ", control, value);
      switch(control) {
        case 1:
          Serial.println("Mod wheel");
          break;
        // ====================== MIXER =====================
        case 75:
          fTemp =  velocity2amplitude[value];
          Serial.printf("Mix: Osc1 amplitude = %.2f\n", fTemp);
          for (i = 0; i < NUM_VOICES; i++) {
            SynthEngine::voice[i].setOsc1Amp(fTemp);
          }
          break;
    
        case 76:
          fTemp = velocity2amplitude[value];
          Serial.printf("Mix: Osc2 amplitude = %.2f\n", fTemp);
          for (i = 0; i < NUM_VOICES; i++) {
            SynthEngine::voice[i].setOsc2Amp(fTemp);
          }
          break;
    
        case 77:
          fTemp = velocity2amplitude[value];
          Serial.printf("Mix: Wavetable amplitude = %.2f\n", fTemp);
          for (i = 0; i < NUM_VOICES; i++) {
            SynthEngine::voice[i].setWaveAmp(fTemp);
          }
          break;
    
        case 102:
          fTemp = velocity2amplitude[value];
          Serial.printf("Mix: Noise amplitude = %.2f\n", fTemp);
          for (i = 0; i < NUM_VOICES; i++) {
            SynthEngine::voice[i].setNoiseAmp(fTemp);
          }
          break;
    
        // ======================== Amp ADSR ==========================
        case 103:
          fTemp = mapf(value, 0, 127, 0.0, 3000.0);
          Serial.printf("Attack = %.2f\n", fTemp);
          for (i = 0; i < NUM_VOICES; i++) {
            SynthEngine::voice[i].setADSRAttack(fTemp);
          }
          break;
    
        case 104:
          fTemp = mapf(value, 0, 127, 0.0, 3000.0);
          Serial.printf("Decay = %.2f\n", fTemp);
          for (i = 0; i < NUM_VOICES; i++) {
            SynthEngine::voice[i].setADSRDecay(fTemp);
          }
          break;
    
        case 105:
          fTemp = mapf(value, 0, 127, 0.0, 1.0);
          Serial.printf("Sustain = %.2f\n", fTemp);
          for (i = 0; i < NUM_VOICES; i++) {
            SynthEngine::voice[i].setADSRSustain(fTemp);
          }
          break;
    
        case 106:
          fTemp = mapf(value, 0, 127, 0.0, 3000.0);
          Serial.printf("Release = %.2f\n", fTemp);
          for (i = 0; i < NUM_VOICES; i++) {
            SynthEngine::voice[i].setADSRRelease(fTemp);
          }
          break;
    
        // ========================= FILTER =======================
        case 107:
          {
              filter_band_t band;
              value /= 20;
              if (value == 0) {
                band = Filt_FILT_OFF;
              }
              else if (value == 1) {
                band= Filt_LPF;
              }
              else if (value = 2) {
                band= Filt_BPF;
              }
              else {
                band = Filt_HPF;
              }
              Serial.printf("Filter band = %s\n", bands[band]);
              SynthEngine::Filter.setFiltBand(band);
          }
          break;
    
        case 108:
          fTemp = mapf(value, 0, 127, 0, 10000);
          Serial.printf("Filter Freq = %.2fHz\n", fTemp);
          SynthEngine::Filter.setFiltFreq(fTemp);
          break;
    
        case 109:
          fTemp = mapf(value, 0, 127, 0.7, 5.0);
          Serial.printf("Resonance = %.2f\n", fTemp);
          SynthEngine::Filter.setFiltRes(fTemp);
          break;
    
        // =================== WAVE SHAPE ======================
        case 110:
        case 111:
        case 116:
        case 117:
          value /= 10;
          switch (value) {
            case 0:
              // nothing for now as it'll be used to disable source below
              break;
            case 1:
            default:
              change = WAVEFORM_SINE;
              break;
            case 2:
              change = WAVEFORM_SAWTOOTH;
              break;
            case 3:
              change = WAVEFORM_SAWTOOTH_REVERSE;
              break;
            case 4:
              change = WAVEFORM_SQUARE;
              break;
            case 5:
              change = WAVEFORM_TRIANGLE;
              break;
            case 6:
              change = WAVEFORM_SAMPLE_HOLD;
              break;
            case 7:
              change = WAVEFORM_ARBITRARY;
              break;
            case 8:
              change = WAVEFORM_PULSE;
              break;
          }
          Serial.printf("Wave: %s (", (value == 0) ? "Off" : waves[change]);
          if (control == 110) {
            Serial.println("LFO1)");
            SynthEngine::LFOs.setLFO1Waveform(change);
          }
          if (control == 111) {
            Serial.println("LFO2)");
            SynthEngine::LFOs.setLFO2Waveform(change);
          }
          if (control == 116) {
            Serial.println("OSC1)");
            for (i = 0; i < NUM_VOICES; i++) {
                SynthEngine::voice[i].setOsc1Waveform(change);
            }
          }
          if (control == 117) {
            Serial.println("OSC2)");
            for (i = 0; i < NUM_VOICES; i++) {
                SynthEngine::voice[i].setOsc2Waveform(change);
            }
          }
          break;
    
        // ===========================  LFO1 =====================
        case 112:
          if (value < 64) {
            fTemp = mapf(value, 0, 64, 0.0, 20.0);        
          }
          else {
            fTemp = mapf(value, 64, 127, 20.0, 200.0);
          }
          Serial.printf("LFO1 freq = %.2fHz\n", fTemp);
          SynthEngine::LFOs.setLFO1Freq(fTemp);
          break;
    
        case 114:
          fTemp = mapf(value, 0, 127, 0.0, 0.2);
          Serial.printf("LFO1 depth = %.2f\n", fTemp);
          SynthEngine::LFOs.setLFO1Depth(ftemp);
          break;
    
        case 20:
          fTemp = mapf(value, 0, 127, 0.0, 1.0);
          Serial.printf("LFO1 PWM = %.2f\n", fTemp);
          SynthEngine::LFOs.setLFO1PWM(ftemp);
          break;
    
        // ===================== LFO2 ========================
        case 113:
          if (value < 64) {
            fTemp = mapf(value, 0, 64, 0.0, 20.0);        
          }
          else {
            fTemp = mapf(value, 64, 127, 20.0, 200.0);
          }
          Serial.printf("LFO2 freq = %.2fHz\n", fTemp);
          SynthEngine::LFOs.setLFO2Freq(fTemp);
          break;
    
        case 115:
          fTemp = mapf(value, 0, 127, 0.0, 0.2);
          Serial.printf("LFO2 depth = %.2f\n", fTemp);
          SynthEngine::LFOs.setLFO2Depth(ftemp);
          break;
    
        case 21:
          fTemp = mapf(value, 0, 127, 0.0, 1.0);
          Serial.printf("LFO2 PWM = %.2f\n", fTemp);
          SynthEngine::LFOs.setLFO2PWM(ftemp);
          break;
    
        // ======================= OSC1 =======================
        case 118:
            fTemp = mapf(value, 0.0, 127, 0, 12);
            Serial.printf("Osc1 semis = %u\n", fTemp);
            for (i = 0; i < NUM_VOICES; i++) {
                SynthEngine::voice[i].setOsc1Semis(fTemp);
            }
            break;
    
        case 23:
            // -2, -1, 0, +1, +2 octave (-24 to +24)
            value = (value > 48) ? 48 : value;
            n = (value / 12) * 12;
            n -= 24;
            for (i = 0; i < NUM_VOICES; i++) {
                SynthEngine::voice[i].setOsc1Octave(n);
            }
            Serial.printf("Osc1 Octave: %d\n", n);
            break;
    
        case 25:
            fTemp = mapf(value, 0, 127, 1.0, 0.85);
            Serial.printf("Osc1 detune = %.2f\n", fTemp);
            for (i = 0; i < NUM_VOICES; i++) {
                SynthEngine::voice[i].setOsc1Detune(fTemp);
            }
            break;
    
        case 14:
          Serial.printf("Osc1 modulation = %s\n", value == 0 ? "FM" : "PM");
          // 0..63 = FM, 64..127 = PM
          if (value < 64) {
            for (i = 0; i < NUM_VOICES; i++) {
                SynthEngine::voice[i].setOsc1Mod(Mod_FM);
            }
          }
          else {
            for (i = 0; i < NUM_VOICES; i++) {
                SynthEngine::voice[i].setOsc1Mod(Mod_PM);
            }
          }
          break;
    
        // ======================= OSC2 ============================
        case 119:
            fTemp = mapf(value, 0.0, 127, 0, 12);
            Serial.printf("Osc2 semis = %u\n", fTemp);
            for (i = 0; i < NUM_VOICES; i++) {
                SynthEngine::voice[i].setOsc2Semis(fTemp);
            }
            break;
    
        case 26:
            // -2, -1, 0, +1, +2 octave (-24 to +24)
            value = (value > 48) ? 48 : value;
            n = (value / 12) * 12;
            n -= 24;
            for (i = 0; i < NUM_VOICES; i++) {
                SynthEngine::voice[i].setOsc2Octave(n);
            }
            Serial.printf("Osc1 Octave: %d\n", n);
          break;
    
        case 27:
            fTemp = mapf(value, 0, 127, 1.0, 0.85);
            Serial.printf("Osc1 detune = %.2f\n", fTemp);
            for (i = 0; i < NUM_VOICES; i++) {
                mSynth.voice[i].setOsc2Detune(fTemp);
            }
            break;
    
        case 15:
          Serial.printf("Osc2 modulation = %s\n", value == 0 ? "FM" : "PM");
          // 0..63 = FM, 64..127 = PM
          if (value < 64) {
            for (i = 0; i < NUM_VOICES; i++) {
                SynthEngine::voice[i].setOsc2Mod(Mod_FM);
            }
          }
          else {
            for (i = 0; i < NUM_VOICES; i++) {
                SynthEngine::voice[i].setOsc2Mod(Mod_PM);
            }
          }
          break;
    
        // ======================== Filter ADSR =======================
        case 22:
          fTemp = mapf(value, 0, 127, 0.0, 3000.0);
          Serial.printf("Filter Attack = %.2f\n", ftemp);
          SynthEngine::Filter.setFiltAttack(fTemp);
          break;
    
        case 24:
          fTemp = mapf(value, 0, 127, 0.0, 3000.0);
          Serial.printf("Filter Decay = %.2f\n", fTemp);
          SynthEngine::Filter.setFiltDecay(fTemp);
          break;
    
        case 28:
          fTemp = mapf(value, 0, 127, 0.0, 1.0);
          Serial.printf("Filter Sustain = %.2f\n", fTemp);
          SynthEngine::Filter.setFiltSustain(fTemp);
          break;
    
        case 29:
          fTemp = mapf(value, 0, 127, 0.0, 3000.0);
          Serial.printf("Filter Release = %.2f\n", fTemp);
          SynthEngine::Filter.setFiltRelease(fTemp);
          break;
    
        // ====================== FILTER ====================
        case 30:
          fTemp = mapf(value, 0, 127, -1.0, 1.0);
          Serial.printf("DC = %.2f\n", fTemp);
          SynthEngine::Filter.setFiltDC(fTemp);
          break;
    
        case 31:
          // use amp as a 0/1 switch for filter DC modulation
          n = (value < 64) ? 0 : 1;
          Serial.printf("Modulation = %d\n", n);
          SynthEngine::Filter.setFiltModulation(n);
          if (n == 0) {
            dumpPatch();
          }
          break;
    
        // ===================== ARPEGGIATOR ===================
        case 85:
          value /= 20;
          value *= 20;
          Arpeggiator::setArpMode(value);
          break;
    
        case 86:
          Arpeggiator::setArpOctave(value / 20);
          Serial.printf("Arp Octaves = %d\n", (value / 20) + 1);
          break;
    
        case 87:
          n = (value >= 64) ? 1 : 0; // 0 /1
          Arpeggiator::setArpOctave(n);
          Serial.printf("Arp Latch = %u\n", n);
          break;
    
        case 88:
          fTemp = mapf(value, 0, 127, 1200, 50); // not sure of units yet
          Arpeggiator::setArpPeriod(fTemp);
          Serial.printf("Arp Period = %.2f\n", fTemp);
          break;
    
        case 89:
          fTemp = mapf(value, 0, 127, 0, 2000);
          Arpeggiator::setArpDelay(fTemp);
          Serial.printf("Arp Delay = %.2f\n", fTemp);
          break;
    
        case 90:
          n = (int)mapf(value, 0, 127, 0, 11);
          Arpeggiator::setArpTranspose(n);
          Serial.printf("Arp Transpose = %u\n", n);
          break;
    
        case 92:
          Arpeggiator::setArpScaleMode(value);
          Serial.printf("Scale = %s\n", scaleModes[value].name);
          break;
    
        // ===================== CHORUS ====================
        case 91:
          value /= 20;
          value *= 20;
          if (value == 0) {
            n = 0;
          }
          else if (value == 20) {
            n = 2;
          }
          else if (value == 40) {
            n = 4;
          }
          else if (value == 60) {
            n = 6;
          }
          Serial.printf("Chorus = %u\n", n);
          SynthEngine::Filter.setChorusVoices(n);
          updateChorus();
          break;
    
        // ========================= PANIC =================
        case 123:
          // MIDI PANIC!!
          Arpeggiator::setArpMode(0); // off
          Serial.println("Osc off because Panic");
          oscillatorsOff();
          break;
    
        default:
          // if unrecognised do nothing
          break;
      }
    }
    
    
};
// PhatBass: end automatically generated code
