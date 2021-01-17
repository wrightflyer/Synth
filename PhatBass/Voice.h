#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>


 
// PhatBass: begin automatically generated code

/**
 * A voice has two Osc, Noise and Wavetable. 
 * Each Osc are modulated by two external LFO.
 * Each voice has it's own ADSR so can be at a unique position in start/stop playing.
 */
class Voice
{
public:
    private:
    // modulator settings
    int osc1Waveform = WAVEFORM_SINE;
    float osc1Amplitude = 1.0;
    int osc1Octave = 0;
    int osc1Semis = 0;
    float osc1Detune = 1.0; // can range 1.0 to 0.85
    float osc1PB = 1.0;
    osc_mod_t osc1Mod = Mod_FM;
    
    int osc2Waveform = WAVEFORM_SAWTOOTH;
    float osc2Amplitude = 1.0;
    int osc2Octave = 0;
    int osc2Semis = 0;
    float osc2Detune = 1.0; // can range 1.0 to 0.85
    float osc2PB = 1.0;
    osc_mod_t osc2Mod = Mod_FM;
    
    // noise settings
    float noiseAmplitude = 1.0;
    
    // wavetable
    int waveInstrument = 0; // piano
    float waveAmplitude = 1.0;
    
    // Mixer settings
    float osc1Amp = 1.0;
    float osc2Amp = 0.0;
    float noiseAmp = 0.0;
    float waveAmp = 0.0;
    
    // ADSR envelope settings
    float envAttack = 10.5;
    float envDecay = 35;
    float envSustain = 0.5;
    float envRelease = 300;
    
    AudioSynthWaveformModulated      VCO1;
    AudioSynthWaveformModulated      VCO2;
    AudioSynthWavetable              WaveTable;
    AudioSynthNoisePink              pinkNoise;
    AudioMixer4                      VoiceMixer;
    AudioEffectEnvelope              ADSR;
    AudioConnection                  *patchCord[5]; // total patchCordCount:5 including array typed ones.

    Voice() { // constructor (this is called when class-object is created)
        int pci = 0; // used only for adding new patchcords


        patchCord[pci++] = new AudioConnection(VCO1, 0, VoiceMixer, 0);
        patchCord[pci++] = new AudioConnection(VCO2, 0, VoiceMixer, 1);
        patchCord[pci++] = new AudioConnection(WaveTable, 0, VoiceMixer, 2);
        patchCord[pci++] = new AudioConnection(pinkNoise, 0, VoiceMixer, 3);
        patchCord[pci++] = new AudioConnection(VoiceMixer, 0, ADSR, 0);
        updateOsc1();
        updateOsc2();
        updateNoise();
        updateMix(AllMix);
        updateADSR(AllADSR);
        
    }

    private:
    void updateMix(Mix_change_t change) {
      VoiceMixer.gain(0, osc1Amp);  
      VoiceMixer.gain(1, osc2Amp);
      VoiceMixer.gain(2, waveAmp);
      VoiceMixer.gain(3, noiseAmp);
      updateMixerBars(change);
    }
    
    
    void updateADSR(ADSR_change_t change) {
      ADSR.attack(envAttack);
      ADSR.decay(envDecay);
      ADSR.sustain(envSustain);
      ADSR.release(envRelease);
      updateADSRBars(change);
    }
    
    void updateOsc1() {
      VCO1.begin(osc1Waveform);
      VCO1.amplitude(osc1Amplitude);
      if (osc1Mod == Mod_FM) {
        //TODO: consider allowing freq mod range to be changed
        VCO1.frequencyModulation(12);
      }
      else {
        //TODO: consider allowing phase mod range to be changed
        VCO1.phaseModulation(180);
      }
    }
    
    void updateOsc2() {
      VCO2.begin(osc2Waveform);
      VCO2.amplitude(osc2Amplitude);
      if (osc2Mod == Mod_FM) {
        VCO2.frequencyModulation(12);
      }
      else {
        VCO2.phaseModulation(180);
      }
    }
    
    void updateNoise() {
      pinkNoise.amplitude(noiseAmplitude);
    }
    
    void updateWave() {
      WaveTable.amplitude(waveAmplitude);
      Wavetable.setInstrument(*GMinst[waveInstrument]);
    }
    
    public:
    void setOsc1Waveform(int wave) {
        osc1Waveform = wave;
        updateOsc1();
    }
    
    void setOsc1Octave(int oct) {
        osc1Octave = oct;
    }
    
    void setOsc1Semis(int semis) {
        osc1Semis = semis;
    }
    
    void setOsc1Detune(float detune) {
        osc1Detune = detune;
    }
    
    void setOsc1PB(float PB) {
        osc1PB = PB;
    }
    
    void setOsc1Mod(osc_mot_t mod) {
        osc1Mod = mod;
    }
    
    void setOsc2Waveform(int wave) {
        osc2Waveform = wave;
        updateOsc2();
    }
    
    void setOsc2Octave(int oct) {
        osc2Octave = oct;
    }
    
    void setOsc2Semis(int semis) {
        osc2Semis = semis;
    }
    
    void setOsc2Detune(float detune) {
        osc2Detune = detune;
    }
    
    void setOsc2PB(float PB) {
        osc2PB = PB;
    }
    
    void setOsc2Mod(osc_mot_t mod) {
        osc2Mod = mod;
    }
    
    void setOsc1Amp(float amp) {
        osc1Amp = amp;
        updateMix(OSC1);
    }
    
    void setOsc2Amp(float amp) {
        osc2Amp = amp;
        updateMix(OSC2);
    }
    
    void setNoiseAmp(float amp) {
        noiseAmp = amp;
        updateMix(Noise);
    }
    
    void setWaveAmp(float amp) {
        waveAmp = amp;
        updateMix(WaveTable);
    }
    
    void setADSRAttack(float attack) {
        envAttack = attack;
        updateADSR(Attack);
    }
    
    void setADSRDecay(float decay) {
        envDecay = decay;
        updateADSR(Decay);
    }
    
    void setADSRSustain(float sus) {
        envSustain = sus;
        updateADSR(Sustain);
    }
    
    void setADSRRelease(float rel) {
        envRelease = rel;
        updateADSR(Release);
    }
    
    
};
// PhatBass: end automatically generated code
