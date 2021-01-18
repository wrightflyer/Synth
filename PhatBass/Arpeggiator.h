#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "types.h"

#include "Synth.h"
 
// PhatBass: begin automatically generated code

class Arpeggiator
{
public:
    const static int NumStoredNotes = 32;
    // Arpeggiator
    private:
    arp_mode_t arpMode;
    int arpOctave;
    float arpPeriod;
    bool arpLatch;
    float arpDelay;
    bool arpDelayActive;
    int arpTranspose;
    byte arpNotes[NumStoredNotes]; 
    int arpStoreIndex;
    int arpPlayIndex;
    int arpNumDown;
    int arpIncrement;
    int arpPlayOctave;
    int arpScaleMode;
    
    Synth & mSynth;
    

    Arpeggiator() { // constructor (this is called when class-object is created)

        arpMode = Arp_Off;
        arpLatch = false;
        arpOctave = 0;
        arpPeriod = 1000;
        arpDelay = 0;
        arpTranspose = 0;
        
    }

    private:
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
    
    public:
    void update() {
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
    }
    
    void setArpOctave(int octave) {
        arpOctave = octave;
    }
    
    void setArpPeriod(float period) {
        arpPeriod = period;
    }
    
    void setArpLatch(bool latch) {
        arpLatch = latch;
        if (latch == 0) {
            arpPlayIndex = 0;
            arpStoreIndex = 0;
            arpPlayOctave = 0;
            //        Serial.println("Osc off because latch Off");
            oscillatorsOff();
        }
    }
    
    void setArpDelay(float delay) {
        arpDelay = delay;
    }
    
    void setArpDelayActive(bool active) {
        arpDelayActive = active;
    }
    
    void setArpTranspose(int transpose) {
        arpTranspose = transpose;
    }
    
    void setArpScaleMode(int n) {
        setArpScaleMode = n;
    }
    
    void setSynth(Synth & refSynth) {
        mSynth = refSynth;
    }
    
    void setArpMode(int value) {
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
    }
    
};
// PhatBass: end automatically generated code
