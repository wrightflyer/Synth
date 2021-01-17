#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>


 
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

    LFOSources() { // constructor (this is called when class-object is created)

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
        updateLFO1();
    }
    
    void setLFO2Waveform(int wave) {
        lfo2Waveform = wave;
        updateLFO2();
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
