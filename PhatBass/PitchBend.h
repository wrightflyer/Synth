#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "types.h"

#include "Synth.h"
 
// PhatBass: begin automatically generated code

class PitchBend
{
public:
    Synth & mSynth;
    
    

    PitchBend() { // constructor (this is called when class-object is created)

        usbMIDI.setHandlePitchChange(onPitchChange);
        MIDI.setHandlePitchBend(onPitchChange);
        
    }

    static void onPitchChange(byte channel, int pitch) {
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
    
    void setSynth(Synth & refSynth) {
        mSynth = refSynth;
    }
    
    
};
// PhatBass: end automatically generated code
