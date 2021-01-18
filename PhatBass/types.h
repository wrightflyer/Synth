
// ****** Start Of Included File:types.h ****** 
#ifndef _TYPES_H_
#define _TYPES_H_

#include <Arduino.h>
#include <Audio.h>
#include <ILI9341_t3.h>
#include "defines.h"

typedef enum {
  Mix_AllMix,
  Mix_OSC1,
  Mix_OSC2,
  Mix_Noise,
  Mix_WaveTable
} Mix_change_t;

typedef enum {
  Env_AllADSR,
  Env_Attack,
  Env_Decay,
  Env_Sustain,
  Env_Release
} ADSR_change_t;

typedef enum {
  Filt_FILT_OFF,
  Filt_LPF,
  Filt_BPF,
  Filt_HPF
} filter_band_t;

typedef enum {
  Arp_Off,
  Arp_Up,
  Arp_Down,
  Arp_UpDown,
  Arp_Random,
  Arp_Scale,
  Arp_Record
} arp_mode_t;

typedef enum {
  Mod_FM,
  Mod_PM
} osc_mod_t;

typedef struct {
  uint8_t type;
  float offset;
} keyb_t;

typedef struct {
  char name[20]; // max is actually 15
  int entries;
  char offsets[14]; // max actually 12
} scale_chord_t;

#endif

// ****** End Of Included file:types.h ******
