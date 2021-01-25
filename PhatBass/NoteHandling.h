#pragma once

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "types.h"


#include "NoteData.h"
 
// PhatBass: begin automatically generated code

/**
 * Subscribes and receives MIDI note on/off and passes on to oscillators
 */
class NoteHandling
{
public:

    NoteHandling() { // constructor (this is called when class-object is created)

        usbMIDI.setHandleNoteOff(OnNoteOff);
        usbMIDI.setHandleNoteOn(OnNoteOn);
        MIDI.setHandleNoteOff(OnNoteOff);
        MIDI.setHandleNoteOn(OnNoteOn);
        
    }

    public:
    
    static void OnNoteOn(byte channel, byte note, byte velocity) {
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
    
    static void OnNoteOff(byte channel, byte note, byte velocity) {
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
    
    
};
// PhatBass: end automatically generated code
