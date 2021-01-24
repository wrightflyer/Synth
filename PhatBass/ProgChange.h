#pragma once

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "types.h"
#include "SynthEngine.h"
#include "Arpeggiator.h"



 
// PhatBass: begin automatically generated code

/**
 * Subscribes to MIDI Prog Change and sets wavetable
 */
class ProgChange
{
public:

    ProgChange() { // constructor (this is called when class-object is created)

        usbMIDI.setHandleProgramChange(onProgramChange);
        MIDI.setHandleProgramChange(onProgramChange);
        
    }

    static void onProgramChange(byte channel, byte program) {
        program &= 0x7F;
        for (int i = 0; i < NUM_VOICES; i++) {
            SyntheEngine::voices[i].setWaveInstrument(program); // currently so limit 0..127
        }
        Serial.printf("Progran change: %u = %s\n", program, instrumentNames[program]);
    }
    
    
};
// PhatBass: end automatically generated code
