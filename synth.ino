#include <synth_wavetable.h>
#include "types.h"
#include "defines.h"
#include "constdata.h"
#include "globals.h"
#include "NoteData.h"
#include "GMinst.h"
#include "drawing.h"
#include <MIDI.h>
#include <ILI9341_t3.h>
#include <XPT2046_Touchscreen.h>
#include <Encoder.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       LFO1;           //xy=55,75
AudioSynthWaveform       LFO2;           //xy=57,129
AudioAmplifier           LFO1switch;     //xy=187,74
AudioAmplifier           LFO2switch;     //xy=193,128
AudioSynthSimpleDrum     drum1;          //xy=254,230
AudioSynthWavetable      wavetable1;     //xy=265,191
AudioSynthWaveformModulated waveformMod1;   //xy=359,79
AudioSynthWaveformModulated waveformMod2;   //xy=360,122
AudioMixer4              mixer2;         //xy=423,210
AudioSynthNoisePink      pink1;          //xy=492,160
AudioAmplifier           Osc1switch;     //xy=521,79
AudioAmplifier           Osc2switch;     //xy=524,119
AudioSynthWaveformDc     dc1;            //xy=535,306
AudioMixer4              mixer1;         //xy=680,182
AudioEffectEnvelope      envelope2;      //xy=686,305
AudioEffectEnvelope      envelope1;      //xy=825,199
AudioAmplifier           fModSwitch;     //xy=844,303
AudioFilterStateVariable filter1;        //xy=978,246
AudioMixer4              Filter_Select;  //xy=1184,220
AudioEffectChorus        chorus;         //xy=1341,219
AudioOutputI2S           i2s1;           //xy=1513,218
AudioConnection          patchCord1(LFO1, LFO1switch);
AudioConnection          patchCord2(LFO2, LFO2switch);
AudioConnection          patchCord3(LFO1switch, 0, waveformMod1, 0);
AudioConnection          patchCord4(LFO2switch, 0, waveformMod2, 0);
AudioConnection          patchCord5(drum1, 0, mixer2, 1);
AudioConnection          patchCord6(wavetable1, 0, mixer2, 0);
AudioConnection          patchCord7(waveformMod1, Osc1switch);
AudioConnection          patchCord8(waveformMod2, Osc2switch);
AudioConnection          patchCord9(mixer2, 0, mixer1, 3);
AudioConnection          patchCord10(pink1, 0, mixer1, 2);
AudioConnection          patchCord11(Osc1switch, 0, mixer1, 0);
AudioConnection          patchCord12(Osc2switch, 0, mixer1, 1);
AudioConnection          patchCord13(dc1, envelope2);
AudioConnection          patchCord14(mixer1, envelope1);
AudioConnection          patchCord15(envelope2, fModSwitch);
AudioConnection          patchCord16(envelope1, 0, filter1, 0);
AudioConnection          patchCord17(envelope1, 0, Filter_Select, 3);
AudioConnection          patchCord18(fModSwitch, 0, filter1, 1);
AudioConnection          patchCord19(filter1, 0, Filter_Select, 0);
AudioConnection          patchCord20(filter1, 1, Filter_Select, 1);
AudioConnection          patchCord21(filter1, 2, Filter_Select, 2);
AudioConnection          patchCord22(Filter_Select, chorus);
AudioConnection          patchCord23(chorus, 0, i2s1, 0);
AudioConnection          patchCord24(chorus, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=787,94
// GUItool: end automatically generated code

//extern const unsigned short amelia320[];

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

static byte currentNote;

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
    
    for (int i = 1; i < (len - 1); i++) {
      if (arpNotes[i - 1] > arpNotes[i]) {
        byte temp = arpNotes[i - 1];
        arpNotes[i - 1] = arpNotes[i];
        arpNotes[i] = temp;
        newLen = i + 1;
      }
    }
    len = newLen;
  } while(len > 1);
}

void OnNoteOn(byte channel, byte note, byte velocity) {
  Serial.printf("ch: %u, note: %u, vel: %u ", channel, note, velocity);
  digitalWrite(LED_PIN, HIGH);
  if (note >= 48) {
    highlightKey(note, true);
  }
  arpNumDown++;
  if (arpMode == Arp_Off) {
    oscillatorsOn(note);
  }
  else if (arpStoreIndex < 20) {
    Serial.println(); // finish note info logging
    // just consider adding notes to be played to the Arp array

    if ((arpMode == Arp_Scale) && (arpStoreIndex != 0)) {
      // already playing a scale so clear all notes to allow
      // new one to be added
      arpStoreIndex = 0;
      arpPlayIndex = 0;
      arpPlayOctave = 0;
    }

    // usually just add one note to the sequence
    arpNotes[arpStoreIndex] = note;
    Serial.printf("Added note %u at entry %u\n", note, arpStoreIndex);
    arpStoreIndex++;

    // but want a whole scale if in scale mode...
    if (arpMode == Arp_Scale) {
      int currNote = note;
      // but if Scale mode add all the other notes of this scale...
      for (int n = 0; n < scaleModes[arpScaleMode].entries; n++) {
        currNote += scaleModes[arpScaleMode].offsets[n];
        arpNotes[arpStoreIndex++] = currNote;
      }
    }
    if (arpMode != Arp_Record) {
      sortNotes();
    }
    else {
      // in record mode we simply record everything played in order (no sort)
      Serial.printf("Playing %u while recording\n", note);
      oscillatorsOn(note);
    }
  }
}

void OnNoteOff(byte channel, byte note, byte velocity) {
  if (note >= 48) {
    highlightKey(note, false);
  }
  digitalWrite(LED_PIN, LOW);
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
//  Serial.println("Osc off because NoteOff");
  oscillatorsOff();
}

void updateMix(Mix_change_t change) {
  mixer1.gain(0, osc1Amp);  
  mixer1.gain(1, osc2Amp);
  mixer1.gain(2, noiseAmp);
  mixer1.gain(3, waveAmp); // let drums/wavetable through
  mixer2.gain(0, 1.0); // wavetable - always available
  mixer2.gain(1, 1.0); // drums - always available
  updateMixerBars(change);
}


void updateADSR(ADSR_change_t change) {
  envelope1.attack(envAttack);
  envelope1.decay(envDecay);
  envelope1.sustain(envSustain);
  envelope1.release(envRelease);
  updateADSRBars(change);
}

void updateFiltADSR() {
  envelope2.attack(filtEnvA);
  envelope2.decay(filtEnvD);
  envelope2.sustain(filtEnvS);
  envelope2.release(filtEnvR);
  dc1.amplitude(filtDC);
  if (filtMod == 0) {
    fModSwitch.gain(0.0);
  }
  else {
    fModSwitch.gain(1.0);
  }
}

void updateFilterBand() {
  switch(filtBand) {
    case FILT_OFF:
      Filter_Select.gain(0, 0);
      Filter_Select.gain(1, 0);
      Filter_Select.gain(2, 0);
      Filter_Select.gain(3, 1.0);
      break;

    case LPF:
      Filter_Select.gain(0, 1.0);
      Filter_Select.gain(1, 0);
      Filter_Select.gain(2, 0);
      Filter_Select.gain(3, 0);
      break;

    case BPF:
      Filter_Select.gain(0, 0);
      Filter_Select.gain(1, 1.0);
      Filter_Select.gain(2, 0);
      Filter_Select.gain(3, 0);
      break;

    case HPF:
      Filter_Select.gain(0, 0);
      Filter_Select.gain(1, 0);
      Filter_Select.gain(2, 1.0);
      Filter_Select.gain(3, 0);
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
  if (osc1Mod == Mod_FM) {
    //TODO: consider allowing freq mod range to be changed
    waveformMod1.frequencyModulation(12);
  }
  else {
    //TODO: consider allowing phase mod range to be changed
    waveformMod1.phaseModulation(180);
  }
}

void updateOsc2() {
  waveformMod2.begin(osc2Waveform);
  waveformMod2.amplitude(osc2Amplitude);
  if (osc2Mod == Mod_FM) {
    waveformMod2.frequencyModulation(12);
  }
  else {
    waveformMod2.phaseModulation(180);
  }
}

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

void updateNoise() {
  pink1.amplitude(noiseAmplitude);
}

void updateWave() {
  wavetable1.amplitude(waveAmplitude);
  wavetable1.setInstrument(*GMinst[waveInstrument]);
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

void OnControlChange(byte channel, byte control /* CC num*/, byte value /* 0 .. 127 */) {
  int n;
  Serial.printf("CC: %u = %u ==> ", control, value);
  switch(control) {
    case 1:
      Serial.println("Mod wheel");
      break;
    // ====================== MIXER =====================
    case 75:
      osc1Amp =  velocity2amplitude[value];
      Serial.printf("Mix: Osc1 amplitude = %.2f\n", osc1Amp);
      updateMix(OSC1);
      break;

    case 76:
      osc2Amp = velocity2amplitude[value];
      Serial.printf("Mix: Osc2 amplitude = %.2f\n", osc2Amp);
      updateMix(OSC2);
      break;

    case 77:
      waveAmp = velocity2amplitude[value];
      Serial.printf("Mix: Wavetable amplitude = %.2f\n", waveAmp);
      updateMix(WaveTable);
      break;

    case 102:
      noiseAmp = velocity2amplitude[value];
      Serial.printf("Mix: Noise amplitude = %.2f\n", noiseAmp);
      updateMix(Noise);
      break;

    // ======================== Amp ADSR ==========================
    case 103:
      envAttack = mapf(value, 0, 127, 0.0, 3000.0);
      Serial.printf("Attack = %.2f\n", envAttack);
      updateADSR(Attack);
      break;

    case 104:
      envDecay = mapf(value, 0, 127, 0.0, 3000.0);
      Serial.printf("Decay = %.2f\n", envDecay);
      updateADSR(Decay);
      break;

    case 105:
      envSustain = mapf(value, 0, 127, 0.0, 1.0);
      Serial.printf("Sustain = %.2f\n", envSustain);
      updateADSR(Sustain);
      break;

    case 106:
      envRelease = mapf(value, 0, 127, 0.0, 3000.0);
      Serial.printf("Release = %.2f\n", envRelease);
      updateADSR(Release);
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

void onPitchChange(byte channel, int pitch) {
  float change = 0.0;
  Serial.printf("Pitch change: %d\n", pitch);
  if (pitch == 0) {
    osc1PB = 1.0;
    osc2PB = 1.0;
  }
  else {
    // pitch is -8192 to +8192 so convert that to -0.5 to +0.5
    change = (0.5 / 8192) * pitch;
    // so swing from 0.5 to 1.5
    osc1PB = 1.0 + change;
    osc2PB = 1.0 + change;
  }
  Serial.printf("Pitch change: %d so bend = %.05f, osc1PB = %.02f, osc2PB = %.02f\n", pitch, change, osc1PB, osc2PB);
  if (arpNumDown != 0) {
    // something already playing so retrigger with new freq
    oscillatorsOn();
  }
}

void onProgramChange(byte channel, byte program) {
  waveInstrument = program & 0x7F; // currently so limit 0..127
  Serial.printf("Progran change: %u = %s\n", waveInstrument, instrumentNames[waveInstrument]);
  updateWave();
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(JOY_SW, INPUT_PULLUP);
  Serial.begin(9600);
  Serial1.begin(31250);
  Serial.println("The bastard is Alive!");
  MIDI.begin();
  ts.begin();
  ts.setRotation(3);
  tftInit(); 
  AudioMemory(20);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.75);
  updateOsc1();
  updateOsc2();
  updateNoise(); // this probably fixed at 1.0 (mixer varies level)
  updateLFO1();
  updateLFO2();
  updateWave();
  updateMix(AllMix);
  updateADSR(AllADSR);
  updateFiltADSR();
  updateFilterBand();
  updateFilter();
  initChorus();
  updateChorus();
  arpMode = Arp_Off;
  arpLatch = false;
  arpOctave = 0;
  arpPeriod = 1000;
  arpDelay = 0;
  arpTranspose = 0;
  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandleControlChange(OnControlChange);
  usbMIDI.setHandlePitchChange(onPitchChange);
  usbMIDI.setHandleProgramChange(onProgramChange);
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
  if (lastMillis > 100) {
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
    lastMillis = 0;
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
        if (lastMillis > arpPeriod) {
//          Serial.printf("Osc off because lasmillis (%lu) beyond arpPeriod (%u)", lastMillis, arpPeriod);
          oscillatorsOff();
        }
        if (lastMillis > arpDelay) {
          arpDelayActive = false;
          Serial.println("Delay stop");
          lastMillis = 0;
        }
      }
      else if (lastMillis > arpPeriod) {
        byte note2play;
        lastMillis = 0;
        Serial.print("[ ");
        for (int i = 0; i < arpStoreIndex; i++) {
          Serial.print(arpNotes[i]);
          if (i == arpPlayIndex) {
            Serial.print('*');
          }
          Serial.print(' ');
        }
        Serial.printf("] Octave=%u\n", arpPlayOctave);
        note2play = arpNotes[arpPlayIndex] + arpTranspose + (12 * arpPlayOctave);
//        Serial.println("Osc off because arpPeriod expired - cancel last playing note");
        if (arpMode != Arp_Record) {
          oscillatorsOff(); // end previous note
          oscillatorsOn(note2play); // start new note from ARP array
        }
        switch(arpMode) {
          case Arp_Up:
          case Arp_Scale:
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
