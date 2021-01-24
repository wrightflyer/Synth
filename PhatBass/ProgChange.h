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
 * Subscribes to MIDI Prog Change and sets wavetable
 */
class ProgChange
{
public:
    SynthEngine * mpSynth;
    

    ProgChange() { // constructor (this is called when class-object is created)

        mpSynth = SyntheEngine::getInst();
        usbMIDI.setHandleProgramChange(onProgramChange);
        MIDI.setHandleProgramChange(onProgramChange);
        
    }

    static void onProgramChange(byte channel, byte program) {
      waveInstrument = program & 0x7F; // currently so limit 0..127
      Serial.printf("Progran change: %u = %s\n", waveInstrument, instrumentNames[waveInstrument]);
      updateWave();
    }
    
    
};
// PhatBass: end automatically generated code
