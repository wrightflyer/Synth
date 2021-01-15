#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>


 
// PhatBass: begin automatically generated code

class ContinuousController
{
public:
    Synth & mSynth;
    
    

    ContinuousController() { // constructor (this is called when class-object is created)

        usbMIDI.setHandleControlChange(OnControlChange);
        MIDI.setHandleControlChange(OnControlChange);
        
    }

    static void OnControlChange(byte channel, byte control /* CC num*/, byte value /* 0 .. 127 */) {
      int i, n;
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
            synth.voice[n].setOsc1Amp(fTemp);
          }
          break;
    
        case 76:
          fTemp = velocity2amplitude[value];
          Serial.printf("Mix: Osc2 amplitude = %.2f\n", fTemp);
          for (i = 0; i < NUM_VOICES; i++) {
            synth.voice[n].setOsc2Amp(fTemp);
          }
          break;
    
        case 77:
          fTemp = velocity2amplitude[value];
          Serial.printf("Mix: Wavetable amplitude = %.2f\n", fTemp);
          for (i = 0; i < NUM_VOICES; i++) {
            synth.voice[n].setWaveAmp(fTemp);
          }
          break;
    
        case 102:
          fTemp = velocity2amplitude[value];
          Serial.printf("Mix: Noise amplitude = %.2f\n", fTemp);
          for (i = 0; i < NUM_VOICES; i++) {
            synth.voice[n].setNoiseAmp(fTemp);
          }
          break;
    
        // ======================== Amp ADSR ==========================
        case 103:
          fTemp = mapf(value, 0, 127, 0.0, 3000.0);
          Serial.printf("Attack = %.2f\n", fTemp);
          for (i = 0; i < NUM_VOICES; i++) {
            synth.voice[n].setADSRAttack(fTemp);
          }
          break;
    
        case 104:
          fTemp = mapf(value, 0, 127, 0.0, 3000.0);
          Serial.printf("Decay = %.2f\n", fTemp);
          for (i = 0; i < NUM_VOICES; i++) {
            synth.voice[n].setADSRDecay(fTemp);
          }
          break;
    
        case 105:
          fTemp = mapf(value, 0, 127, 0.0, 1.0);
          Serial.printf("Sustain = %.2f\n", fTemp);
          for (i = 0; i < NUM_VOICES; i++) {
            synth.voice[n].setADSRSustain(fTemp);
          }
          break;
    
        case 106:
          fTemp = mapf(value, 0, 127, 0.0, 3000.0);
          Serial.printf("Release = %.2f\n", fTemp);
          for (i = 0; i < NUM_VOICES; i++) {
            synth.voice[n].setADSRRelease(fTemp);
          }
          break;
    
        // ========================= FILTER =======================
        case 107:
          value /= 20;
          if (value == 0) {
            filtBand = FILT_OFF;
          }
          else if (value == 1) {
            filtBand= LPF;
          }
          else if (value = 2) {
            filtBand= BPF;
          }
          else {
            filtBand = HPF;
          }
          Serial.printf("Filter band = %s\n", bands[filtBand]);
          updateFilterBand();
          break;
    
        case 108:
          filterFreq = mapf(value, 0, 127, 0, 10000);
          Serial.printf("Filter Freq = %.2fHz\n", filterFreq);
          updateFilter();
          break;
    
        case 109:
          filterRes = mapf(value, 0, 127, 0.7, 5.0);
          Serial.printf("Resonance = %.2f\n", filterRes);
          updateFilter();
          break;
    
        // =================== WAVE SHAPE ======================
        case 110:
        case 111:
        case 116:
        case 117:
          int change;
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
            if (value == 0) {
              LFO1switch.gain(0);          
            }
            else {
              LFO1switch.gain(1.0);
              lfo1Waveform = change;
              updateLFO1();
            }
          }
          if (control == 111) {
            Serial.println("LFO2)");
            if (value == 0) {
              LFO2switch.gain(0);
            }
            else {
              LFO2switch.gain(1.0);
              lfo2Waveform = change;
              updateLFO2();
            }
          }
          if (control == 116) {
            Serial.println("OSC1)");
            if (value == 0) {
              Osc1switch.gain(0);
            }
            else {
              Osc1switch.gain(1.0);
              osc1Waveform = change;
              updateOsc1();
            }
          }
          if (control == 117) {
            Serial.println("OSC2)");
            if (value == 0) {
              Osc2switch.gain(0);
            }
            else {
              Osc2switch.gain(1.0);
              osc2Waveform = change;
              updateOsc2();
            }
          }
          break;
    
        // ===========================  LFO1 =====================
        case 112:
          if (value < 64) {
            lfo1Freq = mapf(value, 0, 64, 0.0, 20.0);        
          }
          else {
            lfo1Freq = mapf(value, 64, 127, 20.0, 200.0);
          }
          Serial.printf("LFO1 freq = %.2fHz\n", lfo1Freq);
          updateLFO1();
          break;
    
        case 114:
          lfo1Depth = mapf(value, 0, 127, 0.0, 0.2);
          Serial.printf("LFO1 depth = %.2f\n", lfo1Depth);
          updateLFO1();
          break;
    
        case 20:
          lfo1PWM = mapf(value, 0, 127, 0.0, 1.0);
          Serial.printf("LFO1 PWM = %.2f\n", lfo1PWM);
          updateLFO1();
          break;
    
        // ===================== LFO2 ========================
        case 113:
          if (value < 64) {
            lfo2Freq = mapf(value, 0, 64, 0.0, 20.0);        
          }
          else {
            lfo2Freq = mapf(value, 64, 127, 20.0, 200.0);
          }
          Serial.printf("LFO2 freq = %.2fHz\n", lfo2Freq);
          updateLFO2();
          break;
    
        case 115:
          lfo2Depth = mapf(value, 0, 127, 0.0, 0.2);
          Serial.printf("LFO2 depth = %.2f\n", lfo2Depth);
          updateLFO2();
          break;
    
        case 21:
          lfo2PWM = mapf(value, 0, 127, 0.0, 1.0);
          Serial.printf("LFO2 PWM = %.2f\n", lfo2PWM);
          updateLFO2();
          break;
    
        // ======================= OSC1 =======================
        case 118:
          osc1Semis = mapf(value, 0.0, 127, 0, 12);
          Serial.printf("Osc1 semis = %u\n", osc1Semis);
          updateOsc1();
          break;
    
        case 23:
          value = (value > 48) ? 48 : value;
          n = (value / 12) * 12;
          osc1Octave = n - 24;
          Serial.printf("Osc1 Octave: %d\n", osc1Octave);
          break;
    
        case 25:
          osc1Detune = mapf(value, 0, 127, 1.0, 0.85);
          Serial.printf("Osc1 detune = %.2f\n", osc1Detune);
          break;
    
        case 14:
          Serial.printf("Osc1 modulation = %s\n", value == 0 ? "FM" : "PM");
          // 0..63 = FM, 64..127 = PM
          if (value < 64) {
            osc1Mod = Mod_FM;
          }
          else {
            osc1Mod = Mod_PM;
          }
          break;
    
        // ======================= OSC2 ============================
        case 119:
          osc2Semis = mapf(value, 0, 127, 0.0, 12);
          Serial.printf("Osc2 semis = %u\n", osc2Semis);
          updateOsc2();
          break;
    
        case 26:
          value = (value > 48) ? 48 : value;
          n = (value / 12) * 12;
          osc2Octave = n - 24;
          Serial.printf("Osc2 Octave: %d\n", osc2Octave);
          break;
    
        case 27:
          osc2Detune = mapf(value, 0, 127, 1.0, 0.85);
          Serial.printf("Osc2 detune = %.2f\n", osc2Detune);
          break;
    
        case 15:
          Serial.printf("Osc2 modulation = %s\n", value == 0 ? "FM" : "PM");
          // 0..63 = FM, 64..127 = PM
          if (value < 64) {
            osc2Mod = Mod_FM;
          }
          else {
            osc2Mod = Mod_PM;
          }
          break;
    
        // ======================== Filter ADSR =======================
        case 22:
          filtEnvA = mapf(value, 0, 127, 0.0, 3000.0);
          Serial.printf("Filter Attack = %.2f\n", filtEnvA);
          updateFiltADSR();
          break;
    
        case 24:
          filtEnvD = mapf(value, 0, 127, 0.0, 3000.0);
          Serial.printf("Filter Decay = %.2f\n", filtEnvD);
          updateFiltADSR();
          break;
    
        case 28:
          filtEnvS = mapf(value, 0, 127, 0.0, 1.0);
          Serial.printf("Filter Sustain = %.2f\n", filtEnvS);
          updateFiltADSR();
          break;
    
        case 29:
          filtEnvR = mapf(value, 0, 127, 0.0, 3000.0);
          Serial.printf("Filter Release = %.2f\n", filtEnvR);
          updateFiltADSR();
          break;
    
        // ====================== FILTER ====================
        case 30:
          filtDC = mapf(value, 0, 127, -1.0, 1.0);
          Serial.printf("DC = %.2f\n", filtDC);
          updateFiltADSR();
          break;
    
        case 31:
          // use amp as a 0/1 switch for filter DC modulation
          filtMod = (value < 64) ? 0 : 1;
          Serial.printf("Modulation = %d\n", filtMod);
          updateFiltADSR();
          if (filtMod == 0) {
            dumpPatch();
          }
          break;
    
        // ===================== ARPEGGIATOR ===================
        case 85:
          value /= 20;
          value *= 20;
          if (value == 0) {
            arpMode = Arp_Off;
            Serial.println("Arp Off - Osc Off");
            oscillatorsOff();
            arpStoreIndex = 0;
            arpPlayIndex = 0;
          }
          else if (value == 20) {
            arpMode = Arp_Up;
            Serial.println("Arp Up");
            arpPlayIndex = 0;
            arpDelayActive = false;
            arpPlayOctave = 0;
          }
          else if (value == 40) {
            arpMode = Arp_Down;
            Serial.println("Arp Down");
            arpPlayIndex = 0;
            arpDelayActive = false;
            arpPlayOctave = arpOctave;
          }
          else if (value == 60) {
            arpMode = Arp_UpDown;
            Serial.println("Arp Up/Down");
            arpPlayIndex = 0;
            arpIncrement = 1; // start "up"
            arpDelayActive = false;
            arpPlayOctave = 0;
          }
          else if (value == 80) {
            arpMode = Arp_Random;
            Serial.println("Arp Random");
          }
          else if (value == 100) {
            Serial.println("Arp Scales");
            arpMode = Arp_Scale;
            arpPlayIndex = 0;
            arpDelayActive = false;
            arpPlayOctave = 0;
          }
          else if (value == 120) {
            Serial.println("Arp Record");
            arpMode = Arp_Record;
            arpStoreIndex = 0;
            arpPlayIndex = 0;
            arpDelayActive = false;
            arpPlayOctave = 0;
          }
          break;
    
        case 86:
          arpOctave = (value / 20);
          Serial.printf("Arp Octaves = %d\n", arpOctave + 1);
          break;
    
        case 87:
          arpLatch = (value >= 64) ? 1 : 0; // 0 /1
          if (value == 0) {
            arpPlayIndex = 0;
            arpStoreIndex = 0;
            arpPlayOctave = 0;
    //        Serial.println("Osc off because latch Off");
            oscillatorsOff();
          }
          Serial.printf("Arp Latch = %u\n", arpLatch);
          break;
    
        case 88:
          arpPeriod = mapf(value, 0, 127, 1200, 50); // not sure of units yet
          Serial.printf("Arp Period = %.2f\n", arpPeriod);
          break;
    
        case 89:
          arpDelay = mapf(value, 0, 127, 0, 2000);
          Serial.printf("Arp Delay = %.2f\n", arpDelay);
          break;
    
        case 90:
          arpTranspose = (int)mapf(value, 0, 127, 0, 11);
          Serial.printf("Arp Transpose = %u\n", arpTranspose);
          break;
    
        case 92:
          arpScaleMode = value;
          Serial.printf("Scale = %s\n", scaleModes[value].name);
          break;
    
        // ===================== CHORUS ====================
        case 91:
          value /= 20;
          value *= 20;
          if (value == 0) {
            chorusVoices = 0;
          }
          else if (value == 20) {
            chorusVoices = 2;
          }
          else if (value == 40) {
            chorusVoices = 4;
          }
          else if (value == 60) {
            chorusVoices = 6;
          }
          Serial.printf("Chorus = %u\n", chorusVoices);
          updateChorus();
          break;
    
        // ========================= PANIC =================
        case 123:
          // MIDI PANIC!!
          arpPlayIndex = 0;
          arpStoreIndex = 0;
          arpPlayOctave = 0;
          arpLatch = 0;
          Serial.println("Osc off because Panic");
          oscillatorsOff();
          break;
    
        default:
          // if unrecognised do nothing
          break;
      }
    }
    
    void setSynth(Synth & refSynth) {
        mSynth = refSynth;
    }
    
    
};
// PhatBass: end automatically generated code
