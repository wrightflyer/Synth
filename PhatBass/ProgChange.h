#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "types.h"

#include "Synth.h"
 
// PhatBass: begin automatically generated code

class ProgChange
{
public:
    Synth & mSynth;
    
    

    ProgChange() { // constructor (this is called when class-object is created)

        usbMIDI.setHandleProgramChange(onProgramChange);
        MIDI.setHandleProgramChange(onProgramChange);
        
    }

    static void onProgramChange(byte channel, byte program) {
      waveInstrument = program & 0x7F; // currently so limit 0..127
      Serial.printf("Progran change: %u = %s\n", waveInstrument, instrumentNames[waveInstrument]);
      updateWave();
    }
    
    void setSynth(Synth & refSynth) {
        mSynth = refSynth;
    }
    
    
};
// PhatBass: end automatically generated code
